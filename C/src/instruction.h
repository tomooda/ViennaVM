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

static inline void movei(Register dst, OID imm) {
  if (dst) {
    write_oid(dst, imm);
  } else {
    err("movei instruction error: operand not specified
");
  }
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
    
static inline void call(Register dst, Register crPointerReg) {
  if (crPointerReg) {
    Pointer cr = read_pointer(crPointerReg);
    if (cr != invalidPointerValue) {
      Int num_regs = read_num_regs(cr);
      Pointer ar =alloc_ar(read_ar(), dst, read_ip(), read_cr(), num_regs);
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

static inline void callrec(Register dst) {
  Pointer cr = read_cr();
  Int num_regs = read_num_regs(cr);
  Pointer ar = alloc_ar(read_ar(), dst, read_ip(), cr, num_regs);
  write_ar(ar);
  write_ip(0);
  for (int i = 1; i <= num_regs; i++) {
    write_local(ar, i, read_oid(i));
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

#define reg1(code) ((code >> 32) & 0xffff)
#define reg2(code) ((code >> 16) & 0xffff)
#define reg3(code) (code & 0xffff)

static inline void step() {
  Qword code = (Qword)oid2int(fetch());
  if (code != invalidIntValue) {
    int opcode = (code >> 48) & 0x7fff;
    switch (opcode) {
    case ERR:
      err("error instruction invoked");
      return;
    case NOP:
      nop();
      return;
    case ALLOC:
      allocate(reg1(code), reg2(code), reg3(code), fetch());
      return;
    case RESET:
      reset(reg1(code), reg2(code), reg3(code));
      return;
    case MOVE:
      mov(reg1(code), reg2(code), reg3(code));
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

  } else {
    err("decoder error: instruction is not integer");
  }
}

#endif
