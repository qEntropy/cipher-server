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
typedef unordered_map<string, string> hashMap;
typedef unordered_map<string, string>::const_iterator hashMapItr;

hashMap getEmailHashMap(string filename);
int getServerPortNumber();
int establishSocket(int portNum);
int doClientConnectionStuff(int sockfd, hashMap emailHashMap);

int main(int argc, char *argv[]) {

    hashMap emailHashMap = getEmailHashMap("keys20.txt");
    int portNumber = getServerPortNumber();
    int sockfd = establishSocket(portNumber);
    int clientConn =  doClientConnectionStuff(sockfd, emailHashMap);

    return 0;
}

/**

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

**/
int getServerPortNumber() {
    int portNumber = -1;
    do {
        cout << "Enter server port number: ";
    } while (cin >> portNumber && portNumber < 1024);
    return portNumber;
}

/**

**/
int establishSocket(int portNumber) {
    /**
    A socket file descriptor
    **/
    int sockfd = -1;

    /** [1]
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
    serverAddress.sin_addr.s_addr = INADDR_ANY; //TODO: check if this is correct
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
    socket() listening for clients
    **/
    if (listen(sockfd, 3) < 0) {
        perror("Error while listening");
    }
    return sockfd;
}

// TODO: change the name of the function
int doClientConnectionStuff(int sockfd, hashMap emailHashMap) {

    struct sockaddr_in cli_addr;
    int newsockfd, clilen;
    char buffer[1024];

    while(true) {
        clilen = sizeof(cli_addr);
        newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, (socklen_t *) &clilen);
        if (newsockfd < 0) {
             perror("ERROR on accept");
             exit(EXIT_FAILURE);
         }
        bzero(buffer, 1024);
        if (read(newsockfd, buffer, 1024) < 0) {
            perror("ERROR reading from socket");
            exit(EXIT_FAILURE);
        }

        // Converting c string to c++ string
        string cppBuffer(buffer);

        hashMapItr itr = emailHashMap.find(cppBuffer);
        if (itr != emailHashMap.end()) {
            string hashValue = itr->second;
            int hashValueSize = hashValue.size();
            if (write(newsockfd, hashValue.c_str(), hashValueSize) < 0) {
                perror("ERROR writing to the new socket");
            }
        }
        else {
            string errorMessage = "Could not find that email in the database";
            int length = errorMessage.size();
            if (write(newsockfd, errorMessage.c_str(), length) < 0) {
                perror("Error writing to the new socket");
            }
        }

        close(newsockfd);
    }
    return 0;
}


// string writeToSocket(const int socketfd, string emailOrKey) {
//
//     if (socketfd < 0) {
//          perror("Error while writing to socket");
//      }
//
// }

/** References
[] http://www.cs.rpi.edu/~moorthy/Courses/os98/Pgms/socket.html

**/
