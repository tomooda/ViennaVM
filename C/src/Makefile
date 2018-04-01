CFLAGS = -Wall

OBJS = memory.o register.o codeRecord.o

LIB = libViennaVM.a
ARFLAGS = crsv

lib : $(LIB)

$(LIB) : $(OBJS)
	$(AR) $(ARFLAGS) $(LIB) $(OBJS)
