#include "server.h"

#define PORT 5253//change to id of who submits it
#define MAX_CLIENTS 5
#define BUFFER_SIZE 1024 

int i = 0;


void *clientHandler(void *socket) {
    // Receive packets from the client
    char recvdata[sizeof(packet_t)];
    memset(recvdata, 0 , sizeof(packet_t));
    int ret = recv(*((int*)socket), recvdata, sizeof(packet_t), 0);
    if(ret == -1)
        perror("recv error");

    // Determine the packet operatation and flags
    packet_t *recvpacket;
    recvpacket = deserializeData(recvdata);
  
    // Receive the image data using the size
    char recvimage[sizeof(packet_t)];
    memset(recvimage, 0 , sizeof(packet_t));
    int retimage = recv(*((int*)socket), recvimage, ntohl(recvpacket->size), 0);
    if(retimage == -1)
        perror("recv error");

    // Process the image data based on the set of flags
		/*
		Stbi_load takes:
		A file name, int pointer for width, height, and bpp
		*/
		int width, height;

        //Creates a file for the recieved image to put data in
        FILE *received_image;
        received_image = fopen("received_image.txt", "wb");//what type of file should I make this
        fprintf(received_image, recvimage);

        //Creates a file for the outputimage to send to client
        FILE *outputimage;
        outputimage = fopen("output_image.txt", "wb");//what type of file should I make this

		uint8_t* image_result = stbi_load(received_image, &width, &height, NULL, CHANNEL_NUM);//make recvimage a file
		uint8_t** result_matrix = (uint8_t **)malloc(sizeof(uint8_t*) * width);
		uint8_t** img_matrix = (uint8_t **)malloc(sizeof(uint8_t*) * width);

		for(int i = 0; i < width; i++){
			result_matrix[i] = (uint8_t *)malloc(sizeof(uint8_t) * height);
			img_matrix[i] = (uint8_t *)malloc(sizeof(uint8_t) * height);
		}
		/*
		linear_to_image takes:
		The image_result matrix from stbi_load
		An image matrix
		Width and height that were passed into stbi_load
		*/
		linear_to_image(image_result, img_matrix, width, height);

		//TODO: you should be ready to call flip_left_to_right or flip_upside_down depends on the angle(Should just be 180 or 270)
		//both take image matrix from linear_to_image, and result_matrix to store data, and width and height.
		//Hint figure out which function you will call.
		//flip_left_to_right(img_matrix, result_matrix, width, height); or flip_upside_down(img_matrix, result_matrix ,width, height);

		if (recvpacket->operation == 180) {
			flip_left_to_right(img_matrix, result_matrix, width, height);
		} 
		else if (recvpacket->operation == 270) {
			flip_upside_down(img_matrix, result_matrix, width, height);
		}
		uint8_t* img_array = (uint8_t*)malloc(sizeof(uint8_t) * width * height); ///Hint malloc using sizeof(uint8_t) * width * height
		//TODO: you should be ready to call flatten_mat function, using result_matrix
		//img_arry and width and height;
		flatten_mat(result_matrix, img_array, width, height);


		//TODO: You should be ready to call stbi_write_png using:
		//New path to where you wanna save the file,
		//Width
		//height
		//img_array
		//width*CHANNEL_NUM
		stbi_write_png(outputimage, width, height, CHANNEL_NUM, img_array, width*CHANNEL_NUM);//make outputimage into file
  
    // Acknowledge the request and return the processed image data
    packet_t packet;
    packet.operation = htonl(IMG_OP_ACK);
    
    char *serializedData = NULL;    
    serializedData = serializePacket(&packet);
    ret = send(*((int*)socket), serializedData, sizeof(packet_t), 0);//change to return of process image
    if(ret == -1)
    perror("send error");

    //send picture back to client
    char* buffer = (char*)malloc(BUFF_SIZE);
    int retclient;
    while((retclient = fread(buffer, 1, BUFF_SIZE, outputimage))){//is this the correct fread?

    }
    retclient = send(*((int*)socket), buffer, BUFF_SIZE, 0);
    if(retclient == -1){
        perror("send error");
    }
    fclose(outputimage);
    fclose(received_image);

  return;
}

int main(int argc, char* argv[]) {
       
    // Creating socket file descriptor
    int listen_fd;
    int conn_fd[10];
    listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    
    if(listen_fd == -1)
        perror("socket error");

    struct sockaddr_in servaddr;
    memset(&servaddr,0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);

    // Bind the socket to the port
    int ret = bind(listen_fd, (struct sockaddr *) &servaddr, sizeof(servaddr));
    if(ret == -1)
        perror("bind error");

    // Listen on the socket
    ret = listen(listen_fd, MAX_CLIENTS);
    if(ret == -1)
        perror("listen error");

    // Accept connections and create the client handling threads
    pthread_t thds[10];
    while(1){
    conn_fd[i] = accept(listen_fd, NULL, NULL);
    pthread_create(&thds[i], NULL, (void*) clientHandler, (void*) &conn_fd[i]);
    i++;
    }

    if(*conn_fd == -1)
        perror("accept error");
    // Release any resources
    close(*conn_fd);
    close(listen_fd);
    return 0;
}


