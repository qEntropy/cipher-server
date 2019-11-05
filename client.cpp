#include <iostream>
#include <string>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>

using namespace std;
#define MAX_BUFFER 1024

int getServerPortNumber();
string getServerHostName();
int createServerConnection(int portNumber, string hostName);


int main(int argc, char *argv[]) {

    int portNumber = getServerPortNumber();
    string hostName = getServerHostName();
    string email = "";
    cout << "Please enter the email: ";
    cin >> email;
    int socketfd = createServerConnection(portNumber, hostName);

    char keyBuffer[MAX_BUFFER];

    if(write(socketfd, email.c_str(), email.size()) < 0) {
        perror("Error writing to socket");
        exit(EXIT_FAILURE);
    }

    bzero(keyBuffer, MAX_BUFFER);
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
    if (portNumber < 1024) {
        perror("Please enter a port number greater than 1024");
        exit(1);
    }
    else if (portNumber > 60000) {
        perror("Please enter a port number less than 60000");
        exit(1);
    }
    return portNumber;
}

/**
    prompts the "user" get the server's name
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
    creates a connection to the server
    if the connection is successful
    @returns socket file descriptor for you to read and write to
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

    /**
    set everything in the serverAddress to zero for now
    **/
    bzero((char *) &serverAddress, sizeof(serverAddress));

    /**
    fill the
    .sin_family field of the serverAddress = AF_INET (Internet domain)
    .sin_addr.s_addr field copy from the serverInfo
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
        perror("ERROR connecting");
        exit(0);
    }

    return socketfd;
}

/** References
[] http://www.cs.rpi.edu/~moorthy/Courses/os98/Pgms/socket.html
**/
