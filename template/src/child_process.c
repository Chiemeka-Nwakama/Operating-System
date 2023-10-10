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
            hash_data_block(data_from_hash, argv[4]);//computes hash of the block 
    }
    
    // TODO: If the current process is not a leaf process, spawn two child processes using  
    // exec() and ./child_process. 
    else if(child_id <= n-1){
        pid = fork();
        execl(argv[0], 2*child_id+1, NULL);//Spawn child 1
        pid = fork();
        execl(argv[0], 2*child_id+2, NULL);//Spawn child 2

        // TODO: Wait for the two child processes to finish
        wait(NULL);

        // TODO: Retrieve the two hashes from the two child processes from output/hashes/
        // and compute and output the hash of the concatenation of the two hashes.

        //read files of 2 childern
        fscanf(2*child_id+1, "%s", read_child1);//read child 1
        fscanf(2*child_id+2, "%s", read_child2);//read child 2
        
        //using files call compute dual hash
        compute_dual_hash(dual_hash_result, read_child1, read_child2);

        //store results into current nodes of output file
        fprintf(child_id, "%s", dual_hash_result);//writing hash output to parent
    }
    else{//Exit if error happens
        printf("Error");
        exit(0);
    }
}



