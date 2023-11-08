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
pthread_mutex_t queue_lock3 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t queue_lock4 = PTHREAD_MUTEX_INITIALIZER;
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
}


/*
    1: The processing function takes a void* argument called args. It is expected to be a pointer to a structure processing_args_t 
    that contains information necessary for processing.
    2: The processing thread need to traverse a given dictionary and add its files into the shared queue while maintaining synchronization using lock and unlock. 
    3: The processing thread should pthread_cond_signal/broadcast once it finish the traversing to wake the worker up from their wait.
    4: The processing thread will block(pthread_cond_wait) for a condition variable until the workers are done with the processing of the requests and the queue is empty.
    5: The processing thread will cross check if the condition from step 4 is met and it will signal to the worker to exit and it will exit.
*/

void *processing(void *args)
{
    processing_args_t *procArgs  = (processing_args_t *)args;
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

        if(strcmp(newEntry, ".") == 0 ||strcmp(newEntry, "..")  == 0){
            continue;
        }
        pthread_mutex_lock(&queue_lock);
        while(queue_size >= MAX_QUEUE_LEN){
            pthread_cond_wait(&queue_empty, &queue_lock);
        }

        // store pizza index i at next_pos_for_pizza location in pizza_order_stand and update the next position to store pizza
        //pargs -> //maLLOC HERE
           main_queue[queue_index]->imgpaths[next_pos_for_path] = newEntry;
           main_queue[queue_index]->angle_rot = procArgs -> rotation_angle
           next_pos_for_path = (next_pos_for_path + 1) % MAX_QUEUE_LEN;


        // increment total number of pizza on stand by 1

        queue_size++;

        

        // signal consumer using cons_cond that one pizza is added to stand and unlock the stand

        pthread_cond_signal(&queue_empty);

        pthread_mutex_unlock(&queue_lock);

        

//         fprintf(stdout, "Producer added Pizza %d to stand\n", d_name);
//         fflush(stdout);

    }
    
    pthread_mutex_lock(&queue_lock);
    pthread_cond_signal(&queue_empty);
    pthread_mutex_unlock(&queue_lock);
//     fprintf(stdout, "Producer completed all orders, exiting...\n");
//     fflush(stdout);

        //worker threads

    }
    // close current directory
  
    closedir(dir);


    

    //pthread_cond_signal



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


void * worker(void *args)
{


        /*
            Stbi_load takes:
                A file name, int pointer for width, height, and bpp
        */

 

       /uint8_t* image_result = stbi_load(&width,&height, "?????", "???????",  CHANNEL_NUM);
        

        uint8_t **result_matrix = (uint8_t **)malloc(sizeof(uint8_t*) * width);
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
        //linear_to_image("??????", "????", "????", "????");

    
    // int* image_matrix = create_image_matrix();  
    // linear_to_image(result_matrix, img_matrix, width, height);

     

        ////TODO: you should be ready to call flip_left_to_right or flip_upside_down depends on the angle(Should just be 180 or 270)
        //both take image matrix from linear_to_image, and result_matrix to store data, and width and height.
        //Hint figure out which function you will call. 
        //flip_left_to_right(img_matrix, result_matrix, width, height); or flip_upside_down(img_matrix, result_matrix ,width, height);


        
        
        //uint8_t* img_array = NULL; ///Hint malloc using sizeof(uint8_t) * width * height
    

        ///TODO: you should be ready to call flatten_mat function, using result_matrix
        //img_arry and width and height; 
        //flatten_mat("??????", "??????", "????", "???????");


        ///TODO: You should be ready to call stbi_write_png using:
        //New path to where you wanna save the file,
        //Width
        //height
        //img_array
        //width*CHANNEL_NUM
       // stbi_write_png("??????", "?????", "??????", CHANNEL_NUM, "??????", "?????"*CHANNEL_NUM);
    

}

/*
    Main:
        x Get the data you need from the command line argument 
        x Open the logfile
        x Create the threads needed
        x Join on the created threads
        Clean any data if needed. 
*/

int main(int argc, char* argv[])
{
    if(argc != 4)worker_threads
    {
        printf("Usage: File Path to image dirctory, File path to output dirctory, number of worker thread, and Rotation angle\n");
        exit(1);
    }
    
    char* input_directory = argv[1];
    char* output_directory = argv[2];
    int num_worker_threads = atoi(argv[3]);
    int rotation_angle = atoi(argv[4]);
 
    char full_dir[4096];
    log_file = fopen(LOG_FILE_NAME,"w");
    if (log_file == NULL) {
        perror("Failed to open request_log file");
        exit(1);
    }
    
    pthread_t proccessing_thread;
    
    processing_args pargs = {input_directory, num_worker_threads, rotation_angle};
    
    pthread_create(&proccessing_thread, NULL, proccessing, &pargs);
    for (int i = 0; i < num_worker_threads; i++){num_worker_threads
        pthread_create(&worker_threads[i], NULL, worker, &worker_thread_id[i]);
    }
 
    pthread_join(processing_thread, NULL);
    for (int i = 0; i < num_worker_threads; i++) {
        pthread_join(worker_threads[i], NULL);
    }

    fclose(log_file);
    return 0;
}
