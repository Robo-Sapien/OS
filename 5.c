#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>


int main(int argc,char **argv){
    //Testing the commandline argument
    // for(int i=1;i<argc;i++){
    //     printf("%s\n",argv[i]);
    // }
    if(argc!=3){
        printf("Please specify\n 1.string to search\n 2.file to search the string\n");
        exit(0);
    }

    //Creating the pipe
    int fd[2];
    if(pipe(fd)<0){
        printf("Pipe not created\n");
    }
    //Forking the child
    int pid=fork();

    //Child Process code (handles the grep)
    if(pid==0){
        //Closing the read file descriptor
        close(fd[0]);
        //Closing the stdout descriptor to later connect to pipe
        close(1);
        dup2(fd[1],1);

        //Running the command
        int exit_stat=execlp("grep","grep","-n",argv[1],argv[2],NULL);
        printf("BAD Exit_Status: %d \n",exit_stat);
        exit(0);
    }
    else{
        //Waiting the child process to finish
        wait(NULL);

        //Closing the write file descriptor
        close(fd[1]);

        //Connecting the pipe to stdin of the main program
        close(0);
        dup2(fd[0],0);

        //Now reading the pipe (since its directly connected to stdin of current one)
        int exit_stat=execlp("cut","cut","-d",":","-f","1",NULL);
        printf("BAD Exit_Status: %d \n",exit_stat);
        exit(0);
    }

    return 0;
}
