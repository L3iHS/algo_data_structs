main.o: main.c
	gcc -c main.c

tree.o: tree.c
	gcc -c tree.c

test.o: test.c
	gcc -c test.c

test: test.o tree.o
	gcc -o test test.o tree.o

run_test: test
	./test

all: main.o tree.o
	gcc -o main main.o tree.o

run: all
	./main

clean:
	rm -f *.o main test
