#include "utils.h"

// Create N files and distribute the data from the input file evenly among them
// See section 3.1 of the project writeup for important implementation details
void partition_file_data(char *input_file, int n, char *blocks_folder) {
    FILE* fp;
    fp = fopen(input_file, "r");
    fseek(fp, 0, SEEK_SET);
    int first = floor(size(fp)/n);
    int last_file = first + size(fp) % n;
    int firstOrLast = first; //sets bytes to first used to buffer, read, write, and offset to be = to first files bytes intially
    for(int i = 0; i <= n-1; i++){
        // very rough will probably cause memory errors
          // Starting the data block partition -Chiemeka Nwakama
        if(i == n-1){
            firstOrLast = last_file; //makes bytes = to last files bytes
        }
        char fileName[7];
        sprintf(fileName, "%d.txt", i); //create char array for filename of n.txt
        FILE* fp2 = fopen(fileName, "w"); //creates second file pointer creates N.txt
        size_t bytesRead = fread(firstOrLast, 1, firstOrLast, fp); //reads the bytes from input_file to put in the partition
        fwrite(firstOrLast, 1, bytesRead, fp2); //writes the proper bytes from input file to the n partition file
        fseek(fp, firstOrLast, SEEK_CUR); //moves file point by first or last bytes
        fclose(fp2); //closes fp2 to prevent memory errors
    }
    fclose(fp); // closes inputfile as we are done with it
  
    

}


// ##### DO NOT MODIFY THIS FUNCTION #####
void setup_output_directory(char *block_folder, char *hash_folder) {
    // Remove output directory
    pid_t pid = fork();
    if (pid == 0) {
        char *argv[] = {"rm", "-rf", "./output/", NULL};
        if (execvp(*argv, argv) < 0) {
            printf("ERROR: exec failed\n");
            exit(1);
        }
        exit(0);
    } else {
        wait(NULL);
    }

    sleep(1);

    // Creating output directory
    if (mkdir("output", 0777) < 0) {
        printf("ERROR: mkdir output failed\n");
        exit(1);
    }

    sleep(1);

    // Creating blocks directory
    if (mkdir(block_folder, 0777) < 0) {
        printf("ERROR: mkdir output/blocks failed\n");
        exit(1);
    }

    // Creating hashes directory
    if (mkdir(hash_folder, 0777) < 0) {
        printf("ERROR: mkdir output/hashes failed\n");
        exit(1);
    }
}