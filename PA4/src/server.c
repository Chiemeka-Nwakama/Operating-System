#include "server.h"

#define PORT 5253
#define MAX_CLIENTS 5
#define BUFFER_SIZE 1024 


void *clientHandler(void *socket) {

    // Receive packets from the client
        // Open the file
    // Open the file for writing
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        perror("Error opening file");
        return -1;
    }

        // Receive response packet
    Packet recievedPacket;
    char recvdata[PACKETSZ];
    memset(recvdata, 0, PACKETSZ);
    ret = recv(socket, &recievedPacket, sizeof(Packet), 0);
     
    if(ret == -1) // error check
        perror("recv error");
   


    // Determine the packet operatation and flags

    // Receive the image data using the size

    // Process the image data based on the set of flags

    // Acknowledge the request and return the processed image data
}

int main(int argc, char* argv[]) {

    // Creating socket file descriptor
        int listen_fd, conn_fd;

    listen_fd = socket(AF_INET, SOCK_STREAM, 0); // create listening socket
    if(listen_fd == -1)
        perror("socket error");

    struct sockaddr_in servaddr;
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET; // IPv4
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY); // Listen to any of the network interface (INADDR_ANY)
    servaddr.sin_port = htons(PORT); // Port number


    

    // Bind the socket to the port

    int ret = bind(listen_fd, (struct sockaddr *) &servaddr, sizeof(servaddr)); // bind address, port to socket
    if(ret == -1)
        perror("bind error");




    // Listen on the socket

        ret = listen(listen_fd, BACKLOG); // listen on the listen_fd
    if(ret == -1)
        perror("listen error");

    // Accept connections and create the client handling threads
    
    struct sockaddr_in clientaddr;
    socklen_t clientaddr_len = sizeof(clientaddr);
    conn_fd = accept(listen_fd, (struct sockaddr *) &clientaddr, &clientaddr_len); // accept a request from a client
    if(conn_fd == -1)
        perror("accept error");


    // Release any resources
    
    // Server receiving packet from client
    char recvdata[PACKETSZ];
    memset(recvdata, 0, PACKETSZ);
    ret = recv(conn_fd, recvdata, PACKETSZ, 0); // receive data from client

    return 0;
}
