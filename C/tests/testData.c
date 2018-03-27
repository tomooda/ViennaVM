#include <stdio.h>
#include <data.h>
static int test_oid2int();

int main()
{
  if (test_oid2int()) {
    printf("test_oid2int: passed\n");
  } else {
    printf("test_oid2int: failed\n");
  };
  return 0;
}

int test_oid2int() {
  Int expected = (Int)-0x123456789abcdef;
  Int actual = oid2int(int2oid(expected));
  if (expected != actual) {
    printf("expected: %llx, actual: %llx\n", expected, actual);
    return 0;
  }
  return 1;
}
