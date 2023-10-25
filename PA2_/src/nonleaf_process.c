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
    int pipe_write_end;

    //TODO(overview): fork the child processes(non-leaf process or leaf process) each associated with items under <directory_path>
    //TODO(step1): get <file_path> <pipe_write_end> from argv[]
    file_path = argv[1];
    pipe_write_end = atoi(argv[2]);
    char current_dir[1024];
    getcwd(current_dir, 1024);
    printf("Hi nope, %s\n",file_path);
    //TODO(step2): malloc buffer for gathering all data transferred from child process as in root_process.c
    char *child_data = (char *)malloc(sizeof(char) * strlen(file_path));
    char pipe_data[1024];

    //TODO(step3): open directory: this is done in traverse directory
    DIR *dir;
    struct dirent *entry;
    pid_t pid;
   
    
    char buf[100];
    int nbytes = 0;
    dir = opendir(file_path);
    if(dir == NULL){
        perror("readdir");
        exit(1);
    }

    while((entry = readdir(dir)) != NULL){
       
        if(entry == NULL){
            perror("readdir");
            exit(1);
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
            int fd[2];
            int ret = pipe(fd);
            pid = fork();
            if(pid == 0){//child, but parent for the new child in next nonleaf_process run
                close(fd[0]);//close read end

                getcwd(current_dir, 1024);
                sprintf(child_data, "%s", current_dir);
                strcat(child_data, "/");
                sprintf(child_data, "%s", name);

                write(fd[1], child_data, strlen(child_data));

                close(fd[1]);//close write end
                char str[100];
		sprintf(str, "%d", ret);
                execl("./nonleaf_process","./nonleaf_process",child_data, str, NULL);
            }

    //TODO(step5): read from pipe constructed for child process and write to pipe constructed for parent process
            else if (pid > 0){//parent will read from child that is written above.
             //close write end
                wait(NULL);
                close(fd[1]);

                while((nbytes = read(fd[0], buf, sizeof(char) * 100)) != 0){
                    strcat(child_data, buf);
                }
                write(pipe_write_end,child_data,sizeof(child_data));
                close(fd[0]);//close read end
            }
        }

        // if entry is a regular file 
        else if(entry->d_type == DT_REG){
            int fd[2];
            int ret = pipe(fd);
            pid = fork();
            if(pid == 0){
        //          print entry name to file
                close(fd[0]);//close read end

                getcwd(current_dir, 1024);
                sprintf(child_data, "%s", current_dir);
                strcat(child_data, "/");
                sprintf(child_data, "%s", name);

                write(fd[1], child_data, strlen(child_data));

                close(fd[1]);//close write end
                char str[100];
		    sprintf(str, "%d", fd[1]);
        //array
                printf("this is what you're sennding, %s\n",child_data);
                execl("./leaf_process","./leaf_process",child_data, str, NULL);
            }
            else if (pid > 0){//parent will read from child that is written above.
            
                wait(NULL);
                close(fd[1]);
                //close write end
                printf("hardlink\n");
                while((nbytes = read(fd[0], buf, sizeof(char) * 100)) != 0){
                    strcat(child_data, buf);
                }
                write(pipe_write_end,child_data,sizeof(child_data));
                close(fd[0]);//close read end
                 exit(0);
            }
        }

        // else
        //          if entry is symbolic 
        else{
            int fd[2];
            int ret = pipe(fd);
            pid = fork();
            if(pid == 0){
               
           
                
        //              print entry name to file 
                //close(fd[0]);//close read end

                getcwd(current_dir, 1024);
                sprintf(child_data, "%s", current_dir);
                strcat(child_data, "/");
                sprintf(child_data, "%s", name);

                write(fd[1], child_data, strlen(child_data));

                close(fd[1]);//close write end
                char str[100];
		sprintf(str, "%d", fd[1]);
                execl("./leaf_process","./leaf_process",child_data, str, NULL);
        }
            else if (pid > 0){//parent will read from child that is written above.

                wait(NULL);
                close(fd[1]);//close write end
                printf("symlink\n");
                while((nbytes = read(fd[0], buf, sizeof(char) * 100)) != 0){
                    strcat(child_data, buf);
                }
                write(pipe_write_end,child_data,sizeof(child_data));
                close(fd[0]);//close read end
            }
        }
    free(name);
    name = NULL;
    }

    closedir(dir);
    free(child_data);
    child_data = NULL;
}
