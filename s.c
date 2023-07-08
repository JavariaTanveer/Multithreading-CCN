#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>

#define MAXLINE 1024
#define SERVER_PORT 8080

struct data {
    int X;
    int Y;
};

struct clientData {
    struct data client1;
    struct data client2;
};

struct clientData cd1;

void* clientHandler(void* arg) {
    int connfd = *((int*)arg);
    struct data clientData;

    // Receive data from the client
    ssize_t n = recv(connfd, &clientData, sizeof(clientData), 0);
    if (n <= 0) {
        perror("Error receiving data from client");
        close(connfd);
        pthread_exit(NULL);
    }

    if (cd1.client1.X == 0) {
        cd1.client1 = clientData;
    } else {
        cd1.client2 = clientData;

        // Perform calculations
        int sum = cd1.client1.X + cd1.client2.Y;
        double division = (double)cd1.client1.X / cd1.client2.Y;

        // Display the calculation message on the server side
        printf("Calculation: Addition: %d, Division: %.2f\n", sum, division);
    }

    close(connfd);
    pthread_exit(NULL);
}

int main() {
    int sockfd;
    struct sockaddr_in servaddr, cliaddr;
  socklen_t len;
	
    // Create listening TCP socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr));

    // Filling server information
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(SERVER_PORT);

    // Bind server address to the listening socket
    if (bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0) {
        perror("Binding failed");
        exit(EXIT_FAILURE);
    }

    // Start listening for incoming connections
    if (listen(sockfd, 10) < 0) {
        perror("Listening failed");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d\n", SERVER_PORT);

    while (1) {
        len = sizeof(cliaddr);
        // Accept a new client connection
        int afd = accept(sockfd, (struct sockaddr*)&cliaddr, &len);
        if (afd < 0) {
            perror("Accepting connection failed");
            exit(EXIT_FAILURE);
        }
        // Create a new thread to handle the client
        pthread_t tid;
        pthread_create(&tid, NULL, clientHandler, &afd);
        // Detach the thread to clean up its resources automatically
        pthread_detach(tid);
    }
    // Close the listening socket
    close(sockfd);
    return 0;
}

