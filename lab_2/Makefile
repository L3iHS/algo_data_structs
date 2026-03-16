main.o: src/main.c src/records.h
	gcc -c src/main.c -o main.o

records.o: src/records.c src/records.h
	gcc -c src/records.c -o records.o

sort.o: src/sort.c src/sort.h src/records.h
	gcc -c src/sort.c -o sort.o

search.o: src/search.c src/search.h src/records.h
	gcc -c src/search.c -o search.o

all: main.o records.o sort.o search.o
	gcc -o lab2 main.o records.o sort.o search.o

run: all
	./lab2

clean:
	rm -f *.o lab2
