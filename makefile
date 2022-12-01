CC=gcc

all: a5 run

v:
	valgrind ./a5

run:
	./a5

a5: main.o building.o ghost.o room.o hunter.o cleanup.o evidence.o result.o
	$(CC) -o a5 main.o building.o ghost.o room.o hunter.o cleanup.o evidence.o result.o -lpthread

main.o: main.c defs.h
	$(CC) -c main.c

building.o: building.c defs.h
	$(CC) -c building.c

ghost.o: ghost.c defs.h
	$(CC) -c ghost.c

room.o: room.c defs.h
	$(CC) -c room.c

hunter.o: hunter.c defs.h
	$(CC) -c hunter.c

cleanup.o: cleanup.c defs.h
	$(CC) -c cleanup.c

result.o: result.c defs.h
	$(CC) -c result.c

evidence.o: evidence.c defs.h
	$(CC) -c evidence.c


clean:
	rm -f main.o building.o ghost.o a5 room.o hunter.o cleanup.o evidence.o 