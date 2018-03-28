#include <stdio.h>
#include "../src/data.h"

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
  if (test_oid2int()) {
    printf("test_oid2int: failed\n");
    return 1;
  };
  if (test_oid2float()) {
    printf("test_oid2float: failed\n");
    return 1;
  };
  if (test_oid2char()) {
    printf("test_oid2char: failed\n");
    return 1;
  };
  if (test_oid2pointer()) {
    printf("test_oid2pointer: failed\n");
    return 1;
  };
  printf("testData: all passed\n");
  return 0;
}

int test_oid2int() {
  Int expected = (Int)-0x123456789abcdef;
  Int actual = oid2int(int2oid(expected));
  if (expected != actual) {
    printf("expected: %llx, actual: %llx\n", expected, actual);
    return 1;
  }
  return 0;
}

int test_oid2float() {
  Float expected = 3.1415;
  Float actual = oid2float(float2oid(expected));
  if (expected != actual) {
    printf("expected: %f, actual: %f\n", expected, actual);
    return 1;
  }
  return 0;
}

int test_oid2char() {
  Char expected = (Char)0x40;
  Char actual = oid2char(char2oid(expected));
  if (expected != actual) {
    printf("expected: %x, actual: %x\n", expected, actual);
    return 1;
  }
  return 0;
}

int test_oid2pointer() {
  Pointer expected = (Pointer)0x1234;
  Pointer actual = oid2pointer(pointer2oid(expected));
  if (expected != actual) {
    printf("expected: %llx, actual: %llx\n", expected, actual);
    return 1;
  }
  return 0;
}

