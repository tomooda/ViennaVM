#ifndef _MEMORY_H_
#define _MEMORY_H_

#include "data.h"

typedef struct {
  Pointer address;
  Qword size;
  Pointer endAddress;
} Slot;

/* constants on memory space */
#define ALIGNMENT               64
#define HEAP_PAGE_SIZE          (1024 * 1024) // 4 * 1024
#define POOL_PAGE_SIZE          (HEAP_PAGE_SIZE / ALIGNMENT)

/* data structure of allocated memory */
#define MAX_OBJECT_SIZE         1024
#define SIZE_OFFSET             0
#define FLAGS_OFFSET            4
#define REFERENCE_COUNT_OFFSET  8
#define FORWARDER_OFFSET        16
#define SLOTS_SIZE_OFFSET       24
#define CONTENT_OFFSET          32

/* flags in *((Dword *)(pointer+FLAGS_OFFSET)) */
#define LARGE_OBJECT_FLAG       1

static inline Qword align(Qword x) {
  return ((x - 1) / ALIGNMENT + 1) * ALIGNMENT;
}

/* state variables */
extern Byte ** heapPages;
extern Slot ** poolPages;
extern int * poolSizes;

/* operations */

extern void memory_reset();
extern Pointer alloc(Qword num_slots);
extern void release(Pointer pointer);
extern void add_page();

static inline Qword basic_read(Pointer pointer) {
  return *((Qword *)(heapPages[pointer / HEAP_PAGE_SIZE]+(pointer % HEAP_PAGE_SIZE)));
}

static inline void basic_write(Pointer pointer, Qword data) {
  *((Qword *)(heapPages[pointer / HEAP_PAGE_SIZE]+(pointer % HEAP_PAGE_SIZE))) = data;
}

static inline void basic_init(Pointer pointer) {
  *((Qword *)(heapPages[pointer / HEAP_PAGE_SIZE]+(pointer % HEAP_PAGE_SIZE))) = invalidOidValue;
}

static inline Dword basic_read_dword(Pointer pointer) {
  return *((Dword *)(heapPages[pointer / HEAP_PAGE_SIZE]+(pointer % HEAP_PAGE_SIZE)));
}

static inline void basic_write_dword(Pointer pointer, Dword data) {
  *((Dword *)(heapPages[pointer / HEAP_PAGE_SIZE]+(pointer % HEAP_PAGE_SIZE))) = data;
}

static inline Byte basic_read_byte(Pointer pointer) {
  return *((Byte *)(heapPages[pointer / HEAP_PAGE_SIZE]+(pointer % HEAP_PAGE_SIZE)));
}

static inline void basic_write_byte(Pointer pointer, Byte data) {
  *((Byte *)(heapPages[pointer / HEAP_PAGE_SIZE]+(pointer % HEAP_PAGE_SIZE))) = data;
}

static inline void increment_reference_count(Pointer pointer) {
  ++(*((Qword *)(heapPages[pointer/HEAP_PAGE_SIZE]+pointer%HEAP_PAGE_SIZE+REFERENCE_COUNT_OFFSET)));
}

static inline void decrement_reference_count(Pointer pointer) {
  if (! --(*((Qword *)(heapPages[pointer/HEAP_PAGE_SIZE]+pointer%HEAP_PAGE_SIZE+REFERENCE_COUNT_OFFSET)))) release(pointer);
}

static inline Qword read_reference_count(Pointer pointer) {
  return *((Qword *)(heapPages[pointer/HEAP_PAGE_SIZE]+pointer%HEAP_PAGE_SIZE+REFERENCE_COUNT_OFFSET));
}

static inline Dword slots_size(Pointer pointer) {
  return basic_read(pointer + SLOTS_SIZE_OFFSET);
}

static inline OID read_slot(Pointer pointer, Int index) {
  return basic_read(pointer + CONTENT_OFFSET + (index - 1) * 8);
}

extern void write_slot(Pointer pointer, Int index, OID oid);

static inline void init_slot(Pointer pointer, Int index, OID oid) {
  if (isPointer(oid) && oid2pointer(oid) != invalidPointerValue) {
    increment_reference_count(oid2pointer(oid));
  }
  basic_write(pointer + CONTENT_OFFSET + (index - 1) * 8, oid);
}

static inline void init_slot_p(Pointer pointer, Int index, Pointer p) {
  if (p != invalidPointerValue) {
    increment_reference_count(p);
  }
  basic_write(pointer + CONTENT_OFFSET + (index - 1) * 8, p);
}

static inline void init_slot_i(Pointer pointer, Int index, Int i) {
  basic_write(pointer + CONTENT_OFFSET + (index - 1) * 8, int2oid(i));
}

#endif
