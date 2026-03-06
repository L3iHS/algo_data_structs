main.o: main.c
	gcc -c main.c

tree.o: tree.c
	gcc -c tree.c

all: main.o tree.o
	gcc -o main main.o tree.o

run: all
	./main

clean:
	rm -f *.o main
