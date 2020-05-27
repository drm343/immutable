MEM=valgrind --leak-check=full
GDB_ENABLE=-g -O0
LINK_LIB=`pkg-config --libs readline`
TEST=./example_stack
REMOVES=


example: stack.c ../stack_string.c ../stack.h ../stack.c
	gcc $(GDB_ENABLE) $(LINK_LIB) -o example_stack stack.c ../stack_string.c

.PHONY: mem test clean
mem:
	$(MEM) $(TEST)

test:
	$(TEST)

clean:
	rm $(TEST)
