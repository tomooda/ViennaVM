#include <stdio.h>
#include <math.h>

#define assert(bool, msg) if (!(bool)) {fprintf(stderr, "%s\n", msg); return 1;}
#define assertEquals(actual, expected, msg) if (actual != expected) {fprintf(stderr, "%s - expected: %llx, actual: %llx\n", msg, (unsigned long long)expected, (unsigned long long)actual); return 1;}
#define assertApproxEquals(actual, expected, accuracy, msg) if (fabs(actual-expected) > accuracy) {fprintf(stderr, "%s - expected: %llx, actual: %llx\n", msg, (unsigned long long)expected, (unsigned long long)actual); return 1;}
#define runTest(func) printf("%s: ", #func); if (func()) {printf("failed\n"); return 1; } else {printf("passed\n");}
