#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <sys/syscall.h>
#include <stdio.h>

#define SYS_reader 449
int main()
{
	char* arr;
	arr=(char*) malloc(8*sizeof(char));			
	long res = syscall(SYS_reader,arr);
	printf("SYSTEM CALL RETURNED %ld.\n",res);
	printf("RECIEVED %s\n",arr);
	return 0;
}
