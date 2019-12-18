## Cipher Server

### Goal

Server and Client model using sockets.

*keys.txt*: Text file which contains the user's email addresses and their respective hashed passwords.

*Server*:
- It reads `keys.txt` file in free format and stores it in a HashMap
- Server establishes a socket a connection and binds the port you provide as STDIN
- Returns the hashed password corresponding to email address requested by client

*Client*:
- Input server address using STDIN
- Establish a connection to the server specified
- Input email address through STDIN
- Gets back hashed password from the server


### Compilation

You can use the Makefile to compile the project or typing out the commands.

*Makefile*

- `cd` into the project folder
- Type `make` in the terminal

*Commands*

- `cd` into the project directory
- To compile server: `g++ -Wall -Werror server/server.cpp -o bin/server.o`
- To compile client: `g++ -Wall -Werror client/client.cpp -o bin/client.o`


### Execution

- Open two terminals; one for server, another for client
- `cd` into the bin directory
-  Terminal-1
    ~~~
    ./server.o
    Please enter server port number: 9889
    <waiting for client>
    ~~~

- Terminal-2
    ~~~
    ./client.o
    Enter server host name: localhost
    Enter server port number: 9889
    Please enter the email address of a user: jhouston@uh.edu
    ~~~


- Client (terminal-2) will print: `The public key of jhouston@uh.edu is 0xcde34755210000444488884583861481397412558a84500f23470af`
- Server (terminal-1) will print: `Received request for public key of: jhouston@uh.edu`

- Multiple clients can request from the server at the same time (since server maintains a queue)
