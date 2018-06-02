#include "instruction.h"

void nop() {
}

void allocate(Register reg1, Register reg2) {
  Int size = read_int(reg2);
  if (size != invalidIntValue) {
    Pointer p = alloc(size);
    write_pointer(reg1, p);
  } else {
    err("alloc instruction error: size is not integer");
  }
}

void reset(Register reg1, Register reg2, Register reg3) {
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
    
void mov(Register dst, Register src) {
  move(dst, src);
}

void movei(Register dst, OID imm) {
  write_oid(dst, imm);
}
    
void loadi(Register dst, Register src, Register intReg, int offset) {
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
    
void load(Register dst, Register src, Register intReg) {
  loadi(dst, src, intReg, 0);
}
    
void load1(Register dst, Register src, Register intReg) {
  loadi(dst, src, intReg, 1);
}
    
void load2(Register dst, Register src, Register intReg) {
  loadi(dst, src, intReg, 2);
}
    
void load3(Register dst, Register src, Register intReg) {
  loadi(dst, src, intReg, 3);
}
    
void load4(Register dst, Register src, Register intReg) {
  loadi(dst, src, intReg, 4);
}
    
void load5(Register dst, Register src, Register intReg) {
  loadi(dst, src, intReg, 5);
}
    
void load6(Register dst, Register src, Register intReg) {
  loadi(dst, src, intReg, 6);
}
    
void load7(Register dst, Register src, Register intReg) {
  loadi(dst, src, intReg, 7);
}
    
void storei(Register dst, Register intReg, Register src, int offset) {
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

void store(Register dst, Register intReg, Register src) {
  storei(dst, intReg, src, 0);
}
    
void store1(Register dst, Register intReg, Register src) {
  storei(dst, intReg, src, 1);
}
    
void store2(Register dst, Register intReg, Register src) {
  storei(dst, intReg, src, 2);
}
    
void store3(Register dst, Register intReg, Register src) {
  storei(dst, intReg, src, 3);
}
    
void store4(Register dst, Register intReg, Register src) {
  storei(dst, intReg, src, 4);
}
    
void store5(Register dst, Register intReg, Register src) {
  storei(dst, intReg, src, 5);
}
    
void store6(Register dst, Register intReg, Register src) {
  storei(dst, intReg, src, 6);
}
    
void store7(Register dst, Register intReg, Register src) {
  storei(dst, intReg, src, 7);
}
    
void add(Register dst, Register src1, Register src2) {
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
    
void sub(Register dst, Register src1, Register src2) {
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
    
void mul(Register dst, Register src1, Register src2) {
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
    
void fdiv(Register dst, Register src1, Register src2) {
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

void idiv(Register dst, Register src1, Register src2) {
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
    
void imod(Register dst, Register src1, Register src2) {
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
    
void lessthan(Register dst, Register src1, Register src2) {
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

void equal(Register dst, Register src1, Register src2) {
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

void bnot(Register dst, Register src) {
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
    
void jump(Register intReg) {
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
    
void jumptrue(Register boolReg, Register intReg) {
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
    
void jumpfalse(Register boolReg, Register intReg) {
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
    
void call(Register dst, Register crPointerReg) {
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

void callrec(Register dst) {
  Pointer cr = read_cr();
  Int num_regs = read_num_regs(cr);
  Pointer ar = alloc_ar(read_ar(), dst, read_ip(), cr, num_regs);
  write_ar(ar);
  write_ip(0);
  for (int i = 1; i <= num_regs; i++) {
    init_local(ar, i, read_oid(i));
  }
}
    
void ret(Register src) {
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
    
void rettrue(Register boolReg, Register src) {
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
    
void retfalse(Register boolReg, Register src) {
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
