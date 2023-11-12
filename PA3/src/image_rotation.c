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
//What kind of CVs will you need  (i.e. queue full, queue empty) [Hint you need multiple]
pthread_cond_t queue_empty = PTHREAD_COND_INITIALIZER;
pthread_cond_t queue_full = PTHREAD_COND_INITIALIZER;
//How will you track the requests globally between threads? How will you ensure this is thread safe?
request_t main_queue[MAX_QUEUE_LEN];
char* request_queue[MAX_QUEUE_LEN];
//How will you track which index in the request queue to remove next?
int next_remove = 0;
//How will you update and utilize the current number of requests in the request queue?
int queue_size = 0;
//How will you track the p_thread's that you create for workers?
pthread_t worker_threads[MAX_THREADS];//change sizing mechanism
//How will you know where to insert the next request received into the request queue?
int queue_index = 0; 
char* output_directory;
char* input_directory;
int terminate_workers = 0;

/*
    The Function takes:
    to_write: A file pointer of where to write the logs. 
    requestNumber: the request number that the thread just finished.
    file_name: the name of the file that just got processed. 

    The function output: 
    it should output the threadId, requestNumber, file_name into the logfile and stdout.
*/
void log_pretty_print(FILE* to_write, int threadId, int requestNumber, char * file_name){
   fprintf(to_write, "[%d][%d][%s]\n", threadId, requestNumber, file_name);
    fprintf(stdout,"[%d][%d][%s]\n", threadId, requestNumber, file_name);
    fflush(stdout);
}


/*

    1: The processing function takes a void* argument called args. It is expected to be a pointer to a structure processing_args_t 
    that contains information necessary for processing.

    2: The processing thread need to traverse a given dictionary and add its files into the shared queue while maintaining synchronization using lock and unlock. 

    3: The processing thread should pthread_cond_signal/broadcast once it finish the traversing to wake the worker up from their wait.

    4: The processing thread will block(pthread_cond_wait) for a condition variable until the workers are done with the processing of the requests and the queue is empty.

    5: The processing thread will cross check if the condition from step 4 is met and it will signal to the worker to exit and it will exit.

*/

void *processing(void *args){
    printf("Beginning of processing\n");
    processing_args_t *procArgs  = (processing_args_t *)args;
    const char* dirname = procArgs -> dirPath;
    printf("It's a me %s\n",dirname);
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
        printf("His child %s\n",newEntry);
        if(strcmp(newEntry, ".") == 0 ||strcmp(newEntry, "..")  == 0){
            continue;
        }
        char buf[1024];
        sprintf(buf,"%s/%s",dirname,newEntry);
        printf("His grandchild %s\n",buf);
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
	   printf("The black sheep %s\n",main_queue[queue_index].imgpaths);
	   printf("The order of ascension %d\n",queue_index);
        // increment total number of pizza on stand by 1

        queue_size++;
        queue_index = (queue_index + 1) % MAX_QUEUE_LEN;
        

        // signal consumer using cons_cond that one pizza is added to stand and unlock the stand

        pthread_cond_signal(&queue_empty);

        pthread_mutex_unlock(&queue_lock);

        

//         fprintf(stdout, "Producer added Pizza %d to stand\n", d_name);
//         fflush(stdout);

    }
    
//     fprintf(stdout, "Producer completed all orders, exiting...\n");
//     fflush(stdout);
            if (main_queue[queue_index].imgpaths != NULL) {
                free(main_queue[queue_index].imgpaths);
                fprintf(stdout, "Consumer completed all orders, exiting...\n");
                fflush(stdout);
                }
    
    // close current directory
    terminate_workers = 1;
    pthread_cond_broadcast(&queue_empty);
    closedir(dir);
    
    
    printf("End of processing\n");
    pthread_mutex_unlock(&queue_lock);
    pthread_exit(NULL);
}

/*
    1: The worker threads takes an int ID as a parameter

    2: The Worker thread will block(pthread_cond_wait) for a condition variable that there is a requests in the queue. 

    3: The Worker threads will also block(pthread_cond_wait) once the queue is empty and wait for a signal to either exit or do work.

    4: The Worker thread will processes request from the queue while maintaining synchronization using lock and unlock. 

    5: The worker thread will write the data back to the given output dir as passed in main. 

    6:  The Worker thread will log the request from the queue while maintaining synchronization using lock and unlock.  

    8: Hint the worker thread should be in a While(1) loop since a worker thread can process multiple requests and It will have two while loops in total
        that is just a recommendation feel free to implement it your way :) 
    9: You may need different lock depending on the job.  

*/


void * worker(void *args){
    printf("Beginning of worker\n");
    int threadId = *((int*)args);
    while (1) {
        pthread_mutex_lock(&queue_lock2);
        while (queue_size == 0 && !terminate_workers) {
            printf("Queue size: %d\n",queue_size);
            pthread_cond_wait(&queue_empty, &queue_lock2);
            printf("Queue size: %d\n",queue_size);
        }
        
        if(terminate_workers){
            pthread_mutex_unlock(&queue_lock2);
            break;
        }
        char* file_name = main_queue[next_remove].imgpaths;
        printf("%syeayea\n",main_queue[next_remove].imgpaths);
        int angle = main_queue[next_remove].angle_rot;
        queue_size--;
        printf("The order of descension %d\n",next_remove);
        next_remove = (next_remove + 1) % MAX_QUEUE_LEN;

        pthread_cond_signal(&queue_full);
        pthread_mutex_unlock(&queue_lock2);

        /*
            Stbi_load takes:
                A file name, int pointer for width, height, and bpp

        */
        
        int width, height;
        char log_path[1024];
        sprintf(log_path, "%s/%s", input_directory, file_name);
        uint8_t* image_result = stbi_load(log_path, &width, &height, NULL, CHANNEL_NUM);
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
        

        ////TODO: you should be ready to call flip_left_to_right or flip_upside_down depends on the angle(Should just be 180 or 270)
        //both take image matrix from linear_to_image, and result_matrix to store data, and width and height.
        //Hint figure out which function you will call. 
        //flip_left_to_right(img_matrix, result_matrix, width, height); or flip_upside_down(img_matrix, result_matrix ,width, height);
        if (angle == 180) {
            flip_left_to_right(img_matrix, result_matrix, width, height);
        } else if (angle == 270) {
            flip_upside_down(img_matrix, result_matrix, width, height);
        }

        
        
        uint8_t* img_array = (uint8_t*)malloc(sizeof(uint8_t) * width * height); ///Hint malloc using sizeof(uint8_t) * width * height
    

        ///TODO: you should be ready to call flatten_mat function, using result_matrix
        //img_arry and width and height; 
        flatten_mat(result_matrix, img_array, width, height);


        ///TODO: You should be ready to call stbi_write_png using:
        //New path to where you wanna save the file,
        //Width
        //height
        //img_array
        //width*CHANNEL_NUM
        
        
        
        
        
        
        
        
        printf("happy to be here\n");
        fflush(stdout);
        char output_path[1024];
        sprintf(output_path, "%s/%s", output_directory, file_name);

        printf("Hot one in the mess %s\n",output_path);
        stbi_write_png(output_path, width, height, CHANNEL_NUM, img_array, width*CHANNEL_NUM);
        pthread_mutex_lock(&queue_lock2);
        log_pretty_print(log_file, threadId, next_remove, log_path);
        pthread_mutex_unlock(&queue_lock2);
    }
    printf("Ending of worker\n");
}

/*
    Main:
        Get the data you need from the command line argument 
        Open the logfile
        Create the threads needed
        Join on the created threads
        Clean any data if needed. 


*/

int main(int argc, char* argv[]){
    if(argc != 5)
    {
        fprintf(stderr, "Usage: File Path to image dirctory, File path to output dirctory, number of worker thread, and Rotation angle\n");
    }
    printf("original0\n");
    input_directory = malloc(sizeof(char)*BUFF_SIZE);
    output_directory = malloc(sizeof(char)*BUFF_SIZE);
    strcpy(input_directory,argv[1]);    
    strcpy(output_directory,argv[2]);
    int num_worker_threads = atoi(argv[3]);
    int rotation_angle = atoi(argv[4]);

    char fname[1024];
    sprintf(fname,"%s.txt",LOG_FILE_NAME);
    log_file = fopen(fname,"w");
    if (log_file == NULL) {
        perror("Failed to open request_log file");
        exit(1);
    }
    pthread_t processing_thread;
    struct processing_args pargs = {input_directory, num_worker_threads, rotation_angle};

    pthread_create(&processing_thread, NULL, (void*)processing, &pargs);
    int wthreads[MAX_THREADS];
    for (int i = 0; i < num_worker_threads; i++){
        wthreads[i] = i;
        pthread_create(&worker_threads[i], NULL, (void*)worker, &wthreads[i]);
    }
    pthread_join(processing_thread, NULL);
    for (int i = 0; i < num_worker_threads; i++) {
        pthread_join(worker_threads[i], NULL);
    }
    free(output_directory);
    free(input_directory);
    fclose(log_file);
    return 0;

}
