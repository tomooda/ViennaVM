#include <stdio.h>
#include "cUnit.h"
#include "../src/data.h"

extern int testData();
static int test_oid2int();
static int test_oid2float();
static int test_oid2char();
static int test_oid2pointer();

int main()
{
  return testData();
}

int testData()
{
  runTest(test_oid2int);
  runTest(test_oid2float);
  runTest(test_oid2char);
  runTest(test_oid2pointer);
  return 0;
}

int test_oid2int() {
  assertEquals
    (oid2int(int2oid((Int)-0x123456789abcdef)),
     (Int)-0x123456789abcdef,
     "oid2int");
  return 0;
}

int test_oid2float() {
  assertApproxEquals
    (oid2float(float2oid(3.1415)),
     3.1415,
     0.00000001,
     "oid2float");
  return 0;
}

int test_oid2char() {
  assertEquals
    (oid2char(char2oid((Char)0x40)),
     (Char)0x40,
     "oid2char");
  return 0;
}

int test_oid2pointer() {
  assertEquals
    (oid2pointer(pointer2oid((Pointer)0x1234)),
     (Pointer)0x1234,
     "oid2pointer");
  return 0;
}

