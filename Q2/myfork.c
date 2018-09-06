#include <linux/kernel.h>
#include <linux/syscalls.h>

asmlinkage long sys_myfork(void)
{
	
	printk("custom forking done\n");
	return sys_fork();
}