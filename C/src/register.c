#include "register.h"
#include "data.h"

Pointer codeRecord = invalidPointerValue;
Dword instructionPointer = 0;
Pointer activationRecord = invalidPointerValue;
Reg *regs = registers;
Reg registers[NUM_REGISTERS] = {};

void register_reset() {
  Reg *reg = registers;
  for (int r = 0; r < NUM_REGISTERS; r++) {
    reg->oid = invalidTag;
    reg->i = invalidIntValue;
    reg->f = invalidFloatValue;
    reg->c = invalidCharValue;
    (reg++)->p = invalidPointerValue;
  }
  regs = registers;
}

void move(Register dst, Register src) {
  if (dst == src) {
    return;
  }
  Pointer dstPointer = regs[dst].p;
  Pointer srcPointer = regs[src].p;
  if (srcPointer != invalidPointerValue) {
    increment_reference_count(srcPointer);
  }
  if (dstPointer != invalidPointerValue) {
    decrement_reference_count(dstPointer);
  }
  regs[dst] = regs[src];
}

OID read_oid(Register src) {
  Reg *reg = regs + src;
  OID oid;
  if (reg->oid != invalidOidValue) {
    return reg->oid;
  }
  if (reg->p != invalidPointerValue) {
    oid = pointer2oid(reg->p);
    reg->oid = oid;
    return oid;
  }
  if (reg->i != invalidIntValue) {
    oid = int2oid(reg->i);
    reg->oid = oid;
    return oid;
  }
  if (reg->f != invalidFloatValue) {
    oid = float2oid(reg->f);
    reg->oid = oid;
    return oid;
  }
  if (reg->c != invalidCharValue) {
    oid = char2oid(reg->c);
    reg->oid = oid;
    return oid;
  }
  return invalidOidValue;
}

void write_oid(Register dst, OID oid) {
  Reg *reg = regs + dst;
  if (reg->oid == oid && oid != invalidOidValue) {
    return;
  }
  Pointer p1 = regs[dst].p;
  Pointer p2 = oid2pointer(oid);
  if (p1 != p2) {
    if (p2 != invalidPointerValue) {
      increment_reference_count(p2);
    }
    if (p1 != invalidPointerValue) {
      decrement_reference_count(p1);
    }
  }
  reg->oid = oid;
  reg->i = invalidIntValue;
  reg->f = invalidFloatValue;
  reg->c = invalidCharValue;
  reg->p = p2;
}

Int read_int(Register src) {
  Int i = regs[src].i;
  if (i == invalidIntValue) {
    i = oid2int(regs[src].oid);
    if (i != invalidIntValue) {
      regs[src].i = i;
    }
  }
  return i;
}

void write_int(Register dst, Int i) {
  Reg *reg = regs+dst;
  if (reg->i == i) {
    return;
  }
  if (reg->p != invalidPointerValue) {
    decrement_reference_count(reg->p);
  }
  reg->oid = invalidOidValue;
  reg->i = i;
  reg->f = invalidFloatValue;
  reg->c = invalidCharValue;
  reg->p = invalidPointerValue;
}

Float read_float(Register src) {
  Reg *reg = regs+src;
  Float f = reg->f;
  if (f == invalidFloatValue) {
    f = oid2float(reg->oid);
    if (f == invalidFloatValue) {
      Int i = read_int(src);
      if (i != invalidIntValue) {
	f = (Float)i;
      }
    }
    reg->f = f;
  }
  return f;
}
    
void write_float(Register dst, Float f) {
  Reg *reg = regs+dst;
  if (reg->f == f) {
    return;
  }
  if (reg->p != invalidPointerValue) {
    decrement_reference_count(reg->p);
  }
  reg->oid = invalidOidValue;
  reg->i = invalidIntValue;
  reg->f = f;
  reg->c = invalidCharValue;
  reg->p = invalidPointerValue;
}

Char read_char(Register src) {
  Reg *reg = regs+src;
  Char c = reg->c;
  if (c == invalidCharValue) {
    c = oid2char(reg->oid);
    if (c != invalidCharValue) {
      reg->c = c;
    }
  }
  return c;
}

void write_char(Register dst, Char c) {
  Reg *reg = regs+dst;
  if (reg -> c == c) {
    return;
  }
  if (reg->p != invalidPointerValue) {
    decrement_reference_count(reg->p);
  }
  reg->oid = invalidOidValue;
  reg->i = invalidIntValue;
  reg->f = invalidFloatValue;
  reg->c = c;
  reg->p = invalidPointerValue;
}

Pointer read_pointer(Register src) {
  return regs[src].p;
}
    
void write_pointer(Register dst, Pointer p) {
  Reg *reg = regs+dst;
  if (reg -> p == p) {
    return;
  }
  if (p != invalidPointerValue) {
    increment_reference_count(p);
  }
  if (reg->p != invalidPointerValue) {
    decrement_reference_count(reg->p);
  }
  reg->oid = invalidOidValue;
  reg->i = invalidIntValue;
  reg->f = invalidFloatValue;
  reg->c = invalidCharValue;
  reg->p = p;
}

