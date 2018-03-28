#include <stdlib.h>
#include <stdio.h>
#include "memory.h"

Byte ** heapPages = NULL;
Slot ** poolPages = NULL;
int num_pages = 0;
int max_pages = 0;

int poolCleanerPageIndex = 0;
int poolCleanerIndex = 0;

void reset() {
  heapPages = malloc(sizeof(void *) * 1024);
  poolPages = malloc(sizeof(void *) * 1024);
  max_pages = 1024;
  for (int i = 1; i < max_pages; i++) {
    heapPages[i] = NULL;
    poolPages[i] = NULL;
  }
  heapPages[0] = malloc(HEAP_PAGE_SIZE);
  poolPages[0] = malloc(sizeof(Slot) * POOL_PAGE_SIZE);
  num_pages = 1;
  poolCleanerPageIndex = 0;
  poolCleanerIndex = 0;
  
}

Pointer alloc(Qword num_slots) {
  Qword size = align(CONTENT_OFFSET + num_slots * 8);
  for (int page = 0; page < num_pages; page++) {
    Slot *pool = poolPages[page];
    for (int i = 0; i < POOL_PAGE_SIZE; i++) {
      if (pool[i].size >= size) {
	Pointer address = pool[i].address;
	Pointer pointer = page * HEAP_PAGE_SIZE + address;
	pool[i].address += size;
	pool[i].size -= size;
	basic_write_dword(pointer + SIZE_OFFSET, size);
	basic_write_dword(pointer + FLAGS_OFFSET, 0);
	basic_write(pointer + REFERENCE_COUNT_OFFSET, 0);
	basic_write(pointer + FORWARDER_OFFSET, invalidPointerValue);
	basic_write(pointer + SLOTS_SIZE_OFFSET, num_slots);
	for (int s = 0; s < num_slots; s++) {
	  basic_write(pointer + CONTENT_OFFSET + s * 8, invalidOidValue);
	}
	return pointer;
      }
    }
  }
  return invalidPointerValue;
}

void freePointer(Pointer pointer) {
  Dword size = basic_read_dword(pointer + SIZE_OFFSET);
  int page = pointer / HEAP_PAGE_SIZE;
  Slot *pool = poolPages[page];
  int address = pointer % HEAP_PAGE_SIZE;
  int slotsSize = slots_size(pointer);
  basic_write_dword(pointer+SIZE_OFFSET, 0);
  for (int i = 0; i < slotsSize; i++) {
    Pointer p = oid2pointer(basic_read(pointer + CONTENT_OFFSET + 1 * 8));
    if (p != invalidPointerValue) {
      decrement_reference_count(p);
    }
  }
  for (int i = 0; i < POOL_PAGE_SIZE; i++) {
    if (pool[i].address + pool[i].size == address) {
      pool[i].size += size;
      return;
    }
    if (address + size == pool[i].address) {
      pool[i].address = address;
      pool[i].size += size;
      return;
    }
  }
  for (int i = 0; i < POOL_PAGE_SIZE; i++) {
    if (!pool[i].size) {
      pool[i].address = address;
      pool[i].size = size;
      return;
    }
  }
  fprintf(stderr, "can't free a pointer. something must have gone wrong!\n");
  exit(1);
}
