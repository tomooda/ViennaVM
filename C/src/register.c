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
  static Reg r = {invalidOidValue, invalidIntValue, invalidFloatValue, invalidCharValue, invalidPointerValue};
  r.oid = oid;
  r.p = oid2pointer(oid);
  if (r.p != invalidPointerValue) {
    increment_reference_count(r.p);
  }
  if (regs[dst].p != invalidPointerValue) {
    decrement_reference_count(regs[dst].p);
  }
  regs[dst] = r;
}

void write_int(Register dst, Int i) {
  static Reg r = {invalidOidValue, invalidIntValue, invalidFloatValue, invalidCharValue, invalidPointerValue};
  r.i = i;
  if (regs[dst].p != invalidPointerValue) {
    decrement_reference_count(regs[dst].p);
  }
  regs[dst] = r;
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
  static Reg r = {invalidOidValue, invalidIntValue, invalidFloatValue, invalidCharValue, invalidPointerValue};
  r.f = f;
  if (regs[dst].p != invalidPointerValue) {
    decrement_reference_count(regs[dst].p);
  }
  regs[dst] = r;
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
  static Reg r = {invalidOidValue, invalidIntValue, invalidFloatValue, invalidCharValue, invalidPointerValue};
  r.c = c;
  if (regs[dst].p != invalidPointerValue) {
    decrement_reference_count(regs[dst].p);
  }
  regs[dst] = r;
}

Pointer read_pointer(Register src) {
  return regs[src].p;
}
    
void write_pointer(Register dst, Pointer p) {
  static Reg r = {invalidOidValue, invalidIntValue, invalidFloatValue, invalidCharValue, invalidPointerValue};
  r.p = p;
  if (p != invalidPointerValue) {
    increment_reference_count(p);
  }
  if (regs[dst].p != invalidPointerValue) {
    decrement_reference_count(regs[dst].p);
  }
  regs[dst] = r;
}

