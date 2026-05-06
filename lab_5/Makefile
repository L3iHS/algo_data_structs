main.o: src/main.c src/graph.h src/dijkstra.h
	gcc -Wall -Wextra -Werror -std=c11 -c src/main.c -o main.o

graph.o: src/graph.c src/graph.h
	gcc -Wall -Wextra -Werror -std=c11 -c src/graph.c -o graph.o

dijkstra.o: src/dijkstra.c src/dijkstra.h src/graph.h
	gcc -Wall -Wextra -Werror -std=c11 -c src/dijkstra.c -o dijkstra.o

all: main.o graph.o dijkstra.o
	gcc -Wall -Wextra -Werror -std=c11 -o lab5 main.o graph.o dijkstra.o

run: all
	./lab5 input.txt 0 graph_output.txt result_output.txt

run_cli: all
	./lab5

clean:
	rm -f *.o lab5 graph_output.txt result_output.txt
