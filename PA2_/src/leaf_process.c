#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "../include/hash.h"
#include "../include/utils.h"
#include <sys/wait.h>

char *output_file_folder = "output/inter_submission/";

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Usage: Inter Submission --> ./leaf_process <file_path> 0\n");
        printf("Usage: Final Submission --> ./leaf_process <file_path> <pipe_write_end>\n");
        return -1;
    }
    //Getting <file_path> <pipe_write_end> from argv[]
     //char* file_path[1024] = argv[1];//Get the file path for hashblock
     int pipe_write_end = atoi(argv[2]);//Get the pipe end
     char data_from_hash[SHA256_BLOCK_SIZE * 2 + 1] = "";//buffer to hold hash data
     char* root_directory = extract_root_directory(argv[1]);// String that contains root directory
     hash_data_block(data_from_hash, argv[1]);//Creating hash of given file
     int lenStr = strlen(argv[1])+strlen(data_from_hash);
     char strToPipe[lenStr+1];//Buffer for combined string to write to pipe
     char file_name[1024] ="";
     sprintf(file_name, "%s", extract_filename(argv[1]));
     sprintf(strToPipe, "%s|%s|", argv[1], data_from_hash);
    if(pipe_write_end == 0){
        //TODO(overview): create a file in output_file_folder("output/inter_submission/root*") and write the constructed string to the file
        
        //TODO(step1): extract the file_name from file_path using extract_filename() in utils.c
        //printf("%s\n",extract_filename(argv[1]));
        //TODO(step2): extract the root directory(e.g. root1 or root2 or root3) from file_path using extract_root_directory() in utils.c
        //char *file_root = extract_root_directory(argv[1]);// String that contains root directory
        //TODO(step3): get the location of the new file (e.g. "output/inter_submission/root1" or "output/inter_submission/root2" or "output/inter_submission/root3")
        char location[1024] = "";
        //file_root[strlen(file_root)-1] = '\0';
        sprintf(location, "%s%s", output_file_folder, root_directory);
        char outputFilePath[1024]  = "";
        sprintf(outputFilePath, "%s%s%s", output_file_folder, root_directory,file_name);
        //TODO(step4): create and write to file, and then close file
        FILE* output_file = fopen(outputFilePath, "w");
        //file open error check
        if(output_file == NULL){
            perror("No such file");
            exit(-1);
        }
        fwrite(strToPipe, 1, strlen(strToPipe), output_file); //writes to blockFile
        fclose(output_file); // close file
        //TODO(step5): free any arrays that are allocated using malloc!! Free the string returned from extract_root_directory()!! It is allocated using malloc in extract_root_directory()
        
        free(root_directory);
        root_directory = NULL;


    }else{
        //TODO(final submission): write the string to pipe
	write(pipe_write_end, strToPipe, 1024);
	printf("Hi hello,%s\n",strToPipe);
    }
    

    exit(0);
}
