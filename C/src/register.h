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
} Reg;

typedef uint16_t Register;

#define NO_REGISTER 0
#define NUM_REGISTERS 0x10000

extern Pointer codeRecord;
extern Dword instructionPointer;
extern Pointer activationRecord;
extern Reg registers[];
extern Reg *regs;

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

static inline void offset_ip(int offset) {
  instructionPointer += offset;
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

static inline void offset_registers(int offset) {
  regs += offset;
}

static inline void move(Register dst, Register src) {
  if (isPointer(regs[src].oid)) {
    increment_reference_count(regs[src].oid);
  }
  if (isPointer(regs[dst].oid)) {
    decrement_reference_count(regs[dst].oid);
  }
  regs[dst] = regs[src];
}

static inline OID basic_read_oid(Register src) {
  return regs[src].oid;
}
static inline Int basic_read_int(Register src) {
  return (regs[src].i != invalidIntValue) ? regs[src].i : (regs[src].i = oid2int(regs[src].oid));
}
static inline Float basic_read_float(Register src) {
  Reg *reg = regs+src;
  register Float f = reg->f;
  if (f != invalidFloatValue)
    return f;
  f = oid2float(reg->oid);
  if (f != invalidFloatValue)
    return reg->f = f;
  return invalidFloatValue;
}

extern OID read_oid(Register r);
extern void write_oid(Register r, OID oid);

static inline Int read_int(Register src) {
  register Int i = regs[src].i;
  if (i != invalidIntValue)
    return i;
  i = oid2int(regs[src].oid);
  if (i != invalidIntValue)
    return regs[src].i = i;
  Float f = basic_read_float(src);
  if (f == invalidFloatValue)
    return invalidIntValue;
  float r = float2real(f);
  if (r == (float)((int)r)) 
    return regs[src].i = (int)r;
  return invalidIntValue;
}
extern void write_int(Register r, Int i);

static inline Float read_float(Register src) {
  Reg *reg = regs+src;
  register Float f = reg->f;
  if (f != invalidFloatValue)
    return f;
  f = oid2float(reg->oid);
  if (f != invalidFloatValue)
    return reg->f = f;
  Int i = basic_read_int(src);
  if (i != invalidIntValue)
    return reg->f = real2float((float)i);
  return invalidFloatValue;
}
extern void write_float(Register r, Float f);

extern Char read_char(Register r);
extern void write_char(Register r, Char c);

extern Pointer read_pointer(Register r);
extern void write_pointer(Register r, Pointer p);
extern void write_invalid(Register dst);

#endif
