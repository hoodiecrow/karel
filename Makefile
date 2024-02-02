PROGRAM  = ktr
O        = o
HDRS     = common.h chunk.h compiler.h debug.h memory.h scanner.h value.h vm.h
OBJS     = main.$(O) chunk.$(O) compiler.$(O) debug.$(O) memory.$(O) scanner.$(O) value.$(O) vm.$(O) object.$(O) table.$(O)

CC       = clang
FEATURES =
CFLAGS   = -g -W -Wall -Wextra -pedantic
LDFLAGS  =
LIBS     = 
RM       = rm -f
LD       = $(CC)
LDOUT    = -o 
EXE      =

.SUFFIXES: .obj

.c.obj:
	$(CC) $(CFLAGS) /c $*.c

.c.o:
	$(CC) $(CFLAGS) $(FEATURES) -c $*.c

$(PROGRAM)$(EXE): $(HDRS) $(OBJS)
	$(LD) $(LDFLAGS) $(OBJS) $(LIBS) $(LDOUT)$@$(EXE)

clean:
	$(RM) $(OBJS)
	$(RM) core $(PROGRAM) $(PROGRAM).exe
	$(RM) *~

