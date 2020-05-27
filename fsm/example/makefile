MEM=valgrind --leak-check=full
GDB_ENABLE=-g -O0
TEST=./example


example: turnstile.c
	gcc $(GDB_ENABLE) $(LINK_LIB) -o example turnstile.c ../fsm.c

.PHONY: mem test clean
mem:
	$(MEM) $(TEST)

test:
	$(TEST)

clean:
	rm $(TEST)
