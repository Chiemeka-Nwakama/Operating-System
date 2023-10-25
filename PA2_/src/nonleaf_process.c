#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>
#include <dirent.h>
#include <sys/wait.h>

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Usage: ./nonleaf_process <directory_path> <pipe_write_end> \n");
        return 1;
    }
    char *file_path;
    char *pipe_write_end;

    //TODO(overview): fork the child processes(non-leaf process or leaf process) each associated with items under <directory_path>
    //TODO(step1): get <file_path> <pipe_write_end> from argv[]
    file_path = argv[1];
    pipe_write_end = argv[2];
     char current_dir[1024];
    getcwd(current_dir, 1024);
    printf("Hi nope, %s\n",current_dir);
    //TODO(step2): malloc buffer for gathering all data transferred from child process as in root_process.c
    char *child_data = (char *)malloc(sizeof(char) * strlen(pipe_write_end));
    char pipe_data[1024];

    //TODO(step3): open directory: this is done in traverse directory
    DIR *dir;
    struct dirent *entry;
    pid_t pid;
   
    int fd[2];
    int ret = pipe(fd);
    pid = fork();
    char buf[100];
    int nbytes = 0;
    if (strncmp(file_path, "./", 2) == 0) {
        // Remove the "./" prefix by shifting the remaining characters to the left.
        memmove(file_path, file_path + 2, strlen(file_path) - 2 + 1); // +1 to include the null terminator
    }
    dir = opendir(file_path);
    if(dir == NULL){
        perror("readdir");
        printf("you thought\n");
        exit(1);
    }

    while((entry = readdir(dir)) != NULL){
        if(entry == NULL){
            perror("readdir");
            printf("not here either");
            exit(1);
        }
        printf("dumb");
    if(ret == -1){
        printf("Error creating pipe...\n");
        exit(-1);
    }

    if(strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0 ){
        continue;
    }
    char *name = (char*)malloc(sizeof(char) * (strlen(file_path) + 1 + strlen(entry->d_name) + 1));
    sprintf(name, "%s", file_path);
    strcat(name, "/");
    strcat(name, entry->d_name);

        //TODO(step4): traverse directory and fork child process
        // Hints: Maintain an array to keep track of each read end pipe of child process

        // if entry is a directory 
        if(entry->d_type == DT_DIR){
            
            if(pid == 0){//child, but parent for the new child in next nonleaf_process run
                close(fd[0]);//close read end

                getcwd(current_dir, 1024);
                sprintf(pipe_data, "%s", current_dir);
                strcat(pipe_data, "/");
                sprintf(pipe_data, "%s", name);

                write(fd[1], pipe_data, strlen(pipe_data));

                close(fd[1]);//close write end
                char str[100];
		sprintf(str, "%d", ret);
                execl("./nonleaf_process","./nonleaf_process",child_data, str, NULL);
            }

    //TODO(step5): read from pipe constructed for child process and write to pipe constructed for parent process
            else if (pid > 0){//parent will read from child that is written above.
                wait(NULL);
                close(fd[1]);//close write end

                while((nbytes = read(fd[0], buf, sizeof(char) * 100)) != 0){
                    strcat(child_data, buf);
                }

                close(fd[0]);//close read end
            }
        }

        // if entry is a regular file 
        else if(entry->d_type == DT_REG){
        //          print entry name to file
                close(fd[0]);//close read end

                getcwd(current_dir, 1024);
                sprintf(pipe_data, "%s", current_dir);
                strcat(pipe_data, "/");
                sprintf(pipe_data, "%s", name);

                write(fd[1], pipe_data, strlen(pipe_data));

                close(fd[1]);//close write end
                char str[100];
		sprintf(str, "%d", ret);
                execl("./leaf_process","./leaf_process",child_data, str, NULL);
        }

        // else
        //          if entry is symbolic 
        else{
        //              print entry name to file 
                close(fd[0]);//close read end

                getcwd(current_dir, 1024);
                sprintf(pipe_data, "%s", current_dir);
                strcat(pipe_data, "/");
                sprintf(pipe_data, "%s", name);

                write(fd[1], pipe_data, strlen(pipe_data));

                close(fd[1]);//close write end
                char str[100];
		sprintf(str, "%d", ret);
                execl("./leaf_process","./leaf_process",child_data, str, NULL);
        }
    free(name);
    name = NULL;
    }

    closedir(dir);
    free(child_data);
    child_data = NULL;
}
