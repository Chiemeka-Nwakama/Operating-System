#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>
#include "hash.h"

#define PATH_MAX 1024

int main(int argc, char* argv[]) {
    if (argc != 5) {
        printf("Usage: ./child_process <blocks_folder> <hashes_folder> <N> <child_id>\n");
        return 1;
    }
     //initalizes variables
    FILE *output;
    int n = atoi(argv[3]);
    int child_id = atoi(argv[4]);
    char data_from_hash[1024] = "";
    char read_child1[1024] = "";
    char read_child2[1024] = "";
    char dual_hash_result[1024] = "";

    // TODO: If the current process is a leaf process, read in the associated block file 
    // and compute the hash of the block.
    pid_t pid;
  
    if(child_id >= n-1 && child_id <= 2*n-2){
       
        char buff[10];
        sprintf(buff, "%s/%d.txt", argv[1], child_id-n+1);//Adding blocks folder and leafs to buff
         
        hash_data_block(data_from_hash, buff);//computes hash of the block 
    
        char blockFileName[1024]; //block file name
        sprintf(blockFileName,"output/hashes/%d.txt", child_id);//Giving blockFileName the correct output file
 
        FILE* blockfp = fopen(blockFileName, "w"); //creates second file pointer

        //file open error check
        if(blockfp == NULL){
            perror("No such file");
            exit(-1);
        }

        fwrite(data_from_hash, 1, 1024, blockfp); //writes to blockFile
        fclose(blockfp); // close file


    }
    
    // TODO: If the current process is not a leaf process, spawn two child processes using  
    // exec() and ./child_process. 
    else if(child_id < n-1){
        
        char c1[50];
        sprintf(c1, "%d", 2*child_id+1);//Left child ID parameter
        char c2[50];
        sprintf(c2, "%d", 2*child_id+2);//Right child ID parameter

        pid = fork();
        if(pid == 0){
            execl(argv[0], argv[0], argv[1], argv[2], argv[3], c1, NULL);//Spawn child 1
        }
     
        pid = fork();
        if(pid == 0){
            execl(argv[0], argv[0], argv[1], argv[2], argv[3], c2, NULL);//Spawn child 2
        }
        
        // TODO: Wait for the two child processes to finish
        wait(NULL);//Wait for child 1
        wait(NULL);//Wait for child 2

        // TODO: Retrieve the two hashes from the two child processes from output/hashes/
        // and compute and output the hash of the concatenation of the two hashes.
        char child1[256]; //child 1 file name
        char child2[256]; //child 2 file name
        sprintf(child1,"output/hashes/%d.out", 2*child_id+1);
        sprintf(child2,"output/hashes/%d.out", 2*child_id+2);
        //error check
        FILE* cd1  = fopen(child1, "r");// make cd1 file pointer
        if(cd1 == NULL){
            perror("No such file");
            exit(-1);
        }
      
        FILE* cd2  = fopen(child2, "r"); //make cd2 file pointer 
        // error check
        if(cd2 == NULL){
            perror("No such file");
            exit(-1);
        }
       
        //read files of 2 childern
        fscanf(cd1, "%s", read_child1);//read child 1
        fscanf(cd2, "%s", read_child2);//read child 2
        
        //using files call compute dual hash
        compute_dual_hash(dual_hash_result, read_child1, read_child2);

        //store results into current nodes of output file
        char fileName[1024]; //block file name
        sprintf(fileName,"output/hashes/%d.out", child_id);

        output  = fopen(fileName, "w");
        fprintf(output, "%s", dual_hash_result);//writing hash output to parent

        //close files

        fclose(cd1);
        fclose(cd2);
        fclose(output);
    }
    else{//Exit if error happens
        printf("Error");
        exit(0);
    }
}



