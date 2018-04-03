CFLAGS = -Wall -O4

OBJS = memory.o register.o codeRecord.o

LIB = libViennaVM.a
ARFLAGS = crsv

lib : $(LIB)

clean:
	rm $(OBJS) $(LIB)

$(LIB) : $(OBJS)
	$(AR) $(ARFLAGS) $(LIB) $(OBJS)
