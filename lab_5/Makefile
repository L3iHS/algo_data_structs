main.o: src/main.c src/graph.h src/dijkstra.h
	gcc -Wall -Wextra -Werror -std=c11 -c src/main.c -o main.o

graph.o: src/graph.c src/graph.h
	gcc -Wall -Wextra -Werror -std=c11 -c src/graph.c -o graph.o

dijkstra.o: src/dijkstra.c src/dijkstra.h src/graph.h
	gcc -Wall -Wextra -Werror -std=c11 -c src/dijkstra.c -o dijkstra.o

test.o: tests/test.cpp src/graph.h src/dijkstra.h
	g++ -Wall -Wextra -Werror -std=c++17 $$(pkg-config --cflags gtest) -c tests/test.cpp -o test.o

all: main.o graph.o dijkstra.o
	gcc -Wall -Wextra -Werror -std=c11 -o lab5 main.o graph.o dijkstra.o

test: test.o graph.o dijkstra.o
	g++ -Wall -Wextra -Werror -std=c++17 -o test test.o graph.o dijkstra.o $$(pkg-config --libs gtest_main)

run: all
	./lab5 input.txt 0 graph_output.txt result_output.txt

run_cli: all
	./lab5

run_test: test
	./test

clean:
	rm -f *.o lab5 test graph_output.txt result_output.txt
