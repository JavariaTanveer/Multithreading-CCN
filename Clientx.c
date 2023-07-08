#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define MAXLINE 1024
#define SERVER_PORT 8080

struct data {
    int X;
    int Y;
};

int main() {
    int sockfd;
    struct sockaddr_in servaddr;
    struct data clientData;

    // Create a TCP socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Failed to create socket");
        exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr));

    // Set server information
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERVER_PORT);
        servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

    // Convert IPv4 addresses from text to binary form
  //  if (inet_pton(AF_INET, "127.0.0.1", &servaddr.sin_addr) <= 0) {
    //    perror("Invalid address/Address not supported");
      //  exit(EXIT_FAILURE);
    //}

    // Connect to the server
    if (connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0) {
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }

    // Read the values of X and Y from the user
    printf("Enter the value of X: ");
    scanf("%d", &clientData.X);

    // Set the value of Y as 0 for client1
    clientData.Y = 0;

    // Send the data to the server
    ssize_t n = send(sockfd, &clientData, sizeof(clientData), 0);
    if (n < 0) {
        perror("Error sending data to server");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    printf("Data sent to server successfully\n");

    // Close the socket
    close(sockfd);

    return 0;
}

