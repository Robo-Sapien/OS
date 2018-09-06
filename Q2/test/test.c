#include <stdio.h>
#include <unistd.h>
#include <linux/kernel.h>
#include <sys/wait.h>
#include "customlib.h" 

int main()
{
	long int pid = customfork();

	if(pid==0)
	{
		printf("Hello, I am child\n");
		printf("child returned: %ld\n", pid);
	}
	else
	{
		wait(NULL);
		printf("Hello, I am parent\n");
		printf("parent returned: %ld\n", pid);
	}


	return 0;
}

