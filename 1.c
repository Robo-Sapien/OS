#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<signal.h>

void intHandler(int sig){
    signal(SIGINT, intHandler);
    printf(" Enter \"quit\" to exit prompt \nmysh> ");
    fflush(stdout);
    return;
}

int main() {
    signal(SIGINT, intHandler);
    char command[100];
    printf("mysh> ");
    scanf(" %[^\n]s", command);
    char delimiters[] = " \t";
    char * inputString = command;
    while( strcmp( command, "quit" ) != 0 ){
        char *commandName = strsep( &inputString, delimiters);
        if( strcmp( commandName, "cd" ) == 0 ){
            char *commandArg = strsep( &inputString, delimiters);
            pid_t pid;
            pid = fork();
            if(pid == 0){
                int result = chdir(commandArg);
                if (result == 0) {
                    printf("%d", execlp("/bin/pwd","pwd", NULL));
                } else {
                    if (commandArg == NULL) {
                        printf("%d", execlp("/bin/pwd","pwd", NULL));
                    }
                    printf("Error: directory not found\n");
                }
                exit(0);
            } else {
                wait(NULL);
            }
        } else if( strcmp( commandName, "echo" ) == 0 ){
            pid_t pid;
            pid = fork();
            if(pid == 0){
                execlp("/bin/echo","echo", inputString, NULL);
                exit(0);
            } else {
                wait(NULL);
            }
        } else if( strcmp( commandName, "ls" ) == 0 ){
            char *directory = strsep( &inputString, delimiters);
            pid_t pid;
            pid = fork();
            if(pid == 0){
                execlp("/bin/ls","ls", directory, NULL);
                exit(0);
            } else {
                wait(NULL);
            }
        } else if( strcmp( commandName, "wc" ) == 0 ){
            char *option = strsep( &inputString, delimiters);
            char *file = strsep( &inputString, delimiters);
            pid_t pid;
            pid = fork();
            if(pid == 0){
                execlp("/usr/bin/wc","wc", option, file, NULL);
                exit(0);
            } else {
                wait(NULL);
            }
        } else {
            printf( "Error: Command does not exist\n" );
        }
        printf( "mysh> " );
        scanf( " %[^\n]s", command );
        inputString = command;
    }
    return 0;
}
