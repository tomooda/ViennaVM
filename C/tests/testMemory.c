#include <stdio.h>
#include "cUnit.h"
#include "../src/memory.h"

extern int testMemory();
static int test_align();
static int test_basic_read();
static int test_basic_write();
static int test_basic_read_dword();
static int test_basic_write_dword();
static int test_basic_read_byte();
static int test_basic_write_byte();
static int test_allocate_release();
static int test_referenceCount();
static int test_read_write_slot();

int main()
{
  return testMemory();
}

int testMemory()
{
  runTest(test_align);
  runTest(test_basic_read);
  runTest(test_basic_write);
  runTest(test_basic_read_dword);
  runTest(test_basic_write_dword);
  runTest(test_basic_read_byte);
  runTest(test_basic_write_byte);
  runTest(test_allocate_release);
  runTest(test_referenceCount);
  runTest(test_read_write_slot);
  return 0;
}

int test_align() {
  for (int i = 1; i <= ALIGNMENT; i++) {
    assertEquals
      (align(i),
       ALIGNMENT,
       "align(1,...,ALIGNMENT) = ALIGNMENT");
  }
  for (int i = ALIGNMENT + 1; i <= ALIGNMENT * 2; i++) {
    assertEquals
      (align(i),
       ALIGNMENT *2,
       "align(ALIGNMENT+1, ..., ALIGNMENT * 2) = ALIGNMENT * 2");
  }
  return 0;
}

int test_basic_read() {
  memory_reset();
  add_page();
  basic_write(0x12, 0xfedcba9876543210);
  assertEquals
    (basic_read(0x12),
     0xfedcba9876543210,
     "basic_read 64bits immediate");
  basic_write_byte(0x10, 0x01);
  basic_write_byte(0x11, 0x23);
  basic_write_byte(0x12, 0x45);
  basic_write_byte(0x13, 0x67);
  basic_write_byte(0x14, 0x89);
  basic_write_byte(0x15, 0xab);
  basic_write_byte(0x16, 0xcd);
  basic_write_byte(0x17, 0xef);
  assertEquals
    (basic_read(0x10),
     0xefcdab8967452301,
     "basic_read little endian");
  return 0;
}

int test_basic_write() {
  memory_reset();
  add_page();
  for (int i = 0; i <= 0x100; i++)
    basic_write_byte(i, 0x00);
  basic_write(0x10, 0x0123456789abcdef);
  assertEquals(basic_read_byte(0x0f), 0x00, "no basic_write on ptr-1");
  assertEquals(basic_read_byte(0x18), 0x00, "no basic_write on ptr+8");
  assertEquals(basic_read_byte(0x10), 0xef, "1st byte");
  assertEquals(basic_read_byte(0x11), 0xcd, "2nd byte");
  assertEquals(basic_read_byte(0x12), 0xab, "3rd byte");
  assertEquals(basic_read_byte(0x13), 0x89, "4th byte");
  assertEquals(basic_read_byte(0x14), 0x67, "5th byte");
  assertEquals(basic_read_byte(0x15), 0x45, "6th byte");
  assertEquals(basic_read_byte(0x16), 0x23, "7th byte");
  assertEquals(basic_read_byte(0x17), 0x01, "8th byte");
  return 0;
}

int test_basic_read_dword() {
  memory_reset();
  add_page();
  basic_write_dword(0x12, 0x12345678);
  assertEquals
    (basic_read_dword(0x12),
     0x12345678,
     "basic_read 32bits immediate");
  basic_write_byte(0x10, 0x21);
  basic_write_byte(0x11, 0x43);
  basic_write_byte(0x12, 0x65);
  basic_write_byte(0x13, 0x87);
  assertEquals
    (basic_read_dword(0x10),
     0x87654321,
     "basic_read little endian");
  return 0;
}

int test_basic_write_dword() {
  memory_reset();
  add_page();
  for (int i = 0; i <= 0x100; i++)
    basic_write_byte(i, 0x00);
  basic_write_dword(0x10, 0x89abcdef);
  assertEquals(basic_read_byte(0x0f), 0x00, "no basic_write on ptr-1");
  assertEquals(basic_read_byte(0x14), 0x00, "no basic_write on ptr+4");
  assertEquals(basic_read_byte(0x10), 0xef, "1st byte");
  assertEquals(basic_read_byte(0x11), 0xcd, "2nd byte");
  assertEquals(basic_read_byte(0x12), 0xab, "3rd byte");
  assertEquals(basic_read_byte(0x13), 0x89, "4th byte");
  return 0;
}
    
int test_basic_read_byte() {
  memory_reset();
  add_page();
  for (int i = 0; i <= 0x100; i++)
    basic_write_byte(i, 0x00);
  basic_write_byte(0x12, 0xff);
  assertEquals(basic_read(0x12), 0xff, "basic_read a byte");
  return 0;
}    

int test_basic_write_byte() {
  memory_reset();
  add_page();
  for (int i = 0; i <= 0x100; i++)
    basic_write_byte(i, 0x00);
  basic_write_byte(0x10, 0xff);
  assertEquals(basic_read_byte(0x0f), 0x00, "no basic_write on ptr-1");
  assertEquals(basic_read_byte(0x11), 0x00, "no basic_write on ptr+8");
  assertEquals(basic_read_byte(0x10), 0xff, "the byte");
  return 0;
}
    
int test_allocate_release() {
  memory_reset();
  Pointer p1 = alloc(0);
  Pointer p2 = alloc(0);
  assert(p1 != p2, "p1 <> p2");
  release(p1);
  release(p2);
  Pointer p3 = alloc(0);
  assertEquals(p3, p1, "allocated the same memory");
  return 0;
}
    
int test_referenceCount() {
  memory_reset();
  Pointer p = alloc(0x10);
  assert(basic_read_dword(p + SIZE_OFFSET) > 0, "allocated");
  assertEquals
    (basic_read(p + REFERENCE_COUNT_OFFSET),
     0,
     "refCount= 0 at the beginning");
  increment_reference_count(p);
  assertEquals
    (basic_read(p + REFERENCE_COUNT_OFFSET),
     1,
     "refCount is incremented");
  assert
    (basic_read_dword(p + SIZE_OFFSET) > 0,
     "still allocated");
  increment_reference_count(p);
  assertEquals
    (basic_read(p + REFERENCE_COUNT_OFFSET),
     2,
     "refCount is incremented");
  assert
    (basic_read_dword(p + SIZE_OFFSET) > 0,
     "still allocated");
  decrement_reference_count(p);
  assertEquals
    (basic_read(p + REFERENCE_COUNT_OFFSET),
     1,
     "refCount is decremented");
  assert
    (basic_read_dword(p + SIZE_OFFSET) > 0,
     "still allocated");
  decrement_reference_count(p);
  assert(basic_read_dword(p + SIZE_OFFSET) == 0, "free");
  return 0;
}
    
int test_read_write_slot() {
  memory_reset();
  Pointer p1 = alloc(2), p2 = alloc(1), p3 = alloc(1);
  write_slot(p1, 1, int2oid(123));
  write_slot(p1, 2, char2oid(0x41));
  assertEquals
    (oid2int(read_slot(p1, 1)),
     123,
     "read and write 123 to slot");
  assertEquals
    (oid2char(read_slot(p1, 2)),
     0x41,
     "read and write 'A'");
  write_slot(p2, 1, pointer2oid(p1));
  assertEquals
    (basic_read(p1 + REFERENCE_COUNT_OFFSET),
     1,
     "refCount of p1 is incremented");
  write_slot(p3, 1, pointer2oid(p2));
  assertEquals
    (basic_read(p2 + REFERENCE_COUNT_OFFSET),
     1,
     "refCount of p2 is incremented");
  write_slot(p3, 1, int2oid(0));
  assertEquals
    (basic_read_dword(p1 + SIZE_OFFSET),
     0,
     "p1 is free");
  assertEquals
    (basic_read_dword(p2 + SIZE_OFFSET),
     0,
     "p2 is free");
  return 0;
}
