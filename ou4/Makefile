CC=gcc
CFLAGS = -std=c99 -Wall -g
INC = -I ../datastructures-v1.0.9/include/


# NOTE: these object paths assume that you make them in their respective datastructures-v1.0.9 folders
OBJECTS = graph.o ../datastructures-v1.0.9/src/queue/queue.o ../datastructures-v1.0.9/src/dlist/dlist.o ../datastructures-v1.0.9/src/list/list.o

GRAPHOBJECTS = ../datastructures-v1.0.9/src/dlist/dlist.o


# testmft: mtftable delim mwe1
# 	./mwe1


all: graph is_connected is_connected_old

is_connected: is_connected.c
	$(CC) $(CFLAGS) ./$< -o $@ $(INC) $(OBJECTS)
is_connected_old: is_connected_old.c
	$(CC) $(CFLAGS) ./$< -o $@ $(INC) $(OBJECTS)



graph: graph.c
	$(CC) $(CFLAGS) -c ./$< -o $@.o $(INC)

# mwe1: ./mwe1.c
# 	$(CC) $(CFLAGS) ./$< -o $@ $(INC) $(ARROBJECTS)

delim:
	@echo "-------------------------"

testgraph: is_connected delim mwe1
	./mwe1

memtest: ./is_connected
	valgrind --leak-check=full --show-reachable=yes ./$< airmap1.map

memtest2: ./is_connected_old
	valgrind --leak-check=full --show-reachable=yes ./$< airmap1.map
