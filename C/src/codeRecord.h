#ifndef _CODE_RECORD_H_
#define _CODE_RECORD_H_
#include "data.h"
#include "memory.h"
#include "register.h"

#define NUM_REGS_SLOT      1
#define ARGTYPES_SLOT      2
#define RETTYPE_SLOT       3
#define IP_SLOT_OFFSET     4

static inline Dword read_code_size(Pointer cr) {
  return slots_size(cr) - IP_SLOT_OFFSET + 1;
}
    
static inline Int read_num_regs(Pointer cr) {
  return oid2int(read_slot(cr, NUM_REGS_SLOT));
}
    
static inline OID read_argtypes(Pointer cr) {
  return read_slot(cr, ARGTYPES_SLOT);
}
    
static inline OID read_rettype(Pointer cr) {
  return read_slot(cr, RETTYPE_SLOT);
}
    
static inline OID read_code(Pointer cr, Dword ip) {
  return read_slot(cr, IP_SLOT_OFFSET + ip);
}
    
static inline void write_code(Pointer cr, Dword ip, OID code) {
  write_slot(cr, IP_SLOT_OFFSET + ip, code);
}

typedef struct Instruction_s {
  Word opcode;
  int reg1;
  int reg2;
  int reg3;
  OID imm;
  char *labelref;
  char *labeldef;
} Instruction;

extern Pointer assemble(OID argtypes, OID rettype, Instruction *instructions);

#endif
