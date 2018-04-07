#include "../src/codeRecord.h"
#include "../src/data.h"
#include "../src/register.h"
#include "../src/instruction.h"
#include "cUnit.h"

int main(int argc, char **argv) {
  if (argc != 2) {
    fprintf(stderr, "usage: %s <num>\n", argv[0]);
    exit(1);
  }
  int n;
  sscanf(argv[1],"%d",&n);
  static OID _ = invalidOidValue;
  Instruction src[10] = {
    /* r1: i, i-1, i-2 r2: 1, sum(i-1) */
    {MOVEI,    2, 0, 0, int2oid(1), NULL, NULL},
    {LESSTHAN, 3, 2, 1, _, NULL, NULL},
    {RETFALSE, 3, 1, 0, _, NULL, NULL},
    {SUB,      1, 1, 2, _, NULL, NULL},
    {CALLREC, 3, 0, 0, _, NULL, NULL},
    {SUB,      1, 1, 2, _, NULL, NULL},
    {CALLREC, 1, 0, 0, _, NULL, NULL},
    {ADD,      1, 1, 3, _, NULL, NULL},
    {RET,      1, 0, 0, _, NULL, NULL},
    {0xffff,   0, 0, 0, _, NULL, NULL}};
  memory_reset();
  register_reset();
  Pointer cr = assemble(invalidOidValue, invalidOidValue, src);
  write_cr(invalidPointerValue);
  write_ip(0);
  write_int(1, n);
  write_pointer(2, cr);
  call(1, 2);
  while (read_cr() != invalidPointerValue) {
    step();
  }
  printf("fib(%d)=%lld\n", n, read_int(1));
  return 0;
}
