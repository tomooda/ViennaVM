#include <stdlib.h>
#include <stdio.h>
#include "memory.h"

Byte ** heapPages = NULL;
Slot ** poolPages = NULL;
int * poolSizes = NULL;

int num_pages = 0;
int max_pages = 1024;

void memory_reset() {
  heapPages = malloc(sizeof(void *) * 1024);
  poolPages = malloc(sizeof(void *) * 1024);
  poolSizes = malloc(sizeof(int) * 1024);
  max_pages = 1024;
  for (int i = 1; i < max_pages; i++) {
    heapPages[i] = NULL;
    poolPages[i] = NULL;
    poolSizes[i] = 0;
  }
  num_pages = 0;
  add_page();
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
  pool[0].endAddress = HEAP_PAGE_SIZE;
  for (int i = 1; i < POOL_PAGE_SIZE; i++) {
    pool[i].address = 0;
    pool[i].size = 0;
    pool[i].endAddress = 0;
  }
  heapPages[num_pages] = heap;
  poolPages[num_pages] = pool;
  poolSizes[num_pages] = 1;
  ++num_pages;
}

Pointer alloc(Qword num_slots) {
  Qword size = align(CONTENT_OFFSET + num_slots * 8);
  for (int page = 0; page < num_pages; page++) {
    Slot *pool = poolPages[page]-1;
    int poolSize = poolSizes[page];
    
    for (int i = poolSize; i; --i) {
      if ((++pool)->size >= size) {
	Pointer address = pool->address;
	Pointer pointer = page * HEAP_PAGE_SIZE + address;
	pool->address += size;
	pool->size -= size;
	basic_write_dword(pointer + SIZE_OFFSET, size);
	basic_write_dword(pointer + FLAGS_OFFSET, 0);
	basic_write(pointer + REFERENCE_COUNT_OFFSET, 0);
	basic_write(pointer + FORWARDER_OFFSET, invalidPointerValue);
	basic_write(pointer + SLOTS_SIZE_OFFSET, num_slots);
	Pointer p = pointer + CONTENT_OFFSET - 8;
	for (int s = 0; s < num_slots; s++) {
	  basic_write((p += 8), invalidOidValue);
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
  int poolSize = poolSizes[page];
  Dword address = pointer % HEAP_PAGE_SIZE;
  Dword slotsSize = slots_size(pointer);
  Dword endAddress = address + size;
  Slot *vacant = NULL;
  basic_write_dword(pointer+SIZE_OFFSET, 0);
  Pointer pointer_i = pointer + CONTENT_OFFSET - 8;
  for (int i = 0; i < slotsSize; i++) {
    Pointer p = oid2pointer(basic_read(pointer_i += 8));
    if (p != invalidPointerValue) {
      decrement_reference_count(p);
    }
  }
  Slot *pool_i = pool - 1;
  Slot *pool_j;
  for (int i = 0; i < poolSize; i++) {
    if (!((++pool_i)->size)) {
      vacant = pool_i;
      continue;
    }
    if (pool_i->endAddress == address) {
      pool_j = pool + i;
      for (int j = i+1; j <= poolSize; j++) {
	if ((++pool_j)->size && pool_j->address == endAddress) {
	  pool_i->size += size + pool_j->size;
	  pool_i->endAddress = pool_j->endAddress;
	  pool_j->address = 0;
	  pool_j->size = 0;
	  pool_j->endAddress = 0;
	  if (j == poolSize - 1) {
	    while (j && !pool[j].size) {
	      --j;
	    }
	    poolSizes[page] = j;
	  }
	  return;
	}
      }
      pool_i->size += size;
      pool_i->endAddress = endAddress;
      return;
    }
    if (endAddress == pool_i->address) {
      pool_j = pool + i;
      for (int j = i+1; j <= poolSize; j++) {
	if ((++pool_j)->size && pool_j->endAddress == address) {
	  pool_i->address = pool_j->address;
	  pool_i->size += pool_j->size + size;
	  pool_j->address = 0;
	  pool_j->size = 0;
	  pool_j->endAddress = 0;
	  if (j == poolSize - 1) {
	    while (j && !pool_j->size) {
	      --j;
	    }
	    poolSizes[page] = j;
	  }
	  return;
	}
      }
      pool_i->address = address;
      pool_i->size += size;
      return;
    }
  }
  if (vacant) {
      vacant->address = address;
      vacant->size = size;
      vacant->endAddress = endAddress;
      return;
  }
  pool_i = pool+poolSize-1;
  for (int i = poolSize; i < POOL_PAGE_SIZE; i++) {
    if (!((++pool_i)->size)) {
      pool_i->address = address;
      pool_i->size = size;
      pool_i->endAddress = endAddress;
      return;
    }
  }
  fprintf(stderr, "can't free a pointer. something must have gone wrong!\n");
  exit(1);
}

void write_slot(Pointer pointer, Int index, OID oid) {
  Pointer address = pointer + CONTENT_OFFSET + (index - 1) * 8;
  Pointer oldOid = basic_read(address);
  if (isPointer(oid)) {
    increment_reference_count(oid2pointer(oid));
  }
  if (isPointer(oldOid)) {
    decrement_reference_count(oid2pointer(oldOid));
  }
  basic_write(address, oid);
}
