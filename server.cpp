#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <strings.h>
#include <fcntl.h>
#include <unistd.h>

using namespace std;

int getServerPortNumber();

int main(int argc, char *argv[]) {

    int portNumber = getServerPortNumber();
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
    cli_addr: address of the client which will connect to this server
    **/
    struct sockaddr_in serverAddress, cli_addr;
    bzero((char *) &serverAddress, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(portNumber);

    if (bind(sockfd, (struct sockaddr *) &serverAddress, sizeof(serverAddress)) < 0)
        perror("ERROR on binding");
    listen(sockfd,5);



    //-------------------RPI-SERVER------------------------//
    int newsockfd, clilen;
    char buffer[256];
    int n;


    clilen = sizeof(cli_addr);
    newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, (socklen_t *) &clilen);
    if (newsockfd < 0)
         perror("ERROR on accept");
    bzero(buffer,256);
    n = read(newsockfd,buffer,255);
    if (n < 0) perror("ERROR reading from socket");
    printf("Here is the message: %s\n",buffer);
    n = write(newsockfd,"I got your message",18);
    if (n < 0) perror("ERROR writing to socket");
    //-------------------RPI-SERVER------------------------//

    return 0;
}

int getServerPortNumber() {
    int portNumber = -1;
    do {
        cout << "Enter server port number: ";
    } while (cin >> portNumber && portNumber < 1024);
    return portNumber;
}

/** References
[1] http://www.cs.rpi.edu/~moorthy/Courses/os98/Pgms/socket.html

**/
