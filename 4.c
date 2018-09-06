#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<signal.h>
#include<error.h>

void printProcesses(int n, int k, int r){
    char kstring[10];
    int pfd1[2];
    while(r--){
        if (pipe (pfd1) == -1)
            perror ("pipe");
        pid_t pid1 = fork();
        if(pid1 == 0){ //child2
            close(1);
            if(dup2(pfd1[1], STDOUT_FILENO) == -1)
                perror("dup2");
            close(pfd1[0]);
            close(pfd1[1]);
            execlp ("/bin/ps", "ps", "aux", "--sort=-pcpu", NULL);
            exit(0);
        } else { //child1
            if (waitpid (pid1, NULL, 0) == -1)
                perror ("waitpid");
            pid_t pid2 = fork();
            if(pid2 == 0){ //child3
                close(0);
                if(dup2(pfd1[0], STDIN_FILENO) == -1)
                    perror("dup2");
                close(pfd1[0]);
                close(pfd1[1]);
                sprintf(kstring, "%d", k+1);
                execlp ("/usr/bin/head", "head", "-n", kstring, NULL);
                    perror ("execlp");
                exit(0);
            } else { //child1
                if (waitpid (pid2, NULL, 0) == -1)
                    perror ("waitpid");
                printf("\n");
                sleep(n);
            }
        }
    }
}

void killProcess(int pid){
    int pidfork = fork();
    if (pidfork == 0) {
        char pidkill[10];
        sprintf(pidkill, "%d", pid);
        execlp ("/bin/kill", "kill", "-9", pidkill, NULL);
            perror ("execlp");
        exit(0);
    } else {
        if (waitpid (pidfork, NULL, 0) == -1)
            perror ("waitpid");
    }
}

int main(){
    int n, k, r;
    scanf("%d %d %d", &n, &k, &r);
    int pfdcom[2], pfdkill[2];
    pipe( pfdkill );
    pipe( pfdcom );
    pid_t pid = fork();
    if(pid == 0){ //Child1
        close(pfdkill[1]);
        close(pfdcom[0]);
        while (1) {
            printProcesses(n, k, r);
            char pidString[10];
            int sig = 1, pidkill;
            write(pfdcom[1],&sig,sizeof(int));
            read(pfdkill[0],&pidkill,sizeof(int));
            if(pidkill != -1)
                killProcess( pidkill );
            else
                printf("No process killed\n");
        }
    } else { //Parent
        close(pfdkill[0]);
        close(pfdcom[1]);
        while(1){
            int sig,pidkill;
            read(pfdcom[0],&sig,sizeof(int));
            if(sig==1){
                printf("Enter the pid of process to kill:\n");
                scanf("%d",&pidkill);
                write(pfdkill[1],&pidkill,sizeof(int));
            }
        }
    }
    return 0;
}
