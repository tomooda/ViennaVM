#include "register.h"
#include "data.h"

Pointer codeRecord = invalidPointerValue;
Dword instructionPointer = 0;
Pointer activationRecord = invalidPointerValue;
Reg registers[NUM_REGISTERS] = {};

void register_reset() {
  for (int r = 0; r < NUM_REGISTERS; r++) {
    Reg *reg = registers+r;
    reg->oid = invalidTag;
    reg->i = invalidIntValue;
    reg->f = invalidFloatValue;
    reg->c = invalidCharValue;
    reg->p = invalidPointerValue;
  }
}

void move(Register dst, Register src) {
  if (dst == src) {
    return;
  }
  Pointer srcPointer = registers[src].p;
  Pointer dstPointer = registers[dst].p;
  if (srcPointer != invalidPointerValue) {
    increment_reference_count(srcPointer);
  }
  if (dstPointer != invalidPointerValue) {
    decrement_reference_count(dstPointer);
  }
  registers[dst] = registers[src];
}

OID read_oid(Register r) {
  Reg *reg = registers + r;
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

void write_oid(Register r, OID oid) {
  Reg *reg = registers + r;
  if (reg->oid == oid && oid != invalidOidValue) {
    return;
  }
  Pointer p1 = registers[r].p;
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

Int read_int(Register r) {
  Int i = registers[r].i;
  if (i == invalidIntValue) {
    i = oid2int(registers[r].oid);
    if (i != invalidIntValue) {
      registers[r].i = i;
    }
  }
  return i;
}

void write_int(Register r, Int i) {
  Reg *reg = &registers[r];
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

Float read_float(Register r) {
  Reg *reg = registers+r;
  Float f = reg->f;
  if (f == invalidFloatValue) {
    f = oid2float(reg->oid);
    if (f == invalidFloatValue) {
      Int i = read_int(r);
      if (i != invalidFloatValue) {
	f = (Float)i;
      }
    }
    reg->f = f;
  }
  return f;
}
    
void write_float(Register r, Float f) {
  Reg *reg = registers+r;
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

Char read_char(Register r) {
  Char c = registers[r].c;
  if (c == invalidCharValue) {
    c = oid2char(registers[r].oid);
    if (c != invalidCharValue) {
      registers[r].c = c;
    }
  }
  return c;
}

void write_char(Register r, Char c) {
  Reg *reg = registers+r;
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

Pointer read_pointer(Register r) {
  return registers[r].p;
}
    
void write_pointer(Register r, Pointer p) {
  Reg *reg = registers+r;
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
