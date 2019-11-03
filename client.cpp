#include <iostream>
#include <string>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>

using namespace std;

int getServerPortNumber();
string getServerHostName();

int main(int argc, char *argv[]) {
    int portNumber = getServerPortNumber();
    string hostName = getServerHostName();

    int sockfd, portno, n;

    struct sockaddr_in serv_addr;
    struct hostent *server;

    char buffer[256];
    portno = portNumber;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        perror("ERROR opening socket");
    server = gethostbyname(hostName.c_str());
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0)
        perror("ERROR connecting");
    printf("Please enter the email: ");
    bzero(buffer,256);
    cin >> buffer;
    n = write(sockfd,buffer,strlen(buffer));
    if (n < 0)
         perror("ERROR writing to socket");
    bzero(buffer,256);
    n = read(sockfd,buffer,255);
    if (n < 0)
         perror("ERROR reading from socket");
    printf("%s\n",buffer);
    return 0;
}

/**
    prompts the "user" to enter the server's port number
    to which the client wishes to make connection to
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

string getServerHostName() {
    string hostName = "";
    cout << "Enter server host name: ";
    cin >> hostName;
    return hostName;
}
