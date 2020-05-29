MEM=valgrind --leak-check=full
GDB_ENABLE=-g -O0
LINK_LIB=
LIB_O=stack_int.o stack_string.o


all: $(LIB_O)

stack_string.o: stack_string.c stack.h stack.c
	gcc $(GDB_ENABLE) $(LINK_LIB) -o stack_string.o -c stack_string.c

stack_int.o: stack_int.c stack.h stack.c
	gcc $(GDB_ENABLE) $(LINK_LIB) -o stack_int.o -c stack_int.c

.PHONY: all clean

clean:
	rm $(LIB_O)
