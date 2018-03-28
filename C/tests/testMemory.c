#include <stdio.h>
#include "../src/memory.h"

extern int testMemory();
static int test_align();

int main()
{
  return testMemory();
}

int testMemory()
{
  if (test_align()) {
    printf("test_align: failed\n");
    return 1;
  };
  printf("testMemory: all passed\n");
  return 0;
}

int test_align() {
  for (int i = 1; i <= ALIGNMENT; i++) {
    Int expected = ALIGNMENT;
    Int actual = align(i);
    if (expected != actual) {
      printf("expected: %lld, actual: %lld\n", expected, actual);
      return 1;
    }
  }
  for (int i = ALIGNMENT + 1; i <= ALIGNMENT * 2; i++) {
    Int expected = ALIGNMENT * 2;
    Int actual = align(i);
    if (expected != actual) {
      printf("expected: %lld, actual: %lld\n", expected, actual);
      return 1;
    }
  }
  return 0;
}
