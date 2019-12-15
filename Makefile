server: server/server.cpp keys.txt
	g++ -Wall -Werror  server/server.cpp -o bin/server

client: client/client.cpp keys.txt
	g++ -Wall -Werror  client/client.cpp -o bin/client

clean: FORCE
	rm -rf bin/*

FORCE:
