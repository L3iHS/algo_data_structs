main.o: src/main.c src/records.h
	gcc -c src/main.c -o main.o

records.o: src/records.c src/records.h
	gcc -c src/records.c -o records.o

sort.o: src/sort.c src/sort.h src/records.h
	gcc -c src/sort.c -o sort.o

all: main.o records.o sort.o
	gcc -o lab2 main.o records.o sort.o

run: all
	./lab2

clean:
	rm -f *.o lab2
