#include "../src/codeRecord.h"
#include "../src/data.h"
#include "../src/register.h"
#include "../src/instruction.h"
#include "cUnit.h"

extern int testCodeRecord();
static int test_sum_by_loop();
static int test_sum_by_recursion();
static int test_fibonacci();
static int test_fibonacci20();

static OID _ = invalidOidValue;

int main()
{
  return testCodeRecord();
}

int testCodeRecord()
{
  runTest(test_sum_by_loop);
  runTest(test_sum_by_recursion);
  runTest(test_fibonacci);
  runTest(test_fibonacci20);
  return 0;
}

int test_sum_by_loop() {
  Instruction src[10] = {
    {MOVEI, 1, 0, 0, int2oid(0), NULL, NULL},
    {MOVEI, 3, 0, 0, int2oid(10), NULL, NULL},
    {MOVEI, 2, 4, 0, int2oid(1), NULL, NULL},
    {MOVEI, 5, 0, 0, _, "loop", NULL},
    {ADD, 1, 1, 2, _, NULL, "loop"},
    {ADD, 2, 2, 4, _, NULL, NULL},
    {LESSTHAN, 6, 3, 2, _, NULL, NULL},
    {JUMPFALSE, 6, 5, 0, _, NULL, NULL},
    {ERR, 0, 0, 0, _, NULL, NULL},
    {0xffff, 0, 0, 0, _, NULL, NULL}};
  memory_reset();
  register_reset();
  Pointer cr = assemble(invalidOidValue, invalidOidValue, src);
  assertEquals(read_code_size(cr), 13, "code size = 13");
  assertEquals(read_num_regs(cr), 6, "uses r1-r6");
  assertEquals(read_argtypes(cr), invalidOidValue, "no argtypes");
  assertEquals(read_rettype(cr), invalidOidValue, "no rettype");
  write_cr(cr);
  write_ip(0);
  while (read_ip() < 12) {
    step();
  }
  assertEquals(read_int(2), 11, "r2 = 11 (just over the stop value=10)");
  assertEquals(read_int(1), 55, "r1 = 55 (sum of 0,...,10)");
  return 0;
}

static int test_sum_by_recursion() {
  Instruction src[9] = {
    /* r1: 1, i-1, sum(i-1), r2: i */
    {MOVEI, 1, 0, 0, int2oid(1), NULL, NULL},
    {ARG1, 2, 0, 0, _, NULL, NULL},
    {EQUAL, 3, 1, 2, _, NULL, NULL},
    {RETTRUE, 3, 1, 0, _, NULL, NULL},
    {SUB, 1, 2, 1, _, NULL, NULL},
    {CALLREC1, 1, 1, 0, _, NULL, NULL},
    {ADD, 1, 2, 1, _, NULL, NULL},
    {RET, 1, 0, 0, _, NULL, NULL},
    {0xffff, 0, 0, 0, _, NULL, NULL}};
  memory_reset();
  register_reset();
  Pointer cr = assemble(invalidOidValue, invalidOidValue, src);
  assertEquals(read_code_size(cr), 9, "code size = 9");
  assertEquals(read_num_regs(cr), 3, "uses r1-r3");
  assertEquals(read_argtypes(cr), invalidOidValue, "no argtypes");
  assertEquals(read_rettype(cr), invalidOidValue, "no rettype");
  write_cr(invalidPointerValue);
  write_ip(0);
  write_int(1, 10);
  write_pointer(2, cr);
  call1(1, 2, 1);
  while (read_cr() != invalidPointerValue) {
    step();
  }
  assertEquals(read_int(1), 55, "r1 = 55 (sum of 0,...,10)");
  return 0;
}

static int test_fibonacci() {
  Instruction src[11] = {
    /* r1: 1, i-1, sum(i-1), r2: i */
    {MOVEI,    1, 0, 0, int2oid(1), NULL, NULL},
    {ARG1,     2, 0, 0, _, NULL, NULL},
    {LESSTHAN, 3, 1, 2, _, NULL, NULL},
    {RETFALSE, 3, 2, 0, _, NULL, NULL},
    {SUB,      2, 2, 1, _, NULL, NULL},
    {CALLREC1, 3, 2, 0, _, NULL, NULL},
    {SUB,      2, 2, 1, _, NULL, NULL},
    {CALLREC1, 1, 2, 0, _, NULL, NULL},
    {ADD,      1, 1, 3, _, NULL, NULL},
    {RET,      1, 0, 0, _, NULL, NULL},
    {0xffff,   0, 0, 0, _, NULL, NULL}};
  memory_reset();
  register_reset();
  Pointer cr = assemble(invalidOidValue, invalidOidValue, src);
  assertEquals(read_code_size(cr), 11, "code size = 11");
  assertEquals(read_num_regs(cr), 3, "uses r1-r3");
  assertEquals(read_argtypes(cr), invalidOidValue, "no argtypes");
  assertEquals(read_rettype(cr), invalidOidValue, "no rettype");
  write_cr(invalidPointerValue);
  write_ip(0);
  write_int(1, 5);
  write_pointer(2, cr);
  call1(1, 2, 1);
  while (read_cr() != invalidPointerValue) {
    step();
  }
  assertEquals(read_int(1), 5, "r1 = 5 (fib(5) = 5");
  return 0;
}

static int test_fibonacci20() {
  Instruction src[11] = {
    /* r1: 1, i-1, sum(i-1), r2: i */
    {MOVEI,    1, 0, 0, int2oid(1), NULL, NULL},
    {ARG1,     2, 0, 0, _, NULL, NULL},
    {LESSTHAN, 3, 1, 2, _, NULL, NULL},
    {RETFALSE, 3, 2, 0, _, NULL, NULL},
    {SUB,      2, 2, 1, _, NULL, NULL},
    {CALLREC1, 3, 2, 0, _, NULL, NULL},
    {SUB,      2, 2, 1, _, NULL, NULL},
    {CALLREC1, 1, 2, 0, _, NULL, NULL},
    {ADD,      1, 1, 3, _, NULL, NULL},
    {RET,      1, 0, 0, _, NULL, NULL},
    {0xffff,   0, 0, 0, _, NULL, NULL}};
  memory_reset();
  register_reset();
  Pointer cr = assemble(invalidOidValue, invalidOidValue, src);
  assertEquals(read_code_size(cr), 11, "code size = 11");
  assertEquals(read_num_regs(cr), 3, "uses r1-r3");
  assertEquals(read_argtypes(cr), invalidOidValue, "no argtypes");
  assertEquals(read_rettype(cr), invalidOidValue, "no rettype");
  write_cr(invalidPointerValue);
  write_ip(0);
  write_int(1, 20);
  write_pointer(2, cr);
  call1(1, 2, 1);
  while (read_cr() != invalidPointerValue) {
    step();
  }
  assertEquals(read_int(1), 6765, "r1 = 6765 (fib(20) = 6765");
  return 0;
}

