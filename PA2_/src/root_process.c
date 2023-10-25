#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include "../include/utils.h"
#include <sys/wait.h>
#include <sys/stat.h> //had to add this for PERM to be defined

#define WRITE (O_WRONLY | O_CREAT | O_TRUNC)
#define PERM (S_IRUSR | S_IWUSR)
char *output_file_folder = "output/final_submission/";

void redirection(char **dup_list, int size, char* root_dir){


    // TODO(overview): redirect standard output to an output file in output_file_folder("output/final_submission/")

    // TODO(step1): determine the filename based on root_dir. e.g. if root_dir is "./root_directories/root1", the output file's name should be "root1.txt"
    char file[100]; //buffer for file with rootdirect and filename.txt
    sprintf(file, "%s%s.txt",output_file_folder, extract_filename(root_dir)); //
    printf("%s->sda",file);
    int fd = open(file, WRITE, PERM); //opens file and stores file descriptor

    if (fd == -1) { // error check if the file doesnt exist
        perror("open");
        exit(1);
    }


    //TODO(step2): redirect standard output to output file (output/final_submission/root*.txt)

    dup2(fd, STDOUT_FILENO); //copies the fd of our file folder into the STDOUT_FILENO file descriptor redirecting  STDOUT to our file
    printf("fjoKDADA\n");
   


    //TODO(step3): read the content each symbolic link in dup_list, write the path as well as the content of symbolic link to output file(as shown in expected)


    printf("Processing symbolic link: %d\n", size);
    for(int i = 0; i < size; i++){ //i is less than length of duplist
        char buffer[1024]; //create buffer
        printf("Processing symbolic link: %s\n", dup_list[i]);
        ssize_t lin = readlink(dup_list[i], buffer, sizeof(buffer)-1); //Reads sym link and stores in a buffer leaves room for end character
        buffer[lin] = '\0';
        if(lin != -1){ // if symlink is read correctly
              printf("[<path of symbolic link> --> <path of retained file>] : [%s --> %s]\n", dup_list[i], buffer); //prints file path from dup_list and buffer to file (redirected)
              fflush(stdout); //flushe to force printf to stop buffering and to do it right away
        


        }
        
      
        }
    
    close(fd); // closes file
    
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
    	if(dup_list[i] != NULL){
        remove(dup_list[i]);
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
    int pi[2];
    pipe(pi);
    //TODO(step2): fork() child process & read data from pipe to all_filepath_hashvalue
    pid_t pid;
    pid = fork();
    if(pid != 0){ // runs if parent process
    
	close(pi[1]);
	char buf2[1024];
	memset(buf2, 0, sizeof(buf2));
   
	//read(pi[0],all_filepath_hashvalue, 4098);
	while(read(pi[0], buf2, 250)!=0){
		
		strcat(all_filepath_hashvalue,buf2);
		strcat(all_filepath_hashvalue," ");
        printf("%s,you're better than i thought\n",all_filepath_hashvalue);
	
	}
	close(pi[0]);
     } else{ //runs if child process
    close(pi[0]);
	char buf[25];
	sprintf(buf,"%d",pi[1]);
	execl("./nonleaf_process","./nonleaf_process", root_directory, buf,NULL);
	close(pi[1]);
	}
    //TODO(step3): malloc dup_list and retain list & use parse_hash() in utils.c to parse all_filepath_hashvalue
    // dup_list: list of paths of duplicate files. We need to delete the files and create symbolic links at the location
    // retain_list: list of paths of unique files. We will create symbolic links for those files
    char** dup_list = (char**) malloc(sizeof(char*)*10); //mallocs duplist
    char** retain_list = (char**)malloc(sizeof(char*)*10); // mallocs retain
    printf("%s,found me\n",all_filepath_hashvalue);

    int size = parse_hash(all_filepath_hashvalue, dup_list, retain_list);


    
    //TODO(step4): implement the functions
    delete_duplicate_files(dup_list,size);
    create_symlinks(dup_list, retain_list, size);
    redirection(dup_list, size, argv[1]);

    //TODO(step5): free any arrays that are allocated using malloc!!
    for(int i = 0; i < size; i++){
        free(dup_list[i]);
        free(retain_list[i]);
        dup_list[i] = NULL;
        retain_list[i] = NULL;

    }

    free(dup_list);
    free(retain_list);
    dup_list= NULL;
    retain_list = NULL;

    }