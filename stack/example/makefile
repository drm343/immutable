MEM=valgrind --leak-check=full
GDB_ENABLE=-g -O0
LINK_LIB=
TEST=./example
LIB_O=../stack_int.o ../stack_string.o


example: stack.c $(LIB_O)
	gcc $(GDB_ENABLE) $(LINK_LIB) -o $(TEST) stack.c $(LIB_O)

.PHONY: mem test clean
mem:
	$(MEM) $(TEST)

test:
	$(TEST)

clean:
	rm $(TEST) $(LIB_O)
