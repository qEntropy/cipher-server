#include <iostream>
#include <string>
#include <stdio.h>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>

using namespace std;
#define MAX_BUFFER 1024
#define MIN_PORT_NUM 1023
#define MAX_PORT_NUM 60000

string getServerHostName();
int getServerPortNumber();
string getEmailAddress();
int createServerConnection(int portNumber, string hostName);


int main(int argc, char *argv[]) {

    // get hostname ("localhost") from the user
    string hostName = getServerHostName();

    // get portNumber from the user ("9999")
    int portNumber = getServerPortNumber();

    // get the email address from the user ("granch@uh.edu")
    string email = getEmailAddress();

    // connect to the server and return socket file descriptor
    int socketfd = createServerConnection(portNumber, hostName);

    // fixed-size buffer to read the public key requested
    char keyBuffer[MAX_BUFFER];

    // write email to the socket file descriptor
    if(write(socketfd, email.c_str(), email.size()) < 0) {
        perror("Error writing to socket");
        exit(EXIT_FAILURE);
    }

    bzero(keyBuffer, MAX_BUFFER); // empty the fixed size buffer

    // read the public key sent by the server into the buffer
    if (read(socketfd, keyBuffer, MAX_BUFFER) < 0) {
         perror("Error reading from socket");
         exit(EXIT_FAILURE);
     }

    cout << keyBuffer << endl;
    close(socketfd);
    return 0;
}


/**
    prompts the "user" to enter the server's port number
    to which the client wishes to make connection with
    @ returns the entered port number
**/
int getServerPortNumber() {
    int portNumber = -1;
    cout << "Enter server port number: ";
    cin >> portNumber;
    if (portNumber < MIN_PORT_NUM) {
        perror("Please enter a port number greater than or equal to 1024");
        exit(EXIT_FAILURE);
    }
    else if (portNumber > MAX_PORT_NUM) {
        perror("Please enter a port number less than 60000");
        exit(EXIT_FAILURE);
    }
    return portNumber;
}

/**
    prompts the "user" to get the server's name
    in our case it is localhost
    @returns the entered hostname
**/
string getServerHostName() {
    string hostName = "";
    cout << "Enter server host name: ";
    cin >> hostName;
    return hostName;
}

/**
    prompts user for to enter the email address
    for which he/she needs the public key
    @returns the email address entered by the user
**/
string getEmailAddress() {
    string emailAddress = "";
    cout << "Please enter the email address of a user: ";
    cin >> emailAddress;
    return emailAddress;
}

/**
    creates a connection to the server
    if the connection is successful
    @returns socket file descriptor for you to read and write
    @portNumber: port number of server to which client wishes to connect
    @hostName: localhost or your private IP address for this code
**/
int createServerConnection(int portNumber, string hostName) {

    /**
    Address of the server client wishes to connect
    **/
    struct sockaddr_in serverAddress;
    /**
    defines a host computer on the internet
    has a list of address from name server
    first element of which is aliased to 'h_addr'
    **/
    struct hostent *serverInfo;

    /**
    create a socket for client, which we will connect
    to the server using 'serverAddress' and 'connect'
    **/
    int socketfd = socket(AF_INET, SOCK_STREAM, 0);
    if (socketfd < 0) {
        perror("ERROR opening socket");
        return -1;
    }

    /**
    gethostbyname(char* IPv4): takes IPv4 address and
    returns a pointer to hostnet which has the info on server
    (host) of that IP address
    **/
    serverInfo = gethostbyname(hostName.c_str());
    if (serverInfo == NULL) {
        perror("Error getting hostname");
        exit(EXIT_FAILURE);
    }

    /**
    set everything in the serverAddress to zero for now
    **/
    bzero((char *) &serverAddress, sizeof(serverAddress));

    /**
    fill the
    .sin_family field of the serverAddress = AF_INET (Internet domain)
    .sin_addr.s_addr field copy from the 'serverInfo' struct
    .sin_port = fill portNumber from the user
    **/
    serverAddress.sin_family = AF_INET;
    memcpy((char *)&serverAddress.sin_addr.s_addr,
    (char *)serverInfo->h_addr,
    serverInfo->h_length);
    serverAddress.sin_port = htons(portNumber);

    /**
    connect to the server using socketfd
    if not successfull, then exit the program and give an error
    but if connected then just return socket file descriptor
    **/
    if (connect(socketfd,(struct sockaddr *)&serverAddress,sizeof(serverAddress)) < 0) {
        perror("Error connecting to the server");
        exit(EXIT_FAILURE);
    }

    return socketfd;
}

/** References
[] http://www.cs.rpi.edu/~moorthy/Courses/os98/Pgms/socket.html
[] https://stackoverflow.com/questions/33026204/segmentation-fault-when-using-gethostbyname
[] https://en.cppreference.com/w/cpp/string/byte/memcpy
[] http://man7.org/linux/man-pages/man3/memcpy.3.html
**/
