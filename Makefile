CC = g++
CFLAGS = -Wall -Werror

all: server client

server: server/server.cpp keys.txt
	$(CC) $(CFLAGS) server/server.cpp -o bin/server.o

client: client/client.cpp keys.txt
	$(CC) $(CFLAGS) client/client.cpp -o bin/client.o

clean:
	rm  bin/server.o bin/client.o

.PHONY: all server client clean
