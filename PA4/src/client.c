#include "client.h"



#define PORT 4232
#define BUFFER_SIZE 1024


//Global integer to indicate the length of the queue??
int queue_length;
//Global integer to indicate the number of worker threads
int num_worker_threads;
//Global file pointer for writing to log file in worker??
FILE* log_file;
//Might be helpful to track the ID's of your threads in a global array

request_t main_queue[MAX_QUEUE_LEN];
char* request_queue[MAX_QUEUE_LEN];
//How will you track which index in the request queue to remove next?
int next_remove = 0;
//How will you update and utilize the current number of requests in the request queue?
int queue_size = 0;
int pics_changed = 0;

//How will you track the p_thread's that you create for workers?

//How will you know where to insert the next request received into the request queue?
int queue_index = 0;
int executed = 0;
char* output_directory;
char* input_directory;
int terminate_workers = 0;
int trav_complete = 0;


int send_file(int socket, const char *filename) {
      // Open the file
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        perror("Error opening file");
        return -1;
    }


    char* buffer = (char*)malloc(BUFF_SIZE);
    int ret;
    while ((ret = fread(buffer, 1, BUFF_SIZE, file)) > 0) {
       

    }

     ret = send(socket, buffer, sizeof(buffer), 0); //sends file data
     if (ret == -1) {
        perror("send error");
        
    }
    // Closes the file
    fclose(file);
    return 0;
}


int receive_file(int socket, const char *filename) {
    // Open the file
    // Open the file for writing
    FILE *file = fopen(filename, "wb");
    if (file == NULL) {
        perror("Error opening file");
        return -1;
    }

        

    // Check that the request was acknowledged
    
    char recvdata [sizeof(packet_t)];
    memset(recvdata, 0,  sizeof(packet_t));
    int ret = recv(socket, &recvdata, sizeof(packet_t), 0); // receive data from server
    if(ret == -1)
        perror("recv error");

    

    // Deserialize the received data, check common.h and sample/client.c
    packet_t *ackpacket = NULL;
    ackpacket = deserializeData(recvdata);
    if(ackpacket->operation == IMG_OP_ACK){//if operation is acknowlegement recieve data
        receive_file(socket, filename); // recieves file writes modified image to output directory
    }




    char* buffer = (char*)malloc(ntohl(ackpacket->size));
    ret = recv(socket, buffer, sizeof(BUFF_SIZE), 0);


    //deserialize

   
    if(ret == -1) // error check
        perror("recv error");



    // Receive the file data
 
    ssize_t bytesRead;
   
    while ((bytesRead = fwrite(buffer, 1, bytesRead, file)) > 0) {
        // Write the received data to the file
        fwrite(buffer, 1, bytesRead, file);
    }

  


    // Close the file
    fclose(file);
    
    free(ackpacket);
    ackpacket = NULL;    
    
    


    // Receive the file data

    // Write the data to the file
    return 0;
}

int main(int argc, char* argv[]) {
    if(argc != 4){
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
    if(ret == -1){
        printf("here\n");


        perror("connect error");
    }

    // Read the directory for all the images to rotate

    const char* dirname = argv[1]; // input directory argv 1
    DIR *dir = opendir(dirname);
    if(dir == NULL){
	    perror("opendir");
	    exit(1);
    }
    // Traverse through all entries
    struct dirent *entry;
    while((entry = readdir(dir)) != NULL){
		// skip . and ..
		// concatenate dirname with the directory entry
		char* newEntry = entry->d_name;
		if(strcmp(newEntry, ".") == 0 ||strcmp(newEntry, "..") == 0){
			continue;
		}
		char buf[1024];
		sprintf(buf,"%s/%s",dirname,newEntry);


        main_queue[queue_index].file_name  = (char*)malloc(BUFF_SIZE * sizeof(char));

		memset(main_queue[queue_index].file_name ,0,BUFF_SIZE* sizeof(char));

		strcpy(main_queue[queue_index].file_name ,newEntry);










	  
		main_queue[queue_index].rotation_angle = atoi(argv[3]);
		memset(buf,0,1024);

		// increment total  stand by 1

		queue_size++;
		queue_index = (queue_index + 1);
	}
    closedir(dir);
    while(next_remove < queue_size){
    
    // Fill the content of packet, check sample/client.c
     packet_t packet; 
    // gives operation
     packet.operation = IMG_OP_ROTATE;


    
     if( main_queue[next_remove].rotation_angle == 180){

        packet.flags = IMG_FLAG_ROTATE_180 ;

     }
     else if(main_queue[next_remove].rotation_angle == 270){

        packet.flags = IMG_FLAG_ROTATE_270 ;
     }

     

      FILE *fp = fopen(main_queue[next_remove].file_name, "rb"); // opens file
      //printf("%s\n", main_queue[next_remove].file_name);

    if (fp==NULL) //checks for error
        return -1;

    if (fseek(fp, 0, SEEK_END) < 0) { // fseeks to end of file
        fclose(fp);
        return -1;
    }


    int file_size = ftell(fp); // gets the size of the file
    fclose(fp);
    packet.size = htonl(file_size);


    
    // Serialize the packet, check common.h and sample/client.c, we don't want to serialize with this function because it doesn't exist
    char *serializedData = serializePacket(&packet);
    // send the serialized data to server
    //after sending the packet with all the request information, send the image data
    int ret = send(sockfd, serializedData, sizeof(packet_t), 0);
     if(ret == -1)
        perror("send error");


    send_file(sockfd, main_queue[next_remove].file_name); //sends file data after packet with operation info is  sent




    next_remove = (next_remove + 1); // goes to next thing in queue


    // Receive the processed image and save it in the output dir


    // Terminate the connection once all images have been processed

    // Release any resources


    receive_file(sockfd, argv[2]);

    

    
    }
    close(sockfd); // close socket
    
return 0;

}
