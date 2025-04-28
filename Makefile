CC=gcc
CFLAGS=-Wall -pthread

all: setup

setup: main.o setup.o handler.o request.o response.o
	$(CC) $(CFLAGS) -o setup main.o setup.o handler.o request.o response.o

main.o: main.c setup.h handler.h
	$(CC) $(CFLAGS) -c main.c

setup.o: setup.c setup.h handler.h request.h response.h
	$(CC) $(CFLAGS) -c setup.c

handler.o: handler.c handler.h request.h response.h
	$(CC) $(CFLAGS) -c handler.c

request.o: request.c request.h
	$(CC) $(CFLAGS) -c request.c

response.o: response.c response.h
	$(CC) $(CFLAGS) -c response.c

clean:
	rm -f *.o setup

