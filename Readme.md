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
