#include <stdlib.h>
#include <stdio.h>
#include "memory.h"

Byte ** heapPages = NULL;
Slot ** poolPages = NULL;
int num_pages = 0;
int max_pages = 1024;

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
  num_pages = 0;
  add_page();
  poolCleanerPageIndex = 0;
  poolCleanerIndex = 0;
  
}

void add_page() {
  if (num_pages >= max_pages) {
    fprintf(stderr, "Fatal: exceeded the max number of memory pages.\n");
    exit(1);
  }
  Slot *pool = malloc(sizeof(Slot) * POOL_PAGE_SIZE);
  Byte *heap = malloc(HEAP_PAGE_SIZE);
  if (!pool || !heap) {
    fprintf(stderr, "Fatal: failed to malloc a new page.\n");
    exit(1);
  }
  pool[0].address = 0;
  pool[0].size = HEAP_PAGE_SIZE;
  for (int i = 1; i < POOL_PAGE_SIZE; i++) {
    pool[i].address = 0;
    pool[i].size = 0;
  }
  heapPages[num_pages] = heap;
  poolPages[num_pages] = pool;
  ++num_pages;
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
  add_page();
  return alloc(num_slots);
}

void release(Pointer pointer) {
  Dword size = basic_read_dword(pointer + SIZE_OFFSET);
  Dword page = pointer / HEAP_PAGE_SIZE;
  Slot *pool = poolPages[page];
  Dword address = pointer % HEAP_PAGE_SIZE;
  Dword slotsSize = slots_size(pointer);
  basic_write_dword(pointer+SIZE_OFFSET, 0);
  for (int i = 0; i < slotsSize; i++) {
    Pointer p = oid2pointer(basic_read(pointer + CONTENT_OFFSET + i * 8));
    if (p != invalidPointerValue) {
      decrement_reference_count(p);
    }
  }
  for (int i = 0; i < POOL_PAGE_SIZE; i++) {
    if (!pool[i].size) continue;
    if (pool[i].address + pool[i].size == address) {
      Dword endAddress = address + size;
      for (int j = 0; j < POOL_PAGE_SIZE; j++) {
	if (pool[j].size && pool[j].address == endAddress) {
	  pool[i].size += size + pool[j].size;
	  pool[j].address = 0;
	  pool[j].size = 0;
	}
      }
      pool[i].size += size;
      return;
    }
    if (address + size == pool[i].address) {
      for (int j = 0; j < POOL_PAGE_SIZE; j++) {
	if (pool[j].size && pool[j].address + pool[j].size == address) {
	  pool[i].address = pool[j].address;
	  pool[i].size += pool[j].size + size;
	  pool[j].address = 0;
	  pool[j].size = 0;
	  return;
	}
      }
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
