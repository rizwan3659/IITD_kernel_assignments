#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <sys/syscall.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#define SYS_writer 448
int main()
{
	FILE *fp;
	char* arr;
	char buff[8];
	arr=(char*) malloc(8*sizeof(char));
	fp= fopen("/dev/urandom","r");
	if(!fp)
	{
		printf("file does not exist.");
		exit(EXIT_FAILURE);
	}
	for(int i=0;i<8;i++)
	{
			buff[i]=fgetc(fp);
	}
	memcpy(arr,buff,8*sizeof(char));
	printf("SENDING %s\n",arr);
	long res = syscall(SYS_writer,arr);
	printf("SYSTEM CALL RETURNED %ld.\n",res);
	return 0;
}
