#include "utils.h"
#include "print_tree.h"

// ##### DO NOT MODIFY THESE VARIABLES #####
char *blocks_folder = "output/blocks";
char *hashes_folder = "output/hashes";
char *visualization_file = "output/visualization.txt";


int main(int argc, char* argv[]) {
    if (argc != 3) {
        // N is the number of data blocks to split <file> into (should be a power of 2)
        // N will also be the number of leaf nodes in the merkle tree
        printf("Usage: ./merkle <file> <N>\n");
        return 1;
    }

    // TODO: Read in the command line arguments and validate them
    //Validating file and ensuring n is a correct entry
    char *input_file = argv[1];
    int n = atoi(argv[2]);

    if((n & (n - 1)) != 0){
        perror("Invalid amount of data blocks");
        exit(-1);
    }
    FILE *file = fopen(input_file,"r");
    if(file == NULL){
        perror("No such file");
        exit(-1);
    }


    //close file
    fclose(file);

    // ##### DO NOT REMOVE #####
    setup_output_directory(blocks_folder, hashes_folder);

    // TODO: Implement this function in utils.c
    partition_file_data(input_file, n, blocks_folder);


    // TODO: Start the recursive merkle tree computation by spawning first child process (root)
    pid_t pid;
	pid = fork();
    if(pid == 0){
        execl("./child_process","./child_process", blocks_folder, hashes_folder,argv[2],"0",NULL);//rum child_process
    }
   
    wait(NULL);//Wait for child process
  

    // ##### DO NOT REMOVE #####
    #ifndef TEST_INTERMEDIATE
        // Visually display the merkle tree using the output in the hashes_folder
  
        print_merkle_tree(visualization_file, hashes_folder, n);
    #endif

    return 0;
}
