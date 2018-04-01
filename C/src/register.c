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

