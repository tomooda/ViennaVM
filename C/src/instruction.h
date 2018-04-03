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
#define ARGS       0x0018
#define ARG1       0x0019
#define ARG2       0x001a
#define ARG3       0x001b
#define ARG4       0x001c
#define ARG5       0x001d
#define ARG6       0x001e
#define ARG7       0x001f
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
#define CALL0      0x0081
#define CALL1      0x0082
#define CALLREC    0x0083
#define CALLREC0   0x0084
#define CALLREC1   0x0085
#define CALLREC2   0x0086
#define RET        0x0088
#define RETTRUE    0x0089
#define RETFALSE   0x008a

static inline void err(char *msg) {
  fprintf(stderr, "Fatal: %s\n", msg);
  exit(1);
}
    
static inline OID fetch() {
  Dword ip = read_ip();
  Pointer cr = read_cr();
  write_ip(ip + 1);
  if (ip < read_code_size(cr)) {
    return read_code(cr, ip);
  } else {
    err("fetch error: ip out of bounds");
    return invalidOidValue;
  }
}

static inline void nop() {
}

static inline void allocate(Register reg1, Register reg2, Register reg3, OID imm) {
  Int size = oid2int(imm);
  if (size != invalidIntValue) {
    Pointer p = alloc(size);
    if (reg1) {
      write_pointer(reg1, p);
    }
    if (reg2) {
      write_pointer(reg2, p);
    }
    if (reg3) {
      write_pointer(reg3, p);
    }
  } else {
    err("alloc instruction error: size is not integer");
  }
}

static inline void reset(Register reg1, Register reg2, Register reg3) {
  if (reg1) {
    write_oid(reg1, invalidOidValue);
  }
  if (reg2) {
    write_oid(reg2, invalidOidValue);
  }
  if (reg3) {
    write_oid(reg3, invalidOidValue);
  }
}
    
static inline void mov(Register dst1, Register dst2, Register src) {
  if (src) {
    if (dst1) {
      move(dst1, src);
    }
    if (dst2) {
      move(dst2, src);
    }
  } else {
    err("move instruction error: src register not specified");
  }
}

static inline void movei(Register dst1, Register dst2, Register dst3, OID imm) {
  if (imm != invalidOidValue) {
    if (dst1) {
      write_oid(dst1, imm);
    }
    if (dst2) {
      write_oid(dst2, imm);
    }
    if (dst3) {
      write_oid(dst3, imm);
    }
  } else {
    err("movei instruction error: invalid immediate value");
  }
}
    
static inline void args(Register dst1, Register dst2, Register dst3) {
  Pointer args = read_args(read_ar());
  if (dst1) {
    write_pointer(dst1, args);
  }
  if (dst2) {
    write_pointer(dst2, args);
  }
  if (dst3) {
    write_pointer(dst3, args);
  }
}
    
static inline void argi(Register dst1, Register dst2, Register dst3, int i) {
  OID arg = read_arg(read_ar(), i);
  if (dst1){
    write_oid(dst1, arg);
  }
  if (dst2) {
    write_oid(dst2, arg);
  }
  if (dst3) {
    write_oid(dst3, arg);
  }
}

static inline void arg1(Register dst1, Register dst2, Register dst3) {
  argi(dst1, dst2, dst3, 1);
}

static inline void arg2(Register dst1, Register dst2, Register dst3) {
  argi(dst1, dst2, dst3, 2);
}

static inline void arg3(Register dst1, Register dst2, Register dst3) {
  argi(dst1, dst2, dst3, 3);
}

static inline void arg4(Register dst1, Register dst2, Register dst3) {
  argi(dst1, dst2, dst3, 4);
}

static inline void arg5(Register dst1, Register dst2, Register dst3) {
  argi(dst1, dst2, dst3, 5);
}

static inline void arg6(Register dst1, Register dst2, Register dst3) {
  argi(dst1, dst2, dst3, 6);
}

static inline void arg7(Register dst1, Register dst2, Register dst3) {
  argi(dst1, dst2, dst3, 7);
}

static inline void loadi(Register dst, Register src, Register intReg, int offset) {
  if (dst && src) {
    Pointer pointer = read_pointer(src);
    Int slot = (intReg ? read_int(intReg) : 0) + offset;
    if (slots_size(pointer) >= slot) {
      write_oid(dst, read_slot(pointer, slot));
    } else {
      err("load instruction error: index out of bounds");
    }
  } else {
    err("load instruction error: operands not specified");
  }
}
    
static inline void load(Register dst, Register src, Register intReg) {
  loadi(dst, src, intReg, 0);
}
    
static inline void load1(Register dst, Register src, Register intReg) {
  loadi(dst, src, intReg, 1);
}
    
static inline void load2(Register dst, Register src, Register intReg) {
  loadi(dst, src, intReg, 2);
}
    
static inline void load3(Register dst, Register src, Register intReg) {
  loadi(dst, src, intReg, 3);
}
    
static inline void load4(Register dst, Register src, Register intReg) {
  loadi(dst, src, intReg, 4);
}
    
static inline void load5(Register dst, Register src, Register intReg) {
  loadi(dst, src, intReg, 5);
}
    
static inline void load6(Register dst, Register src, Register intReg) {
  loadi(dst, src, intReg, 6);
}
    
static inline void load7(Register dst, Register src, Register intReg) {
  loadi(dst, src, intReg, 7);
}
    
static inline void storei(Register dst, Register intReg, Register src, int offset) {
  if (dst && src) {
    Pointer pointer = read_pointer(dst);
    Int slot = (intReg ? read_int(intReg) : 0) + offset;
    if (slots_size(pointer) >= slot) {
      write_slot(pointer, slot, read_oid(src));
    } else {
      err("store instruction error: index out of bounds");
    }
  } else {
    err("store instruction error: operands not specified");
  }
}

static inline void store(Register dst, Register intReg, Register src) {
  storei(dst, intReg, src, 0);
}
    
static inline void store1(Register dst, Register intReg, Register src) {
  storei(dst, intReg, src, 1);
}
    
static inline void store2(Register dst, Register intReg, Register src) {
  storei(dst, intReg, src, 2);
}
    
static inline void store3(Register dst, Register intReg, Register src) {
  storei(dst, intReg, src, 3);
}
    
static inline void store4(Register dst, Register intReg, Register src) {
  storei(dst, intReg, src, 4);
}
    
static inline void store5(Register dst, Register intReg, Register src) {
  storei(dst, intReg, src, 5);
}
    
static inline void store6(Register dst, Register intReg, Register src) {
  storei(dst, intReg, src, 6);
}
    
static inline void store7(Register dst, Register intReg, Register src) {
  storei(dst, intReg, src, 7);
}
    
static inline void add(Register dst, Register src1, Register src2) {
  if (dst && src1 && src2) {
    Int int1 = read_int(src1);
    Int int2 = read_int(src2);
    if (int1 != invalidIntValue && int2 != invalidIntValue) {
      write_int(dst, int1+int2);
      return;
    }
    Float float1 = read_float(src1);
    Float float2 = read_float(src2);
    if (float1 != invalidFloatValue && float2 != invalidFloatValue) {
      write_float(dst, real2float(float2real(float1)+float2real(float2)));
      return;
    }
    err("add instruction error: arithmetic error");
  } else {
    err("add instruction error: operands not specified");
  }
}
    
static inline void sub(Register dst, Register src1, Register src2) {
  if (dst && src1 && src2) {
    Int int1 = read_int(src1);
    Int int2 = read_int(src2);
    if (int1 != invalidIntValue && int2 != invalidIntValue) {
      write_int(dst, int1-int2);
      return;
    }
    Float float1 = read_float(src1);
    Float float2 = read_float(src2);
    if (float1 != invalidFloatValue && float2 != invalidFloatValue) {
      write_float(dst, real2float(float2real(float1)-float2real(float2)));
      return;
    }
    err("sub instruction error: arithmetic error");
  } else {
    err("sub instruction error: operands not specified");
  }
}
    
static inline void mul(Register dst, Register src1, Register src2) {
  if (dst && src1 && src2) {
    Int int1 = read_int(src1);
    Int int2 = read_int(src2);
    if (int1 != invalidIntValue && int2 != invalidIntValue) {
      write_int(dst, int1*int2);
      return;
    }
    Float float1 = read_float(src1);
    Float float2 = read_float(src2);
    if (float1 != invalidFloatValue && float2 != invalidFloatValue) {
      write_float(dst, real2float(float2real(float1)*float2real(float2)));
      return;
    }
    err("mul instruction error: arithmetic error");
  } else {
    err("mul instruction error: operands not specified");
  }
}
    
static inline void fdiv(Register dst, Register src1, Register src2) {
  if (dst && src1 && src2) {
    Float float1 = read_float(src1);
    Float float2 = read_float(src2);
    if (float1 != invalidFloatValue && float2 != invalidFloatValue) {
      write_float(dst, real2float(float2real(float1)/float2real(float2)));
      return;
    }
    err("fdiv instruction error: arithmetic error");
  } else {
    err("fdiv instruction error: operands not specified");
  }
}

static inline void idiv(Register dst, Register src1, Register src2) {
  if (dst && src1 && src2) {
    Int int1 = read_int(src1);
    Int int2 = read_int(src2);
    if (int1 != invalidIntValue && int2 != invalidIntValue) {
      write_int(dst, int1/int2);
      return;
    }
    err("idiv instruction error: arithmetic error");
  } else {
    err("idiv instruction error: operands not specified");
  }
}
    
static inline void imod(Register dst, Register src1, Register src2) {
  if (dst && src1 && src2) {
    Int int1 = read_int(src1);
    Int int2 = read_int(src2);
    if (int1 != invalidIntValue && int2 != invalidIntValue) {
      write_int(dst, int1 * int2 < 0 ? int1 % int2 + int2 : int1 % int2);
      return;
    }
    err("imod instruction error: arithmetic error");
  } else {
    err("imod instruction error: operands not specified");
  }
}
    
static inline void lessthan(Register dst, Register src1, Register src2) {
  if (dst && src1 && src2) {
    Int int1 = read_int(src1);
    Int int2 = read_int(src2);
    if (int1 != invalidIntValue && int2 != invalidIntValue) {
      if (int1 < int2) {
	write_oid(dst, trueValue);
      } else {
	write_oid(dst, falseValue);
      }
      return;
    }
    Float float1 = read_float(src1);
    Float float2 = read_float(src2);
    if (float1 != invalidFloatValue && float2 != invalidFloatValue) {
      if (float1 < float2) {
	write_oid(dst, trueValue);
      } else {
	write_oid(dst, falseValue);
      }
      return;
    }
    err("lessthan instruction error: arithmetic error");
  } else {
    err("lessthan instruction error: operands not specified");
  }
}

static inline void equal(Register dst, Register src1, Register src2) {
  if (dst && src1 && src2) {
    OID oid1 = read_oid(src1);
    OID oid2 = read_oid(src2);
    if (oid1 == oid2) {
      write_oid(dst, trueValue);
    } else {
      write_oid(dst, falseValue);
    }
  } else {
    err("lessthan instruction error: operands not specified");
  }
}

static inline void bnot(Register dst, Register src) {
  if (dst && src) {
    switch (read_oid(src)) {
    case trueValue:
      write_oid(dst, falseValue);
      return;
    case falseValue:
      write_oid(dst, trueValue);
      return;
    default:
      err("not instruction error: 2nd operand not boolean");
      return;
    }
  } else {
    err("not instruction error: operands not specified");
  }
}
    
static inline void jump(Register intReg) {
  if (intReg) {
    Int ip = read_int(intReg);
    if (ip != invalidIntValue) {
      write_ip(ip);
    } else {
      err("jump instruction error: operand not integer");
    }
  } else {
    err("jump instruction error: register not specified");
  }
}
    
static inline void jumptrue(Register boolReg, Register intReg) {
  if (boolReg && intReg) {
    Int ip = read_int(intReg);
    if (ip != invalidIntValue) {
      switch (read_oid(boolReg)) {
      case trueValue:
	write_ip(ip);
	return;
      case falseValue:
	return;
      default:
	err("jumptrue instruction error: operand not boolean");
	return;
      }
    } else {
      err("jumptrue instruction error: operand not integer");
    }
  } else {
    err("jumptrue instruction error: operands not specified");
  }
}
    
static inline void jumpfalse(Register boolReg, Register intReg) {
  if (boolReg && intReg) {
    Int ip = read_int(intReg);
    if (ip != invalidIntValue) {
      switch (read_oid(boolReg)) {
      case falseValue:
	write_ip(ip);
	return;
      case trueValue:
	return;
      default:
	err("jumpfalse instruction error: operand not boolean");
	return;
      }
    } else {
      err("jumpfalse instruction error: operand not integer");
    }
  } else {
    err("jumpfalse instruction error: operands not specified");
  }
}
    
static inline void call(Register dst, Register crPointerReg, Register argsPointerReg) {
  if (crPointerReg) {
    Pointer args = argsPointerReg ? read_pointer(argsPointerReg) : invalidPointerValue;
    Pointer cr = read_pointer(crPointerReg);
    if (cr != invalidPointerValue) {
      Int num_regs = read_num_regs(cr);
      Pointer ar =alloc_ar(read_ar(), args, dst, read_ip(), read_cr(), num_regs);
      write_ar(ar);
      write_cr(cr);
      for (int i = 1; i <=  num_regs; i++) {
	write_local(ar, i, read_oid(i));
      }
    } else {
      err("call instruction error: second operand not pointer");
    }
  } else {
    err("call instruction error: operands not specified");
  }
}    

static inline void call0(Register dst, Register crPointerReg) {
  if (crPointerReg) {
    Pointer cr = read_pointer(crPointerReg);
    if (cr != invalidPointerValue) {
      Int num_regs = read_num_regs(cr);
      Pointer ar = alloc_ar0(read_ar(), dst, read_ip(), read_cr(), num_regs);
      write_ar(ar);
      write_cr(cr);
      for (int i = 1; i <=  num_regs; i++) {
	write_local(ar, i, read_oid(i));
      }
    } else {
      err("call0 instruction error: second operand not pointer");
    }
  } else {
    err("call0 instruction error: operands not specified");
  }
}    
    
static inline void call1(Register dst, Register crPointerReg, Register argReg) {
  if (crPointerReg) {
    Pointer cr = read_pointer(crPointerReg);
    if (cr != invalidPointerValue) {
      Int num_regs = read_num_regs(cr);
      Pointer ar = alloc_ar1(read_ar(), read_oid(argReg), dst, read_ip(), read_cr(), num_regs);
      write_ar(ar);
      write_cr(cr);
      for (int i = 1; i <=  num_regs; i++) {
	write_local(ar, i, read_oid(i));
      }
    } else {
      err("call1 instruction error: second operand not pointer");
    }
  } else {
    err("call1 instruction error: operands not specified");
  }
}    
    
static inline void callrec(Register dst, Register argsPointerReg) {
  if (argsPointerReg) {
    Pointer args = read_pointer(argsPointerReg);
    Pointer cr = read_cr();
    Int num_regs = read_num_regs(cr);
    Pointer ar = alloc_ar(read_ar(), args, dst, read_ip(), cr, num_regs);
    write_ar(ar);
    write_ip(0);
    for (int i = 1; i <= num_regs; i++) {
      write_local(ar, i, read_oid(i));
    }
  } else {
    err("callrec instruction error: operands not specified");
  }
}
    
static inline void callrec0(Register dst) {
  Pointer cr = read_cr();
  Int num_regs = read_num_regs(cr);
  Pointer ar =alloc_ar0(read_ar(), dst, read_ip(), cr, num_regs);
  write_ar(ar);
  write_ip(0);
  for (int i = 1; i <= num_regs; i++) {
    write_local(ar, i, read_oid(i));
  }
}
    
static inline void callrec1(Register dst, Register arg1) {
  if (arg1) {
    Pointer args = alloc(1);
    Pointer cr = read_cr();
    Int num_regs = read_num_regs(cr);
    Pointer ar = alloc_ar(read_ar(), args, dst, read_ip(), cr, num_regs);
    write_slot(args, 1, read_oid(arg1));
    write_ar(ar);
    write_ip(0);
    for (int i = 1; i <= num_regs; i++) {
      write_local(ar, i, read_oid(i));
    }
  } else {
    err("callrec1 instruction error: second operand not specified");
  }
}
    
static inline void callrec2(Register dst, Register arg1, Register arg2) {
  if (arg1 && arg2) {
    Pointer args = alloc(2);
    write_slot(args, 1, read_oid(arg1));
    write_slot(args, 2, read_oid(arg2));
    Pointer cr = read_cr();
    Int num_regs = read_num_regs(cr);
    Pointer ar = alloc_ar(read_ar(), args, dst, read_ip(), cr, num_regs);
    write_ar(ar);
    write_ip(0);
    for (int i = 1; i <= num_regs; i++) {
      write_local(ar, i, read_oid(i));
    }
  } else {
    err("callrec2 instruction error: second/third operand not specified");
  }
}
    
static inline void ret(Register src) {
  Pointer ar = read_ar();
  Pointer cr = read_cr();
  if (ar != invalidPointerValue && cr != invalidPointerValue) {
    Pointer ret_ar = read_dynamic_link(ar);
    Pointer ret_cr = read_return_cr(ar);
    Dword ret_ip = read_return_ip(ar);
    Register ret_reg = read_ret_reg(ar);
    Int num_regs = read_num_regs(cr);
    if (src && ret_reg && src != ret_reg) {
      move(ret_reg, src);
    }
    for (int i = 1; i <= num_regs; i++) {
      if (i != ret_reg) {
	write_oid(i, read_local(ar, i));
      }
    }
    write_cr(ret_cr);
    write_ip(ret_ip);
    write_ar(ret_ar);
  } else {
    err("ret instruction error: no cr/ar");
  }
}
    
static inline void rettrue(Register boolReg, Register src) {
  Pointer ar;
  Pointer cr;
  if (boolReg) {
    switch (read_oid(boolReg)) {
    case trueValue:
      ar = read_ar();
      cr = read_cr();
      if (ar != invalidPointerValue && cr != invalidPointerValue) {
	Pointer ret_ar = read_dynamic_link(ar);
	Pointer ret_cr = read_return_cr(ar);
	Dword ret_ip = read_return_ip(ar);
	Register ret_reg = read_ret_reg(ar);
	Int num_regs = read_num_regs(cr);
	if (src && ret_reg && src != ret_reg) {
	  move(ret_reg, src);
	}
	for (int i = 1; i <= num_regs; i++) {
	  if (i != ret_reg) {
	    write_oid(i, read_local(ar, i));
	  }
	}
	write_cr(ret_cr);
	write_ip(ret_ip);
	write_ar(ret_ar);
      } else {
	err("rettrue instruction error: no cr/ar");
      }
      return;
    case falseValue:
      return;
    default:
      err("rettrue instruction error: operand1 not boolean");
    }
  } else {
    err("rettrue instruction error: operand1 not specified");
  }
}
    
static inline void retfalse(Register boolReg, Register src) {
  Pointer ar;
  Pointer cr;
  if (boolReg) {
    switch (read_oid(boolReg)) {
    case falseValue:
      ar = read_ar();
      cr = read_cr();
      if (ar != invalidPointerValue && cr != invalidPointerValue) {
	Pointer ret_ar = read_dynamic_link(ar);
	Pointer ret_cr = read_return_cr(ar);
	Dword ret_ip = read_return_ip(ar);
	Register ret_reg = read_ret_reg(ar);
	Int num_regs = read_num_regs(cr);
	if (src && ret_reg && src != ret_reg) {
	  move(ret_reg, src);
	}
	for (int i = 1; i <= num_regs; i++) {
	  if (i != ret_reg) {
	    write_oid(i, read_local(ar, i));
	  }
	}
	write_cr(ret_cr);
	write_ip(ret_ip);
	write_ar(ret_ar);
      } else {
	err("retfalse instruction error: no cr/ar");
      }
      return;
    case trueValue:
      return;
    default:
      err("retfalse instruction error: operand1 not boolean");
    }
  } else {
    err("retfalse instruction error: operand1 not specified");
  }
}

static inline void step() {
  Qword code = (Qword)oid2int(fetch());
  if (code != invalidIntValue) {
    int opcode = (code >> 48) & 0x7fff;
    int reg1 = (code >> 32) & 0xffff;
    int reg2 = (code >> 16) & 0xffff;
    int reg3 = code & 0xffff;
    switch (opcode) {
    case ERR:
      err("error instruction invoked");
      return;
    case NOP:
      nop();
      return;
    case ALLOC:
      allocate(reg1, reg2, reg3, fetch());
      return;
    case RESET:
      reset(reg1, reg2, reg3);
      return;
    case MOVE:
      mov(reg1, reg2, reg3);
      return;
    case MOVEI:
      movei(reg1, reg2, reg3, fetch());
      return;
    case ARGS:
      args(reg1, reg2, reg3);
      return;
    case ARG1:
      arg1(reg1, reg2, reg3);
      return;
    case ARG2:
      arg2(reg1, reg2, reg3);
      return;
    case ARG3:
      arg3(reg1, reg2, reg3);
      return;
    case ARG4:
      arg4(reg1, reg2, reg3);
      return;
    case ARG5:
      arg5(reg1, reg2, reg3);
      return;
    case ARG6:
      arg6(reg1, reg2, reg3);
      return;
    case ARG7:
      arg7(reg1, reg2, reg3);
      return;
    case LOAD:
      load(reg1, reg2, reg3);
      return;
    case LOAD1:
      load1(reg1, reg2, reg3);
      return;
    case LOAD2:
      load2(reg1, reg2, reg3);
      return;
    case LOAD3:
      load3(reg1, reg2, reg3);
      return;
    case LOAD4:
      load4(reg1, reg2, reg3);
      return;
    case LOAD5:
      load5(reg1, reg2, reg3);
      return;
    case LOAD6:
      load6(reg1, reg2, reg3);
      return;
    case LOAD7:
      load7(reg1, reg2, reg3);
      return;
    case STORE:
      store(reg1, reg2, reg3);
      return;
    case STORE1:
      store1(reg1, reg2, reg3);
      return;
    case STORE2:
      store2(reg1, reg2, reg3);
      return;
    case STORE3:
      store3(reg1, reg2, reg3);
      return;
    case STORE4:
      store4(reg1, reg2, reg3);
      return;
    case STORE5:
      store5(reg1, reg2, reg3);
      return;
    case STORE6:
      store6(reg1, reg2, reg3);
      return;
    case STORE7:
      store7(reg1, reg2, reg3);
      return;
    case ADD:
      add(reg1, reg2, reg3);
      return;
    case SUB:
      sub(reg1, reg2, reg3);
      return;
    case MUL:
      mul(reg1, reg2, reg3);
      return;
    case IDIV:
      idiv(reg1, reg2, reg3);
      return;
    case IMOD:
      imod(reg1, reg2, reg3);
      return;
    case LESSTHAN:
      lessthan(reg1, reg2, reg3);
      return;
    case EQUAL:
      equal(reg1, reg2, reg3);
      return;
    case NOT:
      bnot(reg1, reg2);
      return;
    case JUMP:
      jump(reg1);
      return;
    case JUMPTRUE:
      jumptrue(reg1, reg2);
      return;
    case JUMPFALSE:
      jumpfalse(reg1, reg2);
      return;
    case CALL:
      call(reg1, reg2, reg3);
      return;
    case CALL0:
      call0(reg1, reg2);
      return;
    case CALL1:
      call1(reg1, reg2, reg3);
      return;
    case CALLREC:
      callrec(reg1, reg2);
      return;
    case CALLREC0:
      callrec0(reg1);
      return;
    case CALLREC1:
      callrec1(reg1, reg2);
      return;
    case CALLREC2:
      callrec2(reg1, reg2, reg3);
      return;
    case RET:
      ret(reg1);
      return;
    case RETTRUE:
      rettrue(reg1, reg2);
      return;
    case RETFALSE:
      retfalse(reg1, reg2);
      return;
    default:
      err("decode error: unknown instruction");
    }

  } else {
    err("decoder error: instruction is not integer");
  }
}

#endif
