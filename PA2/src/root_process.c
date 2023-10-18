#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include "../include/utils.h"
#include <sys/stat.h> //had to add this for PERM to be defined

#define WRITE (O_WRONLY | O_CREAT | O_TRUNC)
#define PERM (S_IRUSR | S_IWUSR)
char *output_file_folder = "output/final_submission/";

void redirection(char **dup_list, int size, char* root_dir){
    int fd = open(output_file_folder, WRITE, PERM); //opens outputfile folder and store file descriptor

        if (fd == -1) { // error check if the file doesnt exist
        perror("open");
        exit(1);
    }

    // TODO(overview): redirect standard output to an output file in output_file_folder("output/final_submission/")
    int TEMP_STDOUT_FILENO = dup(STDOUT_FILENO);  //copies the file descripter of STDOUT and stores as may want to restore it later
    dup2(fd, STDOUT_FILENO); //copies the fd of our file folder into the STDOUT_FILENO file descriptor redirecting  STDOUT to our file

    // TODO(step1): determine the filename based on root_dir. e.g. if root_dir is "./root_directories/root1", the output file's name should be "root1.txt"
    char fileName[100];
    fileName[0] = '\0'; //intially have the null terminator as the first character
    for(int i = strlen(root_dir)-1; i >-1; i--){
        if(strcmp("/", root_dir[i])){ //if we find a / we break out of the loop
            break;
        }
        char *currFilename = fileName;
        sprintf(fileName, "%c%s", root_dir[i],currFilename); //puts the new charcter in front of previous characters since we are going backward through char array until we find a /
    }
    char *currFilename = fileName;
    sprintf(fileName, "%s%s%c", currFilename, ".txt", '\0'); 
    printf("%s", fileName); //prints new file
    fflush(stdout); //forces printf to stop buffering and print the new file

  


    //TODO(step2): redirect standard output to output file (output/final_submission/root*.txt)
    char output_file[100];
    sprintf(output_file, "%s/", root_dir); //put root director plus/ for directory for output file
    strcat(output_file, fileName); // combines the file name with the dir to the file
    int outFileFd = open(output_file, WRITE, PERM); //opens outputfile folder and store file descriptor

        if (outFileFd == -1) { // error check if the file doesnt exist
        perror("open");
        exit(1);
    }

    dup2(outFileFd, STDOUT_FILENO); //redirect std out to output file

    //TODO(step3): read the content each symbolic link in dup_list, write the path as well as the content of symbolic link to output file(as shown in expected)

}

void create_symlinks(char **dup_list, char **retain_list, int size) {
    //TODO(): create symbolic link at the location of deleted duplicate file
    //TODO(): dup_list[i] will be the symbolic link for retain_list[i]
    for(int i = 0; i < size; i++){
        if(symlink(dup_list[i], retain_list[i]) != 0){ // symbolic link
        fprintf(stderr, "Unable to create sym link\n");
        exit(-1);
        }
    }

}

void delete_duplicate_files(char **dup_list, int size) {
    //TODO(): delete duplicate files, each element in dup_list is the path of the duplicate file
    for(int i = 0; i < size; i++){
        pid_t pid = fork();
        if (pid == 0) {
            char *argv[] = {"rm", "-rf", dup_list[i], NULL};
            if (execvp(*argv, argv) < 0) {
               printf("ERROR: exec failed\n");
               exit(1);
            }
            exit(0);
       } else {
        wait(NULL);
        }
    }
}

// ./root_directories <directory>
int main(int argc, char* argv[]) {
    if (argc != 2) {
        // dir is the root_directories directory to start with
        // e.g. ./root_directories/root1
        printf("Usage: ./root <dir> \n");
        return 1;
    }

    //TODO(overview): fork the first non_leaf process associated with root directory("./root_directories/root*")

    char* root_directory = argv[1];
    char all_filepath_hashvalue[4098]; //buffer for gathering all data transferred from child process
    memset(all_filepath_hashvalue, 0, sizeof(all_filepath_hashvalue));// clean the buffer

    //TODO(step1): construct pipe

    //TODO(step2): fork() child process & read data from pipe to all_filepath_hashvalue


    //TODO(step3): malloc dup_list and retain list & use parse_hash() in utils.c to parse all_filepath_hashvalue
    // dup_list: list of paths of duplicate files. We need to delete the files and create symbolic links at the location
    // retain_list: list of paths of unique files. We will create symbolic links for those files


    //TODO(step4): implement the functions
    delete_duplicate_files(dup_list,size);
    create_symlinks(dup_list, retain_list, size);
    redirection(dup_list, size, argv[1]);

    //TODO(step5): free any arrays that are allocated using malloc!!

}
