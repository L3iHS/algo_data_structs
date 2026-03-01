func:
  gcc -c func.c

# compile the main program
all: func test
  gcc -o main func.o main.c

clean:
  rm -f *.o main test

test: func
  gcc -o test func.o test.c
run:
  ./main
run_test:
  ./test