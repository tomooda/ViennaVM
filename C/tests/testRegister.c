#include <stdio.h>
#include "cUnit.h"
#include "../src/register.h"
#include "../src/memory.h"
#include "../src/data.h"

extern int testRegister();
static int test_move();
static int testWriteAndReadDifferentRegister();
static int testBoxAndReadInt();
static int testBoxAndReadFloat();
static int testBoxAndReadChar();
static int testBoxAndReadPointer();
static int testWriteBoxedIntAndUbox();
static int testWriteBoxedFloatAndUbox();
static int testWriteBoxedCharAndUnbox();
static int testWriteBoxedPointerAndUnbox();
static int testWriteInvalidOopAndUnbox();
static int test_reference_count();

static Int int1 = 0x1234;
static Int int2 = 0x5678;
static Float float1;
static Char char1 = 0x40;
static Pointer pointer1 = 0x100;
static OID oid1;
static OID oid2;


int main()
{
  return testRegister();
}

int testRegister()
{
  memory_reset();
  register_reset();
  float1 = real2float(3.14);
  oid1 = int2oid(0x1234);
  oid2 = int2oid(0x5678);
  runTest(test_move);
  runTest(testWriteAndReadDifferentRegister);
  runTest(testBoxAndReadInt);
  runTest(testBoxAndReadFloat);
  runTest(testBoxAndReadChar);
  runTest(testBoxAndReadPointer);
  runTest(testWriteBoxedIntAndUbox);
  runTest(testWriteBoxedFloatAndUbox);
  runTest(testWriteBoxedCharAndUnbox);
  runTest(testWriteBoxedPointerAndUnbox);
  runTest(testWriteInvalidOopAndUnbox);
  runTest(test_reference_count);
  return 0;
}

int test_move() {
  Pointer p = alloc(0);
  write_int(1, 123);
  move(2, 1);
  assertEquals(read_int(2), 123, "r2 is set to 123");
  write_pointer(1, p);
  assertEquals
    (read_reference_count(p),
     1,
     "(r1).refCount = 1");
  move(2, 1);  
  assertEquals
    (read_reference_count(p),
     2,
     "(r1).refCount = 2");
  write_int(2, 0);
  assertEquals
    (read_reference_count(p),
     1,
     "(r1).refCount = 1");
  move(1, 2);
  assertEquals
    (read_reference_count(p),
     0,
     "(r1).refCount = 0");
  return 0;
}

static int testWriteAndReadDifferentRegister() {
  write_oid(1, oid1);
  write_oid(2, oid2);
  assertEquals(read_oid(1), oid1, "read oid1");
  assertEquals(read_oid(2), oid2, "read oid2");
  return 0;
}
    
static int testBoxAndReadInt() {
  write_int(1, int2);
  assertEquals(read_oid(1), int2oid(int2), "box int");
  write_int(1, invalidIntValue);
  assertEquals(read_oid(1), invalidOidValue, "box invalid int");
  return 0;
}

static int testBoxAndReadFloat() {
  write_float(1, float1);
  assertEquals(read_oid(1), float2oid(float1), "box float");
  write_float(1, invalidFloatValue);
  assertEquals(read_oid(1), invalidOidValue, "box invalid float");
  return 0;
}
    
static int testBoxAndReadChar() {
  write_char(1, char1);
  assertEquals(read_oid(1), char2oid(char1), "box char");
  write_char(1, invalidCharValue);
  assertEquals(read_oid(1), invalidOidValue, "box invalid char");
  return 0;
}
    
static int testBoxAndReadPointer() {
  write_pointer(1, pointer1);
  assertEquals(read_oid(1), pointer2oid(pointer1), "box pointer");
  write_pointer(1, invalidPointerValue);
  assertEquals(read_oid(1), invalidOidValue, "box invalid pointer");
  write_oid(1, invalidOidValue);
  assertEquals(read_oid(1), invalidOidValue, "invalid oid");
  return 0;
}
    
static int testWriteBoxedIntAndUbox() {
  write_oid(1, int2oid(int1));
  assertEquals(read_int(1), int1, "int->int");
  assertEquals(read_float(1), real2float((float)int1), "int->float");
  assertEquals(read_char(1), invalidCharValue, "int-char");
  assertEquals(read_pointer(1), invalidPointerValue, "int->pointer");
  return 0;
}
    
static int testWriteBoxedFloatAndUbox() {
  write_oid(1, float2oid(float1));
  assertEquals(read_int(1), invalidIntValue, "float->int");
  assertEquals(read_float(1), float1, "float->float");
  assertEquals(read_char(1), invalidCharValue, "float->char");
  assertEquals(read_pointer(1), invalidPointerValue, "float->pointer");
  return 0;
}
    
static int testWriteBoxedCharAndUnbox() {
  write_oid(1, char2oid(char1));
  assertEquals(read_int(1), invalidIntValue, "char->int");
  assertEquals(read_float(1), invalidFloatValue, "char->float");
  assertEquals(read_char(1), char1, "char->char");
  assertEquals(read_pointer(1), invalidPointerValue, "char->pointer");
  return 0;
}
    
static int testWriteBoxedPointerAndUnbox() {
  write_oid(1, pointer2oid(pointer1));
  assertEquals(read_int(1), invalidIntValue, "pointer->int");
  assertEquals(read_float(1), invalidFloatValue, "pointer->float");
  assertEquals(read_char(1), invalidCharValue, "pointer->char");
  assertEquals(read_pointer(1), pointer1, "pointer->pointer");
  return 0;
}
    
static int testWriteInvalidOopAndUnbox() {
  write_oid(1, invalidOidValue);
  assertEquals(read_int(1), invalidIntValue, "->int");
  assertEquals(read_float(1), invalidFloatValue, "->float");
  assertEquals(read_char(1), invalidCharValue, "->char");
  assertEquals(read_pointer(1), invalidPointerValue, "->pointer");
  return 0;
}

static int test_reference_count() {
  Pointer p1 = alloc(0);
  Pointer p2 = alloc(0);
  OID oid1 = pointer2oid(p1);
  OID oid2 = pointer2oid(p2);
  assertEquals
    (basic_read(p1 + REFERENCE_COUNT_OFFSET),
     0,
     "no referrence to p1");
  assertEquals
    (basic_read(p2 + REFERENCE_COUNT_OFFSET),
     0,
     "no referrence to p2");
  write_pointer(1, p1);
  assertEquals
    (basic_read(p1 + REFERENCE_COUNT_OFFSET),
     1,
     "1 referrence to p1");
  assertEquals
    (basic_read(p2 + REFERENCE_COUNT_OFFSET),
     0,
     "no referrence to p1");
  write_oid(1, oid2);
  assertEquals(basic_read(p1 + SIZE_OFFSET), 0, "p1 is free");
  assertEquals
    (basic_read(p2 + REFERENCE_COUNT_OFFSET),
     1,
     "1 referrence to p2");
  write_int(1, 0);
  assertEquals(basic_read(p1 + SIZE_OFFSET), 0, "p2 is free");
  return 0;
}

