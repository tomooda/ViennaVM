LIB = ../src/libViennaVM.a
CFLAGS = -L../src -lViennaVM -O4
TESTS = testInstruction testActivationRecord testRegister testMemory testData testCodeRecord

all: $(TESTS)
	for p in $(TESTS); do echo [$$p]; ./$$p; done

clean:
	rm $(TESTS) fib

fib: fib.c $(LIB) ../src/instruction.h cUnit.h
	$(CC) $(CFLAGS) fib.c -o fib

testCodeRecord: testCodeRecord.c $(LIB) ../src/instruction.h cUnit.h
	$(CC) $(CFLAGS) testCodeRecord.c -o testCodeRecord

testInstruction: testInstruction.c $(LIB) ../src/instruction.h cUnit.h
	$(CC) $(CFLAGS) testInstruction.c -o testInstruction

testActivationRecord: testActivationRecord.c $(LIB) ../src/activationRecord.h cUnit.h
	$(CC) $(CFLAGS) testActivationRecord.c -o testActivationRecord

testRegister: testRegister.c $(LIB) ../src/register.h cUnit.h
	$(CC) $(CFLAGS) testRegister.c -o testRegister

testMemory: testMemory.c $(LIB) ../src/memory.h ../src/data.h cUnit.h
	$(CC) $(CFLAGS) testMemory.c -o testMemory

testData: testData.c $(LIB) ../src/data.h cUnit.h
	$(CC) $(CFLAGS) testData.c -o testData
