#ifndef _INSTRUCTION_H_
#define _INSTRUCTION_H_

#include <stdio.h>
#include <stdlib.h>
#include "data.h"
#include "memory.h"
#include "register.h"
#include "activationRecord.h"
#include "codeRecord.h"

#define ERR        0x0000
#define NOP        0x0001
#define ALLOC      0x0002
#define RESET      0x0010
#define MOVE       0x0011
#define MOVEI      0x0012
#define LOAD       0x0020
#define LOAD1      0x0021
#define LOAD2      0x0022
#define LOAD3      0x0023
#define LOAD4      0x0024
#define LOAD5      0x0025
#define LOAD6      0x0026
#define LOAD7      0x0027
#define STORE      0x0028
#define STORE1     0x0029
#define STORE2     0x002a
#define STORE3     0x002b
#define STORE4     0x002c
#define STORE5     0x002d
#define STORE6     0x002e
#define STORE7     0x002f
#define ADD        0x0030
#define SUB        0x0031
#define MUL        0x0032
#define IDIV       0x0033
#define IMOD       0x0034
#define EQUAL      0x0050
#define LESSTHAN   0x0051
#define NOT        0x0060
#define JUMP       0x0070
#define JUMPTRUE   0x0071
#define JUMPFALSE  0x0072
#define CALL       0x0080
#define CALLREC    0x0083
#define RET        0x0088
#define RETTRUE    0x0089
#define RETFALSE   0x008a

static inline void err(char *msg) {
  fprintf(stderr, "Fatal: %s\n", msg);
  exit(1);
}
    
static inline OID fetch() {
  /*
  Dword ip = read_ip();
  Pointer cr = read_cr();
  write_ip(ip + 1);
  if (ip < read_code_size(cr)) {
    return read_code(cr, ip);
  } else {
    err("fetch error: ip out of bounds");
    return invalidOidValue;
  }
  */
  return basic_read(codeRecord + CONTENT_OFFSET + (IP_SLOT_OFFSET - 1 + instructionPointer++)*8);
}

extern void nop();
extern void allocate(Register reg1, Register reg2);
extern void reset(Register dst);
extern void mov(Register dst, Register src);
extern void movei(Register dst, OID imm);
extern void loadi(Register dst, Register src, Register intReg, int offset);  
extern void load(Register dst, Register src, Register intReg);
extern void load1(Register dst, Register src, Register intReg);
extern void load2(Register dst, Register src, Register intReg);
extern void load3(Register dst, Register src, Register intReg);
extern void load4(Register dst, Register src, Register intReg);
extern void load5(Register dst, Register src, Register intReg);
extern void load6(Register dst, Register src, Register intReg);
extern void load7(Register dst, Register src, Register intReg);
extern void storei(Register dst, Register intReg, Register src, int offset);
extern void store(Register dst, Register intReg, Register src);
extern void store1(Register dst, Register intReg, Register src);
extern void store2(Register dst, Register intReg, Register src);
extern void store3(Register dst, Register intReg, Register src);
extern void store4(Register dst, Register intReg, Register src);
extern void store5(Register dst, Register intReg, Register src);
extern void store6(Register dst, Register intReg, Register src);
extern void store7(Register dst, Register intReg, Register src);
extern void add(Register dst, Register src1, Register src2);
extern void sub(Register dst, Register src1, Register src2);
extern void mul(Register dst, Register src1, Register src2);
extern void fdiv(Register dst, Register src1, Register src2);
extern void idiv(Register dst, Register src1, Register src2);
extern void imod(Register dst, Register src1, Register src2);
extern void lessthan(Register dst, Register src1, Register src2);
extern void equal(Register dst, Register src1, Register src2);
extern void bnot(Register dst, Register src);
extern void jump(Register offset);
extern void jumptrue(Register boolReg, Register offset);
extern void jumpfalse(Register boolReg, Register offset);
extern void call(Register dst, Register crPointerReg);
extern void callrec(Register dst);
extern void ret(Register src);    
extern void rettrue(Register boolReg, Register src);
extern void retfalse(Register boolReg, Register src);

#define reg1(code) ((code >> 33) & 0xffff)
#define reg2(code) ((code >> 17) & 0xffff)
#define reg3(code) (code>>1 & 0xffff)

static inline void step() {
  //  Qword code = (Qword)oid2int(fetch());
  Qword code = (Qword)fetch();
  int opcode = (code >> 49) & 0x7fff;
  switch (opcode) {
  case ERR:
    err("error instruction invoked");
    return;
  case NOP:
    nop();
    return;
  case ALLOC:
    allocate(reg1(code), reg2(code));
    return;
  case RESET:
    reset(reg1(code));
    return;
  case MOVE:
    mov(reg1(code), reg2(code));
    return;
  case MOVEI:
    movei(reg1(code), fetch());
    return;
  case LOAD:
    load(reg1(code), reg2(code), reg3(code));
    return;
  case LOAD1:
    load1(reg1(code), reg2(code), reg3(code));
    return;
  case LOAD2:
    load2(reg1(code), reg2(code), reg3(code));
    return;
  case LOAD3:
    load3(reg1(code), reg2(code), reg3(code));
    return;
  case LOAD4:
    load4(reg1(code), reg2(code), reg3(code));
    return;
  case LOAD5:
    load5(reg1(code), reg2(code), reg3(code));
    return;
  case LOAD6:
    load6(reg1(code), reg2(code), reg3(code));
    return;
  case LOAD7:
    load7(reg1(code), reg2(code), reg3(code));
    return;
  case STORE:
    store(reg1(code), reg2(code), reg3(code));
    return;
  case STORE1:
    store1(reg1(code), reg2(code), reg3(code));
    return;
  case STORE2:
    store2(reg1(code), reg2(code), reg3(code));
    return;
  case STORE3:
    store3(reg1(code), reg2(code), reg3(code));
    return;
  case STORE4:
    store4(reg1(code), reg2(code), reg3(code));
    return;
  case STORE5:
    store5(reg1(code), reg2(code), reg3(code));
    return;
  case STORE6:
    store6(reg1(code), reg2(code), reg3(code));
    return;
  case STORE7:
    store7(reg1(code), reg2(code), reg3(code));
    return;
  case ADD:
    add(reg1(code), reg2(code), reg3(code));
    return;
  case SUB:
    sub(reg1(code), reg2(code), reg3(code));
    return;
  case MUL:
    mul(reg1(code), reg2(code), reg3(code));
    return;
  case IDIV:
    idiv(reg1(code), reg2(code), reg3(code));
    return;
  case IMOD:
    imod(reg1(code), reg2(code), reg3(code));
    return;
  case LESSTHAN:
    lessthan(reg1(code), reg2(code), reg3(code));
    return;
  case EQUAL:
    equal(reg1(code), reg2(code), reg3(code));
    return;
  case NOT:
    bnot(reg1(code), reg2(code));
    return;
  case JUMP:
    jump(reg1(code));
    return;
  case JUMPTRUE:
    jumptrue(reg1(code), reg2(code));
    return;
  case JUMPFALSE:
    jumpfalse(reg1(code), reg2(code));
    return;
  case CALL:
    call(reg1(code), reg2(code));
    return;
  case CALLREC:
    callrec(reg1(code));
    return;
  case RET:
    ret(reg1(code));
    return;
  case RETTRUE:
    rettrue(reg1(code), reg2(code));
    return;
  case RETFALSE:
    retfalse(reg1(code), reg2(code));
    return;
  default:
    err("decode error: unknown instruction");
  }

}

#endif
