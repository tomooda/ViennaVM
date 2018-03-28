#include <stdint.h>
#include <math.h>

typedef uint8_t              Byte;
typedef uint16_t             Word;
typedef uint32_t             Dword;
typedef uint64_t             Qword;
typedef Qword                OID;
typedef int64_t              Int;
typedef float                Float;
typedef Dword                Char;
typedef Qword                Pointer;

/* oid tag flags */
#define smallIntegerFlag     0x01
#define immediateFlag        0x02
/* immediate value oid tags */
#define smallIntegerTag      0x01
#define invalidTag           0x0a
#define floatTag             0x12
#define charTag              0x1a
/* common value oids */
#define nilValue             0x22
#define trueValue            0x2a
#define falseValue           0x32
/* default values */
#define invalidOidValue      invalidTag
#define invalidIntValue      (Int)0x7fffffffffffffff
#define invalidFloatValue    (Float)NAN
#define invalidCharValue     0xffffffff
#define invalidPointerValue  (Pointer)0x100000000000000

typedef union {Word words[4]; Qword qword;} words_qword;
typedef union {float f; uint32_t i;} float_int;

static inline Int oid2int(OID oid) {
  return oid & 1 ? ((Int)oid) >> 1 : invalidIntValue;
}
static inline OID int2oid(Int i) {
  return i != invalidIntValue ? (i << 1) | smallIntegerTag : invalidOidValue;
}

static inline Float oid2float(OID oid) {
  if ((oid & 0xff) == floatTag) {
    float_int v;
    v.i = oid >> 8;
    return v.f;
  } else {
    return invalidFloatValue;
  }
}
static inline OID float2oid(Float f) {
  if (f != invalidFloatValue) {
    float_int v;
    v.f = f;
    return (((OID)v.i) << 8) | floatTag;
  } else {
    return invalidOidValue;
  }
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
