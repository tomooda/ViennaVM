LIB = ../src/libViennaVM.a
CFLAGS = -I../src -L../src -lViennaVM

testMemory: testMemory.c $(LIB) ../src/memory.h ../src/data.h
	$(CC) $(CFLAGS) testMemory.c -o testMemory

testData: testData.o $(LIB) ../src/data.h
	$(CC) $(CFLAGS) testData.c -o testData
