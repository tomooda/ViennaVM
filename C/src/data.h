#include <stdint.h>
#include <math.h>

typedef uint8_t              Byte;
typedef uint16_t             Word;
typedef uint32_t             Dword;
typedef uint64_t             Qword;
typedef Qword                OID;
typedef Int                  int64_t;
typedef float                Float;
typedef Char                 Dword;
typedef Pointer              Qword;

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
#define invalidIntValue      0x7fffffffffffffff
#define invalidFloatValue    NAN
#define invalidCharValue     0xffffffff
#define invalidPointerValue  0x100000000000000

typedef union {Word words[4]; Qword qword} words_qword;

extern Int oid2int(OID oid);
extern OID int2oid(Int i);
extern Float oid2float(OID oid);
extern OID float2oid(Float f);
