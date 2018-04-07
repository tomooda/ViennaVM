#ifndef _REGISTER_H_
#define _REGISTER_H_
#include "memory.h"
#include "data.h"
#include <stdint.h>

typedef struct Reg_s {
  OID oid;
  Int i;
  Float f;
  Char c;
  Pointer p;
} Reg;

typedef uint16_t Register;

#define NO_REGISTER 0
#define NUM_REGISTERS 0x10000

extern Pointer codeRecord;
extern Dword instructionPointer;
extern Pointer activationRecord;
extern Reg registers[NUM_REGISTERS];

extern void register_reset();

static inline Pointer read_cr() {
  return codeRecord;
}
    
static inline void write_cr(Pointer pointer) {
  Pointer old_cr = codeRecord;
  codeRecord = pointer;
  instructionPointer = 0;
  if (codeRecord != invalidPointerValue) {
    increment_reference_count(codeRecord);
  }
  if (old_cr != invalidPointerValue) {
    decrement_reference_count(old_cr);
  }
}
    
static inline Dword read_ip() {
  return instructionPointer;
}
    
static inline void write_ip(Dword i) {
  instructionPointer = i;
}
    
static inline Pointer read_ar() {
  return activationRecord;
}
    
static inline void write_ar(Pointer ar) {
  Pointer old_ar = activationRecord;
  activationRecord = ar;
  if (ar != invalidPointerValue) {
    increment_reference_count(ar);
  }
  if (old_ar != invalidPointerValue) {
    decrement_reference_count(old_ar);
  }
}

static inline void move(Register dst, Register src) {
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

static inline OID read_oid(Register r) {
  Reg *reg = registers + r;
  OID oid;
  if (reg->oid != invalidOidValue) {
    return reg->oid;
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
  if (reg->p != invalidPointerValue) {
    oid = pointer2oid(reg->p);
    reg->oid = oid;
    return oid;
  }
  return invalidOidValue;
}

static inline void write_oid(Register r, OID oid) {
  Reg *reg = registers + r;
  if (reg->oid == oid) {
    return;
  }
  Pointer p1 = registers[r].p;
  Pointer p2 = oid2pointer(oid);
  if (p2 != invalidPointerValue) {
    increment_reference_count(p2);
  }
  if (p1 != invalidPointerValue) {
    decrement_reference_count(p1);
  }
  reg->oid = oid;
  reg->i = invalidIntValue;
  reg->f = invalidFloatValue;
  reg->c = invalidCharValue;
  reg->p = p2;
}

static inline Int read_int(Register r) {
  Int i = registers[r].i;
  if (i == invalidIntValue) {
    i = oid2int(registers[r].oid);
    if (i != invalidIntValue) {
      registers[r].i = i;
    }
  }
  return i;
}

static inline void write_int(Register r, Int i) {
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

static inline Float read_float(Register r) {
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
    
static inline void write_float(Register r, Float f) {
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

static inline Char read_char(Register r) {
  Char c = registers[r].c;
  if (c == invalidCharValue) {
    c = oid2char(registers[r].oid);
    if (c != invalidCharValue) {
      registers[r].c = c;
    }
  }
  return c;
}

static inline void write_char(Register r, Char c) {
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

static inline Pointer read_pointer(Register r) {
  return registers[r].p;
}
    
static inline void write_pointer(Register r, Pointer p) {
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

#endif
