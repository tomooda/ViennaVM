#include <stdio.h>
#include "cUnit.h"
#include "../src/activationRecord.h"
#include "../src/memory.h"
#include "../src/data.h"

extern int testActivationRecord();
static int test_alloc_ar();

int main()
{
  return testActivationRecord();
}

int testActivationRecord()
{
  memory_reset();
  register_reset();
  runTest(test_alloc_ar);
  return 0;
}

static int test_alloc_ar() {
  Pointer ar = alloc_ar(invalidPointerValue, 1, 0, invalidPointerValue, 2);
  assertEquals(read_ret_reg(ar), 1, "r1 <- retvalue");
  assertEquals(read_return_ip(ar), 0, "ip <- 0");
  assertEquals(read_return_cr(ar), invalidPointerValue, "cr <- nil");
  write_local(ar, 1, int2oid(100));
  assertEquals(oid2int(read_local(ar, 1)), 100, "local(1) = 100");
  return 0;
}
