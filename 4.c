#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<signal.h>
#include <error.h>

int main (int argc, char **argv) {
    int n, k, r;
    char kstring[10];
    scanf("%d %d %d", &n, &k, &r);
    pid_t pid1, pid2, pid3;
	int pfd1 [2], pfd2 [2];
    while (r--) {
    	if (pipe (pfd1) == -1)
    		perror ("pipe");
    	 pid1 = fork ();
    	if (pid1 == 0) { // first child, will become "ps"
    		if (dup2 (pfd1 [1], STDOUT_FILENO) == -1) //STDOUT_FILENO = 1 (write)
    			perror ("dup2");
    		if (close (pfd1 [0]) == -1)
    			perror ("close");
    		if (close (pfd1 [1]) == -1)
    			perror ("close");
    		execlp ("/bin/ps", "ps", "aux", NULL);
    		perror ("execlp");
            exit(0);
    	}
    	/* parent process */
    	if (pipe (pfd2) == -1)
    		perror ("pipe");
    	pid2 = fork ();
    	if (pid2 == 0) { // second child, who will become "cut"
    		if (dup2 (pfd1 [0], STDIN_FILENO) == -1)
    			perror ("dup2");
    		if (dup2 (pfd2 [1], STDOUT_FILENO) == -1)
    			perror ("dup2");
    		if (close (pfd1 [0]) == -1)
    			perror ("close");
    		if (close (pfd1 [1]) == -1)
    			perror ("close");
    		if (close (pfd2 [0]) == -1)
    			perror ("close");
    		if (close (pfd2 [1]) == -1)
    			perror ("close");
    		execlp ("/usr/bin/sort", "sort", "-rnk", "3", NULL);
    		perror ("execlp");
            exit(0);
    	}
    	/* parent process */
    	if (close (pfd1 [0]) == -1)
    		perror ("close");
    	if (close (pfd1 [1]) == -1)
    		perror ("close");
    	pid3 = fork ();
    	if (pid3 == 0) { // third child, who will become "uniq"
    		if (dup2 (pfd2 [0], STDIN_FILENO) == -1)
    			perror ("dup2");
    		if (close (pfd2 [0]) == -1)
    			perror ("close");
    		if (close (pfd2 [1]) == -1)
    			perror ("close");
            sprintf(kstring, "%d", k);
    		execlp ("/usr/bin/head", "head", "-n", kstring, NULL);
    			perror ("execlp");
            exit(0);
    	}
    	 /* parent process */
    	if (close (pfd2 [0]) == -1)
    		perror ("close");
    	if (close (pfd2 [1]) == -1)
    		perror ("close");
    	if (waitpid (pid1, NULL, 0) == -1)
    		perror ("waitpid");
    	if (waitpid (pid2, NULL, 0) == -1)
    		perror ("waitpid");
    	if (waitpid (pid3, NULL, 0) == -1)
    		perror ("waitpid");
        printf("\n");
        sleep(n);
    }

    int temppdf[2];
    pipe(temppdf);
    pid_t tempPID;
    tempPID = fork();
    if (tempPID == 0) {
        close(temppdf[1]);
        char pidString[10];
        read(temppdf[0], pidString, 10);
        close(temppdf[0]);
        pid_t killpid =  (int) strtol(pidString, (char **)NULL, 10);
        if (killpid != -1) {
            kill(killpid, SIGKILL);
            printf("Process: %d killed\n", killpid);
        } else {
            printf("No process killed\n");
        }
        exit(0);
    } else {
        char killPID[10];
        scanf(" %s", killPID);
        close(temppdf[0]);
        write(temppdf[1], killPID, strlen(killPID));
        close(temppdf[1]);
        wait(NULL);

    }
}
