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

unordered_map<string, string> getEmailHashMap(string filename);
int getServerPortNumber();
int establishSocket(int portNum);
int doClientConnectionStuff(int sockfd, hashMap emailHashMap);

int main(int argc, char *argv[]) {

    unordered_map<string, string> emailHashMap = getEmailHashMap("keys20.txt");
    int portNumber = getServerPortNumber();
    int sockfd = establishSocket(portNumber);

    // TODO: to implement from scratch
    int clientConn =  doClientConnectionStuff(sockfd, emailHashMap);

    return 0;
}

/**

**/
hashMap getEmailHashMap(string filename) {
    unordered_map<string, string> emailHashMap;
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
        cerr << "Error while opening socket\n";
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
    if (listen(sockfd, 5) < 0) {
        perror("Error while listening");
    }
    return sockfd;
}

// TODO: to implement from scratch
int doClientConnectionStuff(int sockfd, hashMap emailHashMap) {

    //-------------------RPI------------------------//
    struct sockaddr_in cli_addr;
    int newsockfd, clilen;
    char buffer[256];
    int n;

    // TODO: all this should be in a while(1) loop
    clilen = sizeof(cli_addr);
    newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, (socklen_t *) &clilen);
    if (newsockfd < 0)
         perror("ERROR on accept");
    bzero(buffer, 256);
    n = read(newsockfd, buffer, 256);
    if (n < 0) perror("ERROR reading from socket");

    // for (int i = 0; i < 14; i++) {
    //     cout << i << " " << buffer[i] << endl;
    // }
    // buffer[strlen(buffer) - 1] = '\0';

    string cppBuffer(buffer);
    string myBuffer = cppBuffer.substr(0, cppBuffer.size()-1);

    // for (int i = 0; i < myBuffer.size(); ++i) {
    //     std::cout << i << " " << myBuffer[i] << endl;
    // }

    hashMapItr itr = emailHashMap.find(myBuffer);
    if (itr != emailHashMap.end()) {
        cout << "found the hashValue: " << itr->second << endl;
    }
    else {
        cout << "could NOT find :" << myBuffer << endl;
    }

    if(write(newsockfd,"I got your message",18) < 0) {
        perror("ERROR writing to socket");
    }

    return 0;
    //-------------------RPI------------------------//
}

/** References
[] http://www.cs.rpi.edu/~moorthy/Courses/os98/Pgms/socket.html

**/
