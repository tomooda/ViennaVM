#ifndef _ACTIVATION_RECORD_H
#define _ACTIVATION_RECORD_H

#include "data.h"
#include "memory.h"
#include "register.h"

#define DYNAMIC_LINK_SLOT      1
#define RETURN_IP_REG_SLOT     2
#define RETURN_CR_SLOT         3
#define MEASURE                4
#define LOCAL_SLOT_OFFSET      4

static inline Pointer alloc_ar(Pointer caller, Register ret_reg, Dword ret_ip, Pointer ret_cr, Register num_locals) {
  Pointer ar = alloc(LOCAL_SLOT_OFFSET + num_locals);
  init_slot(ar, DYNAMIC_LINK_SLOT, pointer2oid(caller));
  init_slot(ar, RETURN_IP_REG_SLOT, int2oid(ret_ip+ret_reg*0x100000000));
  init_slot(ar, RETURN_CR_SLOT, pointer2oid(ret_cr));
  return ar;
}

static inline Pointer read_dynamic_link(Pointer ar) {
  return oid2pointer(read_slot(ar, DYNAMIC_LINK_SLOT));
}
    
static inline Word read_ret_reg(Pointer ar) {
  return oid2int(read_slot(ar, RETURN_IP_REG_SLOT)) / 0x100000000;
}
    
static inline Dword read_return_ip(Pointer ar) {
  return oid2int(read_slot(ar, RETURN_IP_REG_SLOT)) % 0x100000000;
}
    
static inline Pointer read_return_cr(Pointer ar) {
  return oid2pointer(read_slot(ar, RETURN_CR_SLOT));
}
    
static inline OID read_local(Pointer ar, Dword i) {
  return read_slot(ar, LOCAL_SLOT_OFFSET + i);
}
    
static inline void write_local(Pointer ar, Dword i, OID oid) {
  write_slot(ar, LOCAL_SLOT_OFFSET + i, oid);
}

static inline void init_local(Pointer ar, Dword i, OID oid) {
  init_slot(ar, LOCAL_SLOT_OFFSET + i, oid);
}

#endif
