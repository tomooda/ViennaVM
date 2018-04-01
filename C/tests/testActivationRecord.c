#include <stdio.h>
#include "cUnit.h"
#include "../src/activationRecord.h"
#include "../src/memory.h"
#include "../src/data.h"

extern int testActivationRecord();
static int test_alloc_ar();
static int test_alloc_ar0();
static int test_alloc_ar1();

int main()
{
  return testActivationRecord();
}

int testActivationRecord()
{
  memory_reset();
  register_reset();
  runTest(test_alloc_ar);
  runTest(test_alloc_ar0);
  runTest(test_alloc_ar1);
  return 0;
}

static int test_alloc_ar() {
  Pointer args = alloc(3);
  write_slot(args, 1, int2oid(10));
  write_slot(args, 2, int2oid(20));
  write_slot(args, 3, int2oid(30));
  Pointer ar = alloc_ar(invalidPointerValue, args, 1, 0, invalidPointerValue, 2);
  assertEquals(oid2int(read_arg(ar, 1)), 10, "arg(1) = 10");
  assertEquals(oid2int(read_arg(ar, 2)), 20, "arg(2) = 20");
  assertEquals(oid2int(read_arg(ar, 3)), 30, "arg(3) = 30");
  assertEquals(read_ret_reg(ar), 1, "r1 <- retvalue");
  assertEquals(read_return_ip(ar), 0, "ip <- 0");
  assertEquals(read_return_cr(ar), invalidPointerValue, "cr <- nil");
  write_local(ar, 1, int2oid(100));
  assertEquals(oid2int(read_local(ar, 1)), 100, "local(1) = 100");
  return 0;
}

static int test_alloc_ar0() {
  Pointer ar = alloc_ar0(invalidPointerValue, 1, 0, invalidPointerValue, 2);
  assertEquals(read_ret_reg(ar), 1, "r1 <- retvalue");
  assertEquals(read_return_ip(ar), 0, "ip <- 0");
  assertEquals(read_return_cr(ar), invalidPointerValue, "cr <- nil");
  write_local(ar, 1, int2oid(100));
  assertEquals(oid2int(read_local(ar, 1)), 100, "local(1) = 100");
  return 0;
}
    
static int test_alloc_ar1() {
  Pointer ar = alloc_ar1(invalidPointerValue, int2oid(10), 1, 0, invalidPointerValue, 2);
  assertEquals(oid2int(read_arg(ar, 1)), 10, "arg1 = 10");
  assertEquals(read_ret_reg(ar), 1, "r1 <- retvalue");
  assertEquals(read_return_ip(ar), 0, "ip <- 0");
  assertEquals(read_return_cr(ar), invalidPointerValue, "cr <- nil");
  write_local(ar, 1, int2oid(100));
  assertEquals(oid2int(read_local(ar, 1)), 100, "local(1) = 100");
  return 0;
}
