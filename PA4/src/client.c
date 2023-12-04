#include "client.h"

#define PORT 5253
#define BUFFER_SIZE 1024
#include "image_rotation.h"

//Global integer to indicate the length of the queue??
int queue_length;
//Global integer to indicate the number of worker threads
int num_worker_threads;
//Global file pointer for writing to log file in worker??
FILE* log_file;
//Might be helpful to track the ID's of your threads in a global array
int worker_thread_id[MAX_THREADS];//change sizing method
//What kind of locks will you need to make everything thread safe? [Hint you need multiple]
pthread_mutex_t queue_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t queue_lock2 = PTHREAD_MUTEX_INITIALIZER;
//What kind of CVs will you need (i.e. queue full, queue empty) [Hint you need multiple]
pthread_cond_t queue_empty = PTHREAD_COND_INITIALIZER;
pthread_cond_t queue_full = PTHREAD_COND_INITIALIZER;
pthread_cond_t queue_done = PTHREAD_COND_INITIALIZER;
//How will you track the requests globally between threads? How will you ensure this is thread safe?
request_t main_queue[MAX_QUEUE_LEN];
char* request_queue[MAX_QUEUE_LEN];
//How will you track which index in the request queue to remove next?
int next_remove = 0;
//How will you update and utilize the current number of requests in the request queue?
int queue_size = 0;
int pics_changed = 0;

//How will you track the p_thread's that you create for workers?
pthread_t worker_threads[MAX_THREADS];//change sizing mechanism
//How will you know where to insert the next request received into the request queue?
int queue_index = 0;
int executed = 0;
char* output_directory;
char* input_directory;
int terminate_workers = 0;
int trav_complete = 0;


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
    


    // // send the serialized data to server

    //  Packet packet; // makes packet variable
    //  packet.operation = htons(PROTO_REV); // calls htons proto revs
    //  strcpy(packet.data, buffer);  // copies buffer file data into packet data portion
    // // Serialize the packet, check common.h and sample/client.c
    // char *serializedData = serializePacket(&packet); // serializes the data


    int ret;
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
    ret = recv(socket, &responsePacket, sizeof(Packet), 0);
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

    processing_args_t *procArgs = (processing_args_t *)args;
    const char* dirname = procArgs -> dirPath;
    DIR *dir = opendir(dirname);
    if(dir == NULL){
    perror("opendir");
    exit(1);
    }
    // Traverse through all entries
    struct dirent *entry;
    int next_pos_for_path = 0;
    while((entry = readdir(dir)) != NULL){
    // skip . and ..
    // concatenate dirname with the directory entry
    char* newEntry = entry->d_name;
    if(strcmp(newEntry, ".") == 0 ||strcmp(newEntry, "..") == 0){
    continue;
    }
    char buf[1024];
    sprintf(buf,"%s/%s",dirname,newEntry);
    pthread_mutex_lock(&queue_lock);
    while(queue_size >= MAX_QUEUE_LEN){
    pthread_cond_wait(&queue_full, &queue_lock);
    }

    // store pizza index i at next_pos_for_pizza location in pizza_order_stand and update the next position to store pizza
    //pargs -> //maLLOC HERE
    main_queue[queue_index].imgpaths = (char*)malloc(MAX_QUEUE_LEN * sizeof(char));
    memset(main_queue[queue_index].imgpaths,0,MAX_QUEUE_LEN * sizeof(char));
    strcpy(main_queue[queue_index].imgpaths,newEntry);
    main_queue[queue_index].angle_rot = procArgs -> angle_rot;
    next_pos_for_path = (next_pos_for_path + 1) % MAX_QUEUE_LEN;
    memset(buf,0,1024);

    // increment total number of pizza on stand by 1

    queue_size++;
    pics_changed++;
    queue_index = (queue_index + 1) % MAX_QUEUE_LEN;

    // signal consumer using cons_cond that one pizza is added to stand and unlock the stand

    pthread_cond_signal(&queue_empty);

    pthread_mutex_unlock(&queue_lock);


    // fprintf(stdout, "Producer added Pizza %d to stand\n", d_name);
    // fflush(stdout);

    }
    // fprintf(stdout, "Producer completed all orders, exiting...\n");
    // fflush(stdout);
    // if (main_queue[queue_index].imgpaths != NULL) {
    // free(main_queue[queue_index].imgpaths);
    // fprintf(stdout, "Consumer completed all orders, exiting...\n");
    // fflush(stdout);
    // }
    // close current directory
    //terminate_workers = 1;
    //int garbage;
    //if(garbage == 0){
    //terminate_workers = 1;
    closedir(dir);
    pthread_mutex_lock(&queue_lock2);
    //int executed = 0;
    while(queue_size >= 0 ){
    printf("We are here the queue size %d\n", queue_size);
    if(executed == pics_changed){
    //pthread_mutex_lock(&queue_lock2);
    trav_complete = 1;
    pthread_cond_broadcast(&queue_empty);
    pthread_mutex_unlock(&queue_lock2);
    pthread_exit(NULL);
    }
    pthread_cond_wait(&queue_full,&queue_lock2);
    //executed++;
    }
    // printf("We are here the queue size %d\n", queue_size);
    // if(executed == pics_changed){
    // pthread_mutex_lock(&queue_lock);
    // trav_complete = 1;
    // pthread_cond_broadcast(&queue_empty);
    // pthread_mutex_unlock(&queue_lock);
    // }
    // pthread_mutex_unlock(&queue_lock2);
    // printf("End of this\n");
    // pthread_exit(NULL);
    //}
  
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
