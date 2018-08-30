#include <stdio.h>
#include <stdlib.h>
#include <math.h>
//System libraries
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

//Defining the function
int check_visit(int n,int visited[]);
int check_prime(int num);
void sigint_handler(int s);

//Defining the array size
#define arr_len 10

int main(){
    //Starting the file descriptor
    int pfd[2];
    if(pipe(pfd)<0){
        printf("Pipe Not created\n");
        exit(0);
    }

    //signal handler
    signal(SIGINT,sigint_handler);

    //Creating the fork
    int pid=fork();

    //Making the definition of the child process
    if(pid==0){
        //Closing the write end in the chile since it will just read
        close(pfd[1]);

        //Starting the reading process from pipe
        int sum=0;
        while(1){
            //Read handling
            int num;
            read(pfd[0],&num,sizeof(int));

            //Doing child's work
            int flag=check_prime(num);
            printf(">Child\n");
            if(flag==1){
                printf("%d is Prime\n",num);
                sum+=num;
            }
            else{
                printf("%d is not Prime\n",num);
            }
            printf("S:%d\n\n",sum);
            sleep(num%3);
        }

    }
    else{//the parent process
        //Closing the read end for parent since we will just read from pipe
        close(pfd[0]);

        //Starting the transmission process
        int arr[arr_len]={12,23,33,42,57,61,72,87,48,2};
        int visited[arr_len]={0};
        int flag=check_visit(arr_len,visited);
        while(flag==0){
            //generating the random number
            int idx1=rand()%arr_len;
            int idx2=rand()%arr_len;
            while(arr[idx1]==arr[idx2]){
                idx2=rand()%arr_len;
            }
            printf(">Parent\n");
            printf("Selected x:%d y:%d\n",arr[idx1],arr[idx2]);

            //Writing the data from array to pipe (arr[idx2]=y)
            write(pfd[1],arr+idx2,sizeof(int));
            visited[idx2]=1;

            //Sleeping for x/3 sec
            sleep(arr[idx1]/3);
            flag=check_visit(arr_len,visited);
        }

        //Aborting the child if the whole work in done
        kill(pid,SIGTERM);

    }
}

int check_visit(int n,int visited[]){
    int flag=1;
    for(int i=0;i<n;i++){
        if(visited[i]==0){
            flag=0;
            break;
        }
    }
    return flag;
}

int check_prime(int num){
    int flag=1;//assuming its prime
    if(num==2){//base case
        return 1;
    }

    for(int i=2;i*i<=num;i++){
        if(num%i==0){
            flag=0;
            break;
        }
    }
    return flag;
}

void sigint_handler(int s){
    printf("\nTerminating the %d process\n",getpid());
    exit(0);
}
