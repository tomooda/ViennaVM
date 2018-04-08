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

extern void move(Register dst, Register src);
extern OID read_oid(Register r);
extern void write_oid(Register r, OID oid);
extern Int read_int(Register r);
extern void write_int(Register r, Int i);
extern Float read_float(Register r);
extern void write_float(Register r, Float f);
extern Char read_char(Register r);
extern void write_char(Register r, Char c);
extern Pointer read_pointer(Register r);
extern void write_pointer(Register r, Pointer p);

#endif
