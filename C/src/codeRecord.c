#include <stdlib.h>
#include <string.h>
#include "codeRecord.h"

typedef struct labeldef_s {
  char *label;
  Dword value;
} labeldef;

Pointer assemble(OID argtypes, OID rettype, Instruction *instructions) {
  int size = 0;
  int num_labels = 0;
  int max_reg = 0;
  for (int i = 0; instructions[i].opcode < 0x8000; i++) {
    ++size;
    if (instructions[i].imm != invalidOidValue) {
      ++size;
    }
    if (instructions[i].labeldef) {
      ++num_labels;
    }
    if (instructions[i].reg1 > max_reg) {
      max_reg = instructions[i].reg1;
    }
    if (instructions[i].reg2 > max_reg) {
      max_reg = instructions[i].reg2;
    }
    if (instructions[i].reg3 > max_reg) {
      max_reg = instructions[i].reg3;
    }
  }
  labeldef *labelTable = malloc(sizeof(labeldef) * num_labels);
  int labelTableSize = 0;
  Pointer cr = alloc(size + IP_SLOT_OFFSET - 1);
  Dword ip = 0;
  for (int i = 0; instructions[i].opcode < 0x8000; i++) {
    if (instructions[i].labeldef) {
      labelTable[labelTableSize].label = instructions[i].labeldef;
      labelTable[labelTableSize].value = ip;
      ++labelTableSize;
    }
    ++ip;
    if (instructions[i].imm != invalidOidValue) {
      ++ip;
    }
    write_slot(cr, NUM_REGS_SLOT, int2oid(max_reg));
    write_slot(cr, ARGTYPES_SLOT, argtypes);
    write_slot(cr, RETTYPE_SLOT, rettype);
  }
  ip = 0;
  for (int i = 0; instructions[i].opcode < 0x8000; i++) {
    Dword old_ip = ip;
    Word opcode = instructions[i].opcode;
    ++ip;
    if (instructions[i].imm != invalidOidValue) {
      write_slot(cr, IP_SLOT_OFFSET + ip, instructions[i].imm);
      ++ip;
    }
    Word reg1 = instructions[i].reg1 >= 0 ? instructions[i].reg1 : max_reg - instructions[i].reg1;
    Word reg2 = instructions[i].reg2 >= 0 ? instructions[i].reg2 : max_reg - instructions[i].reg2;
    Word reg3 = instructions[i].reg3 >= 0 ? instructions[i].reg3 : max_reg - instructions[i].reg3;
    if (instructions[i].labelref) {
      for (int l = 0; l < labelTableSize; l++) {
	if (!strcmp(instructions[i].labelref, labelTable[l].label)) {
	  int offset = (int)labelTable[l].value - (int)ip;
	  if (!reg1) reg1 = offset;
	  else if (!reg2) reg2 = offset;
	  else if (!reg3) reg3 = offset;
	  break;
	}
      }
    }
    write_slot
      (cr,
       IP_SLOT_OFFSET + old_ip,
       int2oid(words2qword(opcode, reg1, reg2, reg3)));
  }
  return cr;
}
