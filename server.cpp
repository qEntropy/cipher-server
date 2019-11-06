#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <strings.h>
#include <fcntl.h>
#include <unistd.h>

using namespace std;
#define MAX_BUFFER 1024
typedef unordered_map<string, string> hashMap;
typedef unordered_map<string, string>::const_iterator hashMapItr;

hashMap getEmailHashMap(string filename);
int getServerPortNumber();
int establishSocket(int portNum);
int connectClientAndWrite(int sockfd, hashMap emailHashMap);

int main(int argc, char *argv[]) {

    // read file and return a unordered_map {string--->string}
    hashMap emailHashMap = getEmailHashMap("keys20.txt");

    // prompt user for port number
    int portNumber = getServerPortNumber();

    // establishes a socket connection and binds it to the server
    int sockfd = establishSocket(portNumber);
    if (sockfd < 0) {
        perror("Error: Establishing a socket failed");
        exit(EXIT_FAILURE);
    }

    // (loop) part where we connect and write to clients
    int clientConn =  connectClientAndWrite(sockfd, emailHashMap);
    if (clientConn < 0) {
        perror("Error: Connection to client failed");
        exit(EXIT_FAILURE);
    }

    return 0;
}

/**
    @filename: name of the file to read
    @returns: unordered_map<string, string>
             {email-->public_key}
**/
hashMap getEmailHashMap(string filename) {
    hashMap emailHashMap;
    ifstream file;
    file.open(filename);
    vector<string> vectorPair;
    if (!file.is_open()) {
        perror("Error opening file");
        return emailHashMap;
    }

    string email, hash;
    while (file >> email) {
        file >> hash;
        emailHashMap.insert(make_pair(email, hash));
    }
    file.close();
    return emailHashMap;
}

/**
    gets the port number from the user
    if port is between 1024 and 60000: success
    @returns: port number entered by user
**/
int getServerPortNumber() {
    int portNumber = -1;
    cout << "Please enter server port number: ";
    cin >> portNumber;
    if (portNumber < 1024) {
        perror("Please enter a port number greater than 1024");
        exit(EXIT_FAILURE);
    }
    else if (portNumber > 60000) {
        perror("Please enter a port number less than 60000");
        exit(EXIT_FAILURE);
    }
    return portNumber;
}

/**
    server's socket in the listening state
    @portNumber: port number entered by user
    @returns:  socket file desriptor binded to server
**/
int establishSocket(int portNumber) {
    /**
    A socket file descriptor
    **/
    int sockfd = -1;

    /**
    AF_INET: to specify that we're using Internet address domain
    SOCK_STREAM: to specify we're using stream socket (TCP)
    0: OS uses this to use the most appropriate protocol (TCP)
    **/
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Error while opening socket");
        return -1;
    }


    /**
    serverAddress: Address of the server
    **/
    struct sockaddr_in serverAddress;
    bzero((char *) &serverAddress, sizeof(serverAddress)); // TODO: mem-set

    // address domain family i.e. AF_INET
    serverAddress.sin_family = AF_INET;
    // IP address of the server
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    // converting integer to network byte order (short)
    serverAddress.sin_port = htons(portNumber);

    /**
    binding the socket (sockfd) with the server address
    **/
    if (bind(sockfd, (struct sockaddr *) &serverAddress, sizeof(serverAddress)) < 0) {
        close(sockfd);
        perror("ERROR on binding");
    }

    /**
    socket() listening for clients, max in queue = 3
    **/
    if (listen(sockfd, 3) < 0) {
        perror("Error while listening");
    }
    return sockfd;
}


/**
    @sockfd: socket file descriptor os server
    @emailHashMap:[map of] email --> key
    @returns: 0 if everything goes smoothly
**/
int connectClientAndWrite(int sockfd, hashMap emailHashMap) {

    struct sockaddr_in clientAddress;
    int newsockfd, clilen;
    char emailBuffer[MAX_BUFFER];

    while(true) {
        clilen = sizeof(clientAddress);
        /*
            returns a new socket file descriptor (of client)
            all the read and writes will be done on this newsockfd
        */
        newsockfd = accept(sockfd, (struct sockaddr *) &clientAddress, (socklen_t *) &clilen);
        if (newsockfd < 0) {
             perror("Error on accept");
             exit(EXIT_FAILURE);
         }
         /*
            empty the buffer in which we will
            read the email address received from client
         */
        bzero(emailBuffer, MAX_BUFFER);
        if (read(newsockfd, emailBuffer, MAX_BUFFER) < 0) {
            perror("ERROR reading from socket");
            exit(EXIT_FAILURE);
        }

        /* Converting c string to c++ string */
        string cppBuffer(emailBuffer);
        cout << "Received request for public key of: " << cppBuffer << endl;

        /*  search for the email in the email-Key-HashMap */
        hashMapItr itr = emailHashMap.find(cppBuffer);

        /* if email found then write its 'key' to the client's socket fd*/
        if (itr != emailHashMap.end()) {
            string prompt = "The public key of ";
            string hashValue = itr->second;
            string writingString = prompt + cppBuffer + " is " + hashValue;
            int writingStringSize = writingString.size();
            if (write(newsockfd, writingString.c_str(), writingStringSize) < 0) {
                perror("ERROR writing to the new socket");
            }
        }
        /* if email not found then write an error message */
        else {
            string errorMessage = "The database had no public key for user ";
            string writingString = errorMessage + cppBuffer;
            int length = writingString.size();
            if (write(newsockfd, writingString.c_str(), length) < 0) {
                perror("Error writing to the new socket");
            }
        }

        /* close the socket file descriptor once all the read/wrties are over */
        close(newsockfd);
    }

    return 0;
}


/** References
[] http://www.cs.rpi.edu/~moorthy/Courses/os98/Pgms/socket.html
[] https://stackoverflow.com/questions/20372661/read-word-by-word-from-file-in-c
**/
