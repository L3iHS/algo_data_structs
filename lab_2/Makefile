main.o: src/main.c src/records.h
	gcc -c src/main.c -o main.o

records.o: src/records.c src/records.h
	gcc -c src/records.c -o records.o

all: main.o records.o
	gcc -o lab2 main.o records.o

run: all
	./lab2

clean:
	rm -f *.o lab2
