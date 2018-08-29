#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>

int main() {
    char command[100];
    printf("mysh> ");
    scanf(" %[^\n]s", command);
    char delimiters[] = " \t";
    char * inputString = command;
    while( strcmp( command, "quit" ) != 0 ){
        char *commandName = strsep( &inputString, delimiters);
        if( strcmp( commandName, "cd" ) == 0 ){
            char *commandArg = strsep( &inputString, delimiters);
            if(chdir(commandArg) == -1){
                printf( "Error: Invalid Command\n" );
            }
        } else if( strcmp( commandName, "echo" ) == 0 ){

        } else if( strcmp( commandName, "ls" ) == 0 ){

        } else if( strcmp( commandName, "wc" ) == 0 ){

        } else {
            printf( "Error: Command does not exist\n" );
        }
        printf( "mysh> " );
        scanf( " %[^\n]s", command );
        inputString = command;
    }
    return 0;
}
