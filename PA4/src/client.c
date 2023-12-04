#include "client.h"

#define PORT 5253
#define BUFFER_SIZE 1024 

int send_file(int socket, const char *filename) {
     // Open the file
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        return -1;
    }

    // Set up the request packet for the server and send it

    char buffer[BUFFER_SIZE];
    size_t bytesRead;

    while ((bytesRead = fread(buffer, 1, sizeof(buffer), file)) > 0) { //read file data into buffer until there is nothing left tosend
     
    }
    // Closes the file
    fclose(file);
    

    //  Packet packet; // makes packet variable
    //  packet.operation = htons(PROTO_REV); // calls htons proto revs
    //  strcpy(packet.data, buffer);  // copies buffer file data into packet data portion
    // // Serialize the packet, check common.h and sample/client.c
    // char *serializedData = serializePacket(&packet); // serializes the data
    // // send the serialized data to server
   
    ret = send(socket, buffer, bytesRead, 0); // sends data to server
    




    return 0;
}


int receive_file(int socket, const char *filename) {
    // Open the file
    // Open the file for writing
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        perror("Error opening file");
        return -1;
    }

        // Receive response packet
    Packet responsePacket;
    char recvdata[PACKETSZ];
    recv(socket, &responsePacket, sizeof(Packet), 0);
    memset(recvdata, 0, PACKETSZ);
   
    if(ret == -1) // error check
        perror("recv error");


    // // Check if the operation code is what you expect
    // if (responsePacket.operation != PROTO_REV) {
    //     fprintf(stderr, "Unexpected protocol revision\n");
    //     fclose(file);
    //     return -1;
    // }

    // Receive the file data
    char buffer[BUFFER_SIZE];
    ssize_t bytesRead;
    strcpy(buffer, responsePacket.data);  // copies packet data into buffer

    while (bytesRead = fwrite(buffer, 1, bytesRead, file); > 0) {
        // Write the received data to the file
        
    }

    // Close the file
    fclose(file);

    return 0;


    // Receive the file data

    // Write the data to the file
    return 0;
}

int main(int argc, char* argv[]) {
    if(argc != 3){
        fprintf(stderr, "Usage: ./client File_Path_to_images File_Path_to_output_dir Rotation_angle. \n");
        return 1;
    }
    
  




    // Set up socket

    int sockfd = socket(AF_INET, SOCK_STREAM, 0); // create socket to establish connection
    if(sockfd == -1)
        perror("socket error");

    // Connect the socket

    
    struct sockaddr_in servaddr;
    servaddr.sin_family = AF_INET; // IPv4
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1"); // server IP, since the server is on same machine, use localhost IP
    servaddr.sin_port = htons(PORT); // Port the server is listening on

    int ret = connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)); // establish connection to server
    if(ret == -1)
        perror("connect error");

    // Read the directory for all the images to rotate
        // Packet to send to server
    char msg[STRSZ];
    bzero(msg, STRSZ); // initialize msg with '\0'
    fscanf(stdin, "%[^\n]s", msg); ///o/ Read input with space until a newline
    setbuf(stdin, NULL);

    // Send the image data to the server

    

    // Check that the request was acknowledged

    // Receive the processed image and save it in the output dir

    // Terminate the connection once all images have been processed

    // Release any resources
    return 0;
}
