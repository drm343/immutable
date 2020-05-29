MEM=valgrind --leak-check=full
GDB_ENABLE=-g -O0
LINK_LIB=`pkg-config --libs readline`
TEST=./example_s_exp


example: example_base example_btree example_avl example_s_exp


example_base: base_tree.c ../tree.c ../tree.h
	gcc $(GDB_ENABLE) $(LINK_LIB) -o example_base base_tree.c ../tree.c


example_btree: btree.c ../tree.c ../tree.h
	gcc $(GDB_ENABLE) $(LINK_LIB) -o example_btree btree.c ../tree.c


example_avl: avl_tree.c ../tree.c ../tree.h
	gcc $(GDB_ENABLE) $(LINK_LIB) -o example_avl avl_tree.c ../../stack/stack.c


example_s_exp: s_exp.c ../tree.c ../tree.h
	gcc $(GDB_ENABLE) $(LINK_LIB) -o example_s_exp s_exp.c ../tree.c


.PHONY: mem test clean
mem:
	$(MEM) $(TEST)

test:
	./example_base
	./example_btree
	./example_avl

clean:
	rm ./example_*
