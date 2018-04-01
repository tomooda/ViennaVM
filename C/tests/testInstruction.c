#include <stdio.h>
#include "cUnit.h"
#include "../src/data.h"
#include "../src/register.h"
#include "../src/memory.h"
#include "../src/instruction.h"
#include "../src/codeRecord.h"
#include "../src/activationRecord.h"

extern int testInstruction();
static int test_reset();
static int test_move();
static int test_args();
static int test_alloc();
static int test_store_load();
static int test_add();
static int test_sub();
static int test_mul();
static int test_fdiv();
static int test_idiv();
static int test_imod();
static int test_equal();
static int test_jumptrue();
static int test_jumpfalse();
static int test_jump();
static int test_call();
static int test_call0();
static int test_call1();
static int test_callrec();
static int test_callrec0();
static int test_callrec1();
static int test_callrec2();
static int test_ret();
static int test_rettrue();
static int test_retfalse();
static int test_step();

int main()
{
  return testInstruction();
}

int testInstruction()
{
  memory_reset();
  register_reset();
  runTest(test_reset);
  runTest(test_move);
  runTest(test_args);
  runTest(test_alloc);
  runTest(test_store_load);
  runTest(test_add);
  runTest(test_sub);
  runTest(test_mul);
  runTest(test_fdiv);
  runTest(test_idiv);
  runTest(test_imod);
  runTest(test_equal);
  runTest(test_jumptrue);
  runTest(test_jumpfalse);
  runTest(test_jump);
  runTest(test_call);
  runTest(test_call0);
  runTest(test_call1);
  runTest(test_callrec);
  runTest(test_callrec0);
  runTest(test_callrec1);
  runTest(test_callrec2);
  runTest(test_ret);
  runTest(test_rettrue);
  runTest(test_retfalse);
  runTest(test_step);
  return 0;
}

static int test_reset() {
  movei(1, 0, 0, int2oid(0));
  assertEquals(read_int(1), 0, "r1 is now set to 0");
  reset(1, 0, 0);
  assertEquals(read_oid(1), invalidOidValue, "reset r1 to invalid oid");
  return 0;
}
    
static int test_move() {
  reset(1, 2, 0);
  movei(1, 0, 0, int2oid(0));
  assertEquals(read_int(1), 0, "r1 is now set to 0");
  mov(2, 0, 1);
  assertEquals(read_int(2), 0, "r2 is now set to 0");
  return 0;
}
    
static int test_args() {
  Pointer argsPointer = alloc(3);
  write_slot(argsPointer, 1, int2oid(10));
  write_slot(argsPointer, 2, int2oid(20));
  write_slot(argsPointer, 3, int2oid(30));
  write_ar(alloc_ar(invalidPointerValue, argsPointer, 1, 0, invalidPointerValue, 2));
  args(1, 0, 0);
  assertEquals(read_pointer(1), argsPointer, "r1 is args");
  arg1(1, 0, 0);
  assertEquals(read_int(1), 10, "r1 is args(1) = 10");
  arg2(1, 0, 0);
  assertEquals(read_int(1), 20, "r1 is args(2) = 20");
  arg3(1, 0, 0);
  assertEquals(read_int(1), 30, "r1 is args(3) = 30");
  return 0;
}
    
static int test_alloc() {
  allocate(1, 0, 0, int2oid(0));
  Pointer p = read_pointer(1);
  assert(p != invalidPointerValue, "r1 is set to allocated heap");
  assert(basic_read(p + REFERENCE_COUNT_OFFSET) > 0,"the allocated heap is valid");
  movei(1, 0, 0, int2oid(0));
  assert(basic_read(p + REFERENCE_COUNT_OFFSET) == 0, "the allocated heap is free");
  return 0;
}
    
static int test_store_load() {
  allocate(1, 0, 0, int2oid(1));
  allocate(2, 0, 0, int2oid(0));
  Pointer p1 = read_pointer(1);
  Pointer p2 = read_pointer(2);
  assertEquals
    (basic_read(p1 + REFERENCE_COUNT_OFFSET),
     1,
     "(r1).refCount = 1");
  assertEquals
    (basic_read(p2 + REFERENCE_COUNT_OFFSET),
     1,
     "(r2).refCount = 1");
  store1(1, 0, 2);
  assertEquals
    (read_slot(read_pointer(1), 1),
     read_oid(2),
     "r1[1] = r2");
  assertEquals
    (basic_read(p1 + REFERENCE_COUNT_OFFSET),
     1,
     "(r1).refCount = 1");
  assertEquals
    (basic_read(p2 + REFERENCE_COUNT_OFFSET),
     2,
     "(r2).refCount = 2");
  reset(1, 2, 0);
  assertEquals
    (basic_read(p1 + REFERENCE_COUNT_OFFSET),
     0,
     "(r1) is free");
  assertEquals
    (basic_read(p2 + REFERENCE_COUNT_OFFSET),
     0,
     "(r2) is free");
  return 0;
}
    
static int test_add() {
  movei(1, 0, 0, int2oid(3));
  movei(2, 0, 0, int2oid(-4));
  assertEquals(read_int(1), 3, "r1 is set to 3");
  assertEquals(read_int(2), -4, "r2 is set to -4");
  add(3, 1, 2);
  assertEquals(read_int(3), -1, "r3 is set to -1");
  movei(1, 0, 0, float2oid(real2float(3.14)));
  assertEquals(read_float(1), real2float(3.14), "r1 is set to 3.14");
  add(4, 1, 2);
  assertApproxEquals
    (float2real(read_float(4)),
     -0.86,
     0.00001,
     "r4 is set to -0.86");
  return 0;
}
    
static int test_sub() {
  movei(1, 0, 0, int2oid(3));
  movei(2, 0, 0, int2oid(-4));
  assertEquals(read_int(1), 3, "r1 is set to 3");
  assertEquals(read_int(2), -4, "r2 is set to -4");
  sub(3, 1, 2);
  assertEquals(read_int(3), 7, "r3 is set to 7");
  movei(1, 0, 0, float2oid(real2float(3.14)));
  assertEquals(read_float(1), real2float(3.14), "r1 is set to 3.14");
  sub(4, 1, 2);
  assertApproxEquals
    (float2real(read_float(4)),
     7.14,
     0.00001,
     "r4 is set to 7.14");
  return 0;
}
    
static int test_mul() {
  movei(1, 0, 0, int2oid(3));
  movei(2, 0, 0, int2oid(-4));
  assertEquals(read_int(1), 3, "r1 is set to 3");
  assertEquals(read_int(2), -4, "r2 is set to -4");
  mul(3, 1, 2);
  assertEquals(read_int(3), -12, "r3 is set to -12");
  movei(1, 0, 0, float2oid(real2float(3.14)));
  assertEquals(read_float(1), real2float(3.14), "r1 is set to 3.14");
  mul(4, 1, 2);
  assertApproxEquals
    (float2real(read_float(4)),
     -12.56,
     0.00001,
     "r4 is set to  -12.56");
  return 0;
}
    
static int test_fdiv() {
  movei(1, 0, 0, float2oid(real2float(3.14)));
  movei(2, 0, 0, int2oid(-4));
  assertEquals(read_float(1), real2float(3.14), "r1 is set to 3.14");
  assertEquals(read_int(2), -4, "r2 is set to -4");
  fdiv(3, 1, 2);
  assertApproxEquals
    (float2real(read_float(3)),
     -0.785,
     0.000001,
     "r3 is set to  -0.785");
  return 0;
}
    
static int test_idiv() {
  movei(1, 0, 0, int2oid(13));
  movei(2, 0, 0, int2oid(-4));
  assertEquals(read_int(1), 13, "r1 is set to 13");
  assertEquals(read_int(2), -4, "r2 is set to -4");
  idiv(3, 1, 2);
  assertEquals(read_int(3), -3, "r3 is set to -3");
  return 0;
}
    
static int test_imod() {
  movei(1, 0, 0, int2oid(13));
  movei(2, 0, 0, int2oid(-4));
  assertEquals(read_int(1), 13, "r1 is set to 13");
  assertEquals(read_int(2), -4, "r2 is set to -4");
  imod(3, 1, 2);
  assertEquals(read_int(3), -3, "r3 is set to -3");
  return 0;
}
    
static int test_lessthan() {
  movei(1, 0, 0, int2oid(4));
  movei(2, 0, 0, float2oid(real2float(3.14)));
  lessthan(3, 1, 2);
  assertEquals(read_oid(3), falseValue, "r3 is false");
  lessthan(3, 2, 1);
  assertEquals(read_oid(3), trueValue, "r3 is true");
  return 0;
}

static int test_equal() {
  movei(1, 0, 0, int2oid(4));
  movei(2, 0, 0, float2oid(real2float(3.14)));
  equal(3, 1, 2);
  assertEquals(read_oid(3), falseValue, "r3 is false");
  movei(1, 2, 0, int2oid(-3));
  equal(3, 1, 2);
  assertEquals(read_oid(3), trueValue, "r3 is true");
  movei(1, 2, 0, float2oid(real2float(1.732)));
  equal(3, 1, 2);
  assertEquals(read_oid(3), trueValue, "r3 is true");
  return 0;
}
    
static int test_bnot() {
  movei(1, 0, 0, trueValue);
  bnot(2, 1);
  assertEquals(read_oid(2), falseValue, "r2 = not r1 = not true = false");
  bnot(3, 2);
  assertEquals(read_oid(3), trueValue, "r3 = not r2 = not false = true");
  return 0;
}
    
static int test_jumptrue() {
  write_ip(0);
  movei(1, 0, 0, trueValue);
  movei(2, 0, 0, int2oid(100));
  jumptrue(1, 2);
  assertEquals(read_ip(), 100, "jumped with true");
  write_ip(0);
  movei(1, 0, 0, falseValue);
  movei(2, 0, 0, int2oid(100));
  jumptrue(1, 2);
  assertEquals(read_ip(), 0, "not jumped with false");
  return 0;
}
    
static int test_jumpfalse() {
  write_ip(0);
  movei(1, 0, 0, trueValue);
  movei(2, 0, 0, int2oid(100));
  jumpfalse(1, 2);
  assertEquals(read_ip(), 0, "not jumped with true");
  write_ip(0);
  movei(1, 0, 0, falseValue);
  movei(2, 0, 0, int2oid(100));
  jumpfalse(1, 2);
  assertEquals(read_ip(), 100, "jumped with false");
  return 0;
}
    
static int test_jump() {
  write_ip(0);
  movei(1, 0, 0, int2oid(100));
  jump(1);
  assertEquals(read_ip(), 100, "jumped");
  return 0;
}
    
static int test_call() {
  Instruction src[4] = {
    {MOVEI, 1, 0, 0, int2oid(1), NULL, NULL},
    {MOVEI, 2, 0, 0, int2oid(2), NULL, NULL},
    {MOVEI, 3, 0, 0, int2oid(3), NULL, NULL},
    {0xffff, 0, 0, 0, invalidOidValue, NULL, NULL}};
  Pointer caller_cr = assemble(invalidOidValue, invalidOidValue, src);
  Pointer caller_ar = alloc_ar0(invalidPointerValue, 1, 0, invalidPointerValue, 0);
  Pointer callee_cr = assemble(invalidOidValue, invalidOidValue, src);
  Pointer args = alloc(1);
  write_slot(args, 1, int2oid(1));
  write_int(1, 10);
  write_pointer(2, callee_cr);
  write_pointer(3, args);
  write_cr(caller_cr);
  write_ip(2);
  write_ar(caller_ar);
  call(1, 2, 3);
  Pointer callee_ar = read_ar();
  assertEquals(read_cr(), callee_cr, "cr is callee's");
  assertEquals(read_arg(callee_ar, 1), int2oid(1), "arg1 = 1");
  assertEquals(read_dynamic_link(callee_ar), caller_ar, "dynamic link is at caller's ar");
  assertEquals(read_return_cr(callee_ar), caller_cr, "caller's cr");
  assertEquals(read_return_ip(callee_ar), 2, "caller's ip");
  assertEquals(read_ret_reg(callee_ar), 1, "return value to r1");
  assertEquals(read_local(callee_ar, 1), int2oid(10), "r1 is saved");
  assertEquals(read_local(callee_ar, 2), pointer2oid(callee_cr), "r2 is saved");
  assertEquals(read_local(callee_ar, 3), pointer2oid(args), "r3 is saved");
  return 0;
}

static int test_call0() {
  Instruction src[4] = {
    {MOVEI, 1, 0, 0, int2oid(1), NULL, NULL},
    {MOVEI, 2, 0, 0, int2oid(2), NULL, NULL},
    {MOVEI, 3, 0, 0, int2oid(3), NULL, NULL},
    {0xffff, 0, 0, 0, invalidOidValue, NULL, NULL}};
  Pointer caller_cr = assemble(invalidOidValue, invalidOidValue, src);
  Pointer caller_ar = alloc_ar0(invalidPointerValue, 1, 0, invalidPointerValue, 0);
  Pointer callee_cr = assemble(invalidOidValue, invalidOidValue, src);
  write_int(1, 10);
  write_pointer(2, callee_cr);
  write_int(3, 30);
  write_cr(caller_cr);
  write_ip(2);
  write_ar(caller_ar);
  call0(1, 2);
  Pointer callee_ar = read_ar();
  assertEquals(read_cr(), callee_cr, "cr is callee's");
  assertEquals(read_dynamic_link(callee_ar), caller_ar, "dynamic link is at caller's ar");
  assertEquals(read_return_cr(callee_ar), caller_cr, "caller's cr");
  assertEquals(read_return_ip(callee_ar), 2, "caller's ip");
  assertEquals(read_ret_reg(callee_ar), 1, "return value to r1");
  assertEquals(read_local(callee_ar, 1), int2oid(10), "r1 is saved");
  assertEquals(read_local(callee_ar, 2), pointer2oid(callee_cr), "r2 is saved");
  assertEquals(read_local(callee_ar, 3), int2oid(30), "r3 is saved");
  return 0;
}
    
static int test_call1() {
  Instruction src[4] = {
    {MOVEI, 1, 0, 0, int2oid(1), NULL, NULL},
    {MOVEI, 2, 0, 0, int2oid(2), NULL, NULL},
    {MOVEI, 3, 0, 0, int2oid(3), NULL, NULL},
    {0xffff, 0, 0, 0, invalidOidValue, NULL, NULL}};
  Pointer caller_cr = assemble(invalidOidValue, invalidOidValue, src);
  Pointer caller_ar = alloc_ar0(invalidPointerValue, 1, 0, invalidPointerValue, 0);
  Pointer callee_cr = assemble(invalidOidValue, invalidOidValue, src);
  write_int(1, 10);
  write_pointer(2, callee_cr);
  write_int(3, 30);
  write_cr(caller_cr);
  write_ip(2);
  write_ar(caller_ar);
  call1(1, 2, 3);
  Pointer callee_ar = read_ar();
  assertEquals(read_cr(), callee_cr, "cr is callee's");
  assertEquals(read_arg(callee_ar, 1), int2oid(30), "arg1 = 30");
  assertEquals(read_dynamic_link(callee_ar), caller_ar, "dynamic link is at caller's ar");
  assertEquals(read_return_cr(callee_ar), caller_cr, "caller's cr");
  assertEquals(read_return_ip(callee_ar), 2, "caller's ip");
  assertEquals(read_ret_reg(callee_ar), 1, "return value to r1");
  assertEquals(read_local(callee_ar, 1), int2oid(10), "r1 is saved");
  assertEquals(read_local(callee_ar, 2), pointer2oid(callee_cr), "r2 is saved");
  assertEquals(read_local(callee_ar, 3), int2oid(30), "r3 is saved");
  return 0;
}
    
static int test_callrec() {
  Instruction src[4] = {
    {MOVEI, 1, 0, 0, int2oid(1), NULL, NULL},
    {MOVEI, 2, 0, 0, int2oid(2), NULL, NULL},
    {MOVEI, 3, 0, 0, int2oid(3), NULL, NULL},
    {0xffff, 0, 0, 0, invalidOidValue, NULL, NULL}};
  Pointer caller_cr = assemble(invalidOidValue, invalidOidValue, src);
  Pointer caller_ar = alloc_ar0(invalidPointerValue, 1, 0, invalidPointerValue, 0);
  Pointer args = alloc(1);
  write_int(1, 10);
  write_int(2, 20);
  write_pointer(3, args);
  write_slot(args, 1, int2oid(1));
  write_cr(caller_cr);
  write_ip(2);
  write_ar(caller_ar);
  callrec(1, 3);
  Pointer callee_ar = read_ar();
  assertEquals(read_cr(), caller_cr, "cr is caller's");
  assertEquals(read_arg(callee_ar, 1), int2oid(1), "arg1 = 1");
  assertEquals(read_dynamic_link(callee_ar), caller_ar, "dynamic link is at caller's ar");
  assertEquals(read_return_cr(callee_ar), caller_cr, "caller's cr");
  assertEquals(read_return_ip(callee_ar), 2, "caller's ip");
  assertEquals(read_ret_reg(callee_ar), 1, "return value to r1");
  assertEquals(read_local(callee_ar, 1), int2oid(10), "r1 is saved");
  assertEquals(read_local(callee_ar, 2), int2oid(20), "r2 is saved");
  assertEquals(read_local(callee_ar, 3), pointer2oid(args), "r3 is saved");
  return 0;
}
    
static int test_callrec0() {
  Instruction src[4] = {
    {MOVEI, 1, 0, 0, int2oid(1), NULL, NULL},
    {MOVEI, 2, 0, 0, int2oid(2), NULL, NULL},
    {MOVEI, 3, 0, 0, int2oid(3), NULL, NULL},
    {0xffff, 0, 0, 0, invalidOidValue, NULL, NULL}};
  Pointer caller_cr = assemble(invalidOidValue, invalidOidValue, src);
  Pointer caller_ar = alloc_ar0(invalidPointerValue, 1, 0, invalidPointerValue, 0);
  write_int(1, 10);
  write_int(2, 20);
  write_int(3, 30);
  write_cr(caller_cr);
  write_ip(2);
  write_ar(caller_ar);
  callrec0(1);
  Pointer  callee_ar = read_ar();
  assertEquals(read_cr(), caller_cr, "cr is caller's");
  assertEquals(read_dynamic_link(callee_ar), caller_ar, "dynamic link is at caller's ar");
  assertEquals(read_return_cr(callee_ar), caller_cr, "caller's cr");
  assertEquals(read_return_ip(callee_ar), 2, "caller's ip");
  assertEquals(read_ret_reg(callee_ar), 1, "return value to r1");
  assertEquals(read_local(callee_ar, 1), int2oid(10), "r1 is saved");
  assertEquals(read_local(callee_ar, 2), int2oid(20), "r2 is saved");
  assertEquals(read_local(callee_ar, 3), int2oid(30), "r3 is saved");
  return 0;
}
    
static int test_callrec1() {
  Instruction src[4] = {
    {MOVEI, 1, 0, 0, int2oid(1), NULL, NULL},
    {MOVEI, 2, 0, 0, int2oid(2), NULL, NULL},
    {MOVEI, 3, 0, 0, int2oid(3), NULL, NULL},
    {0xffff, 0, 0, 0, invalidOidValue, NULL, NULL}};
  Pointer caller_cr = assemble(invalidOidValue, invalidOidValue, src);
  Pointer caller_ar = alloc_ar0(invalidPointerValue, 1, 0, invalidPointerValue, 0);
  write_int(1, 10);
  write_int(2, 20);
  write_int(3, 30);
  write_cr(caller_cr);
  write_ip(2);
  write_ar(caller_ar);
  callrec1(1, 2);
  Pointer callee_ar = read_ar();
  assertEquals(read_cr(), caller_cr, "cr is caller's");
  assertEquals(read_arg(callee_ar, 1), int2oid(20), "arg1 = 20");
  assertEquals(read_dynamic_link(callee_ar), caller_ar, "dynamic link is at caller's ar");
  assertEquals(read_return_cr(callee_ar), caller_cr, "caller's cr");
  assertEquals(read_return_ip(callee_ar), 2, "caller's ip");
  assertEquals(read_ret_reg(callee_ar), 1, "return value to r1");
  assertEquals(read_local(callee_ar, 1), int2oid(10), "r1 is saved");
  assertEquals(read_local(callee_ar, 2), int2oid(20), "r2 is saved");
  assertEquals(read_local(callee_ar, 3), int2oid(30), "r3 is saved");
  return 0;
}
    
static int test_callrec2() {
  Instruction src[4] = {
    {MOVEI, 1, 0, 0, int2oid(1), NULL, NULL},
    {MOVEI, 2, 0, 0, int2oid(2), NULL, NULL},
    {MOVEI, 3, 0, 0, int2oid(3), NULL, NULL},
    {0xffff, 0, 0, 0, invalidOidValue, NULL, NULL}};
  Pointer caller_cr = assemble(invalidOidValue, invalidOidValue, src);
  Pointer caller_ar = alloc_ar0(invalidPointerValue, 1, 0, invalidPointerValue, 0);
  write_int(1, 10);
  write_int(2, 20);
  write_int(3, 30);
  write_cr(caller_cr);
  write_ip(2);
  write_ar(caller_ar);
  callrec2(1, 2, 3);
  Pointer callee_ar = read_ar();
  assertEquals(read_cr(), caller_cr, "cr is caller's");
  assertEquals(read_arg(callee_ar, 1), int2oid(20), "arg1 = 20");
  assertEquals(read_arg(callee_ar, 2), int2oid(30), "arg2 = 30");
  assertEquals(read_dynamic_link(callee_ar), caller_ar, "dynamic link is at caller's ar");
  assertEquals(read_return_cr(callee_ar), caller_cr, "caller's cr");
  assertEquals(read_return_ip(callee_ar), 2, "caller's ip");
  assertEquals(read_ret_reg(callee_ar), 1, "return value to r1");
  assertEquals(read_local(callee_ar, 1), int2oid(10), "r1 is saved");
  assertEquals(read_local(callee_ar, 2), int2oid(20), "r2 is saved");
  assertEquals(read_local(callee_ar, 3), int2oid(30), "r3 is saved");
  return 0;
}
    
static int test_ret() {
  Instruction src[4] = {
    {MOVEI, 1, 0, 0, int2oid(1), NULL, NULL},
    {MOVEI, 2, 0, 0, int2oid(2), NULL, NULL},
    {MOVEI, 3, 0, 0, int2oid(3), NULL, NULL},
    {0xffff, 0, 0, 0, invalidOidValue, NULL, NULL}};
  Pointer caller_cr = assemble(invalidOidValue, invalidOidValue, src);
  Pointer caller_ar = alloc_ar0(invalidPointerValue, 1, 0, invalidPointerValue, 0);
  Pointer callee_cr = assemble(invalidOidValue, invalidOidValue, src);
  Pointer args = alloc(1);
  write_int(1, 10);
  write_pointer(2, callee_cr);
  write_pointer(3, args);
  write_slot(args, 1, int2oid(1));
  write_cr(caller_cr);
  write_ip(2);
  write_ar(caller_ar);
  call(1, 2, 3);
  Pointer callee_ar = read_ar();
  write_int(1, 0);
  write_int(2, 0);
  write_int(3, 100);
  write_ip(4);
  ret(3);
  assertEquals(read_int(1), 100, "r1 is the return value");
  assertEquals(read_pointer(2), callee_cr, "r2 is restored");
  assertEquals(read_pointer(3), args, "r3 is restored");
  assertEquals(read_cr(), caller_cr, "cr is the caller");
  assertEquals(read_ip(), 2, "ip is the caller");
  assertEquals(read_ar(), caller_ar, "ar is the caller");
  assertEquals(basic_read(callee_ar + SIZE_OFFSET), 0, "callee's ar is free");
  return 0;
}
    
static int test_rettrue() {
  Instruction src[4] = {
    {MOVEI, 1, 0, 0, int2oid(1), NULL, NULL},
    {MOVEI, 2, 0, 0, int2oid(2), NULL, NULL},
    {MOVEI, 3, 0, 0, int2oid(3), NULL, NULL},
    {0xffff, 0, 0, 0, invalidOidValue, NULL, NULL}};
  Pointer caller_cr = assemble(invalidOidValue, invalidOidValue, src);
  Pointer caller_ar = alloc_ar0(invalidPointerValue, 1, 0, invalidPointerValue, 0);
  Pointer callee_cr = assemble(invalidOidValue, invalidOidValue, src);
  Pointer args = alloc(1);
  write_int(1, 10);
  write_pointer(2, callee_cr);
  write_pointer(3, args);
  write_slot(args, 1, int2oid(1));
  write_cr(caller_cr);
  write_ip(2);
  write_ar(caller_ar);
  call(1, 2, 3);
  Pointer callee_ar = read_ar();
  write_int(1, 0);
  write_oid(2, trueValue);
  write_oid(3, falseValue);
  write_ip(4);
  rettrue(3, 2);
  assertEquals(read_ip(), 4, "not returned");
  rettrue(2, 3);
  assertEquals(read_oid(1), falseValue, "r1 is the return value");
  assertEquals(read_pointer(2), callee_cr, "r2 is restored");
  assertEquals(read_pointer(3), args, "r3 is restored");
  assertEquals(read_cr(), caller_cr, "cr is the caller");
  assertEquals(read_ip(), 2, "ip is the caller");
  assertEquals(read_ar(), caller_ar, "ar is the caller");
  assertEquals(basic_read(callee_ar + SIZE_OFFSET), 0, "callee's ar is free");
  return 0;
}
    
static int test_retfalse() {
  Instruction src[4] = {
    {MOVEI, 1, 0, 0, int2oid(1), NULL, NULL},
    {MOVEI, 2, 0, 0, int2oid(2), NULL, NULL},
    {MOVEI, 3, 0, 0, int2oid(3), NULL, NULL},
    {0xffff, 0, 0, 0, invalidOidValue, NULL, NULL}};
  Pointer caller_cr = assemble(invalidOidValue, invalidOidValue, src);
  Pointer caller_ar = alloc_ar0(invalidPointerValue, 1, 0, invalidPointerValue, 0);
  Pointer callee_cr = assemble(invalidOidValue, invalidOidValue, src);
  Pointer args = alloc(1);
  write_int(1, 10);
  write_pointer(2, callee_cr);
  write_pointer(3, args);
  write_slot(args, 1, int2oid(1));
  write_cr(caller_cr);
  write_ip(2);
  write_ar(caller_ar);
  call(1, 2, 3);
  Pointer callee_ar = read_ar();
  write_int(1, 0);
  write_oid(2, trueValue);
  write_oid(3, falseValue);
  write_ip(4);
  retfalse(2, 3);
  assertEquals(read_ip(), 4, "not returned");
  retfalse(3, 2);
  assertEquals(read_oid(1), trueValue, "r1 is the return value");
  assertEquals(read_pointer(2), callee_cr, "r2 is restored");
  assertEquals(read_pointer(3), args, "r3 is restored");
  assertEquals(read_cr(), caller_cr, "cr is the caller");
  assertEquals(read_ip(), 2, "ip is the caller");
  assertEquals(read_ar(), caller_ar, "ar is the caller");
  assertEquals(basic_read(callee_ar + SIZE_OFFSET), 0, "callee's ar is free");
  return 0;
}
    
static int test_step() {
  Instruction src[10] = {
    {MOVEI, 1, 0, 0, int2oid(0), NULL, NULL},
    {MOVEI, 3, 0, 0, int2oid(10), NULL, NULL},
    {MOVEI, 2, 4, 0, int2oid(1), NULL, NULL},
    {MOVEI, 5, 0, 0, invalidOidValue, "loop", NULL},
    {ADD, 1, 1, 2, invalidOidValue, NULL, "loop"},
    {ADD, 2, 2, 4, invalidOidValue, NULL, NULL},
    {LESSTHAN, 6, 3, 2, invalidOidValue, NULL, NULL},
    {JUMPFALSE, 6, 5, 0, invalidOidValue, NULL, NULL},
    {ERR, 0, 0, 0, invalidOidValue, NULL, NULL},
    {0xffff, 0, 0, 0, invalidOidValue, NULL, NULL}};
  Pointer cr = assemble(invalidOidValue, invalidOidValue, src);
  write_cr(cr);
  write_ip(0);
  while (read_ip() < 12) {
    step();
  }
  assertEquals(read_int(2), 11, "r2 = 11 (just over the stop value=10)");
  assertEquals(read_int(1), 55, "r1 = 55 (sum of 0,...,10)");
  return 0;
}
