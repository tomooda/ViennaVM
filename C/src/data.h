#ifndef _DATA_H_
#define _DATA_H_

#include <stdint.h>
#include <math.h>

typedef uint8_t              Byte;
typedef uint16_t             Word;
typedef uint32_t             Dword;
typedef uint64_t             Qword;
typedef Qword                OID;
typedef int64_t              Int;
typedef uint32_t             Float;
typedef Dword                Char;
typedef Qword                Pointer;

/* oid tag flags */
#define smallIntegerFlag     0x01
#define immediateFlag        0x02
#define typeFlag             0x04
#define optionTypeFlag       0x08
/* immediate value oid tags */
#define smallIntegerTag      0x01
#define invalidTag           0x0a
#define floatTag             0x12
#define charTag              0x1a
/* common value oids */
#define nilValue             0x22
#define trueValue            0x2a
#define falseValue           0x32
#define undefinedValue       0x3a
#define emptySeqValue        0x82
#define emptySetValue        0x8a
#define emptyMapValue        0x92
/* immediate type oid defs */
#define unitType             0x06
#define boolType             0x16
#define charType             0x26
#define realType             0x36
#define intType              0x46
#define natType              0x56
#define nat1Type             0x66
#define tokenType            0x76
#define anyType              0xf6
/* heap value oid defs */
#define tupleRecordTag       0x10
#define tokenTag             0x20
#define seqTag               0x30
#define setTag               0x40
#define mapTag               0x50
#define quoteTag             0x60
#define functionTag          0x70
#define operationTag         0x80
#define largePositiveIntTag  0x90
#define largeNegativeIntTag  0xa0
/* heap type oid defs */
#define tupleType            0x04
#define recordType           0x14
#define seqType              0x24
#define setType              0x34
#define mapType              0x44
#define quoteType            0x54
#define functionType         0x64
#define operationType        0x74
#define productType          0x84
#define unionType            0x94
/* default values */
#define invalidOidValue      invalidTag
#define invalidIntValue      (Int)0x7fffffffffffffff
#define invalidFloatValue    (Float)0xffffffff
#define invalidCharValue     0xffffffff
#define invalidPointerValue  (Pointer)0x100000000000000

typedef union {Word words[4]; Qword qword;} words_qword;
typedef union {float r; Float f;} float_real;

static inline int isPointer(oid) {
  return !(oid & 3);
}

static inline Int oid2int(OID oid) {
  return oid & 1 ? ((Int)oid) >> 1 : invalidIntValue;
}
static inline OID int2oid(Int i) {
  return i != invalidIntValue ? (i << 1) | smallIntegerTag : invalidOidValue;
}

static inline Float oid2float(OID oid) {
  if ((oid & 0xff) == floatTag) {
    return oid >> 8;
  } else {
    return invalidFloatValue;
  }
}
static inline OID float2oid(Float f) {
  if (f != invalidFloatValue) {
    return (((OID)f) << 8) | floatTag;
  } else {
    return invalidOidValue;
  }
}

static inline float float2real(Float f) {
  float_real v;
  v.f = f;
  return v.r;
}

static inline Float real2float(float r) {
  float_real v;
  v.r = r;
  return v.f;
}

static inline Char oid2char(OID oid) {
  return (oid & 0xff) == charTag ? oid >> 8 : invalidCharValue;
}
static inline OID char2oid(Char c) {
  return c != invalidCharValue ? (((OID)c) << 8) | charTag : invalidOidValue;
}

static inline Pointer oid2pointer(OID oid) {
  return (oid & 0x3) == 0 ? oid >> 8 : invalidPointerValue;
}
static inline OID pointer2oid(Pointer p) {
  return p != invalidPointerValue ? ((OID)p) << 8 : invalidOidValue;
}

static inline Qword words2qword(Word w1, Word w2, Word w3, Word w4) {
  words_qword v;
  v.words[3] = w1;
  v.words[2] = w2;
  v.words[1] = w3;
  v.words[0] = w4;
  return v.qword;
}

#endif
