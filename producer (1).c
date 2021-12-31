#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <sys/syscall.h>
#include <stdio.h>
#include <string.h>
#define SYS_writer 448
int main()
{
	char* arr;
	arr=(char*) malloc(8*sizeof(char));
	memcpy(arr,"HELLOOO",8*sizeof(char));
	printf("SENDING %s\n",arr);
	long res = syscall(SYS_writer,arr);
	printf("SYSTEM CALL RETURNED %ld.\n",res);
	return 0;
}
