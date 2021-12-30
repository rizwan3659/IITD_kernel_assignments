#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <sys/types.h>
#include<errno.h>

void work()
{
	for(unsigned long i=0; i<100000000000; ++i);
}


long delay_selection(long pid, long long delay)
{
	return syscall(548, pid,delay);
}

int main() 
{
	unsigned long delay;
	nice(19);
	pid_t pid = fork();
	if(pid < 0) 
	{
		perror("Forking Failed");
	}
	else if((long)pid == 0) 
	{
		pid_t pid1;
		pid1 = getpid();
		if(!delay_selection(pid,delay))
		{
                  perror("system call failed");
                  exit(EXIT_FAILURE);
		}	
		work();
		work();
		work();
		exit(0);
	}
	else 
	{
		work();
		work();
		work();
		wait(NULL);
	}
}
