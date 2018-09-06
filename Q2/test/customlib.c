#include "customlib.h"
#include <stdio.h>
#include <unistd.h>
#include <linux/kernel.h>
#include <sys/wait.h>



long customfork(void)
{
	return syscall(__NR_myfork);
}

