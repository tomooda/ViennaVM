#include <stdio.h>

#define assertEquals(actual, expected, msg) if (actual != expected) {fprintf(stderr, "%s\n", msg); return 1;}
