server: server.cpp keys20.txt
	g++ -Wall -Werror  server.cpp -o server
client: client.cpp
	g++ -Wall -Werror  client.cpp -o client
