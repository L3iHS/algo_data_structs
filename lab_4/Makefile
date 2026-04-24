main.o: src/main.c src/rb_tree.h
	gcc -c src/main.c -o main.o

rb_tree.o: src/rb_tree.c src/rb_tree.h
	gcc -c src/rb_tree.c -o rb_tree.o

test.o: tests/test.c src/rb_tree.h
	gcc -c tests/test.c -o test.o

all: main.o rb_tree.o
	gcc -o lab4 main.o rb_tree.o

test: test.o rb_tree.o
	gcc -o test test.o rb_tree.o

run: all
	./lab4 input.txt

run_test: test
	./test

clean:
	rm -f *.o lab4 test
