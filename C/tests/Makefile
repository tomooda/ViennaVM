LIB = ../src/libViennaVM.a
CFLAGS = -L../src -lViennaVM

testRegister: testRegister.c $(LIB) ../src/register.h cUnit.h
	$(CC) $(CFLAGS) testRegister.c -o testRegister

testMemory: testMemory.c $(LIB) ../src/memory.h ../src/data.h cUnit.h
	$(CC) $(CFLAGS) testMemory.c -o testMemory

testData: testData.o $(LIB) ../src/data.h cUnit.h
	$(CC) $(CFLAGS) testData.c -o testData
