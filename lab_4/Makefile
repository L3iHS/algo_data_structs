main.o: src/main.c src/ast.h src/parser.h src/transform.h src/print.h
	gcc -c src/main.c -o main.o

ast.o: src/ast.c src/ast.h
	gcc -c src/ast.c -o ast.o

parser.o: src/parser.c src/parser.h src/ast.h
	gcc -c src/parser.c -o parser.o

transform.o: src/transform.c src/transform.h src/ast.h
	gcc -c src/transform.c -o transform.o

print.o: src/print.c src/print.h src/ast.h
	gcc -c src/print.c -o print.o

test.o: tests/test.c src/ast.h src/parser.h src/transform.h src/print.h
	gcc -c tests/test.c -o test.o

all: main.o ast.o parser.o transform.o print.o
	gcc -o lab3 main.o ast.o parser.o transform.o print.o

test: test.o ast.o parser.o transform.o print.o
	gcc -o test test.o ast.o parser.o transform.o print.o

run: all
	./lab3

run_test: test
	./test

clean:
	rm -f *.o lab3 test
