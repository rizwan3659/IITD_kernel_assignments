#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<unistd.h>



void remove_spaces(char* s) {
    char* d = s;
    do {
        while (*d == ' ') {
            ++d;
        }
    } while (*s++ = *d++);
}


int main(int argc, char *argv[])
{
    char buffer[512];
    FILE *fp;
    char buff3[64];
    int c;
    pid_t pid ;
    float exec_start,cpu_runtime , vruntime;
   
    char buff[100];
    int nice_value ;
   char buff1[256];
    FILE *fpipe;
    char cc = 0;
    int process[100];
    int i;
    int j;
 char line[1000][128];
        char fname[20];
    FILE *fptr = NULL;
    int tot = 0;
    int k=0;
 
  nice_value=1;

   sprintf(buff, "ps -efl | awk -F' '  '$8 == %d {print  $4}' > process.txt",nice_value);
   system(buff);
    fptr = fopen("process.txt", "r");
    if(!fptr)
    {
     printf("file does not exist");
     exit(EXIT_FAILURE);
    }  
	    
    while(fgets(line[i], 128, fptr))
        {
        line[i][strlen(line[i]) - 1] = '\0';
        i++;
    }
    tot = i;
    //printf("\n%d",tot);
    for(i = 0; i < tot; ++i)
    {
       process[i]= atoi(line[i]);
    }
printf("pid\t\texecution_start\t\tcpu_run_time\texecution_end\t\tvruntime\n");
while(1)
{

   for(j = 0; j < tot; j++)
  {
    k=0;	  
    pid=process[j];
       //printf("\n%d %d",j,process[j]);
    sprintf(buff3,"/proc/%d/sched", pid);    
    fp=fopen(buff3,"r");
    if(fp == NULL)
    {
     printf("cannot open the file");	    
     exit(EXIT_FAILURE);
    }	    
   while(fgets(buffer, sizeof(buffer), fp))
   {
	 char *token;
	  if(k>1) 
        {		  
	   if(k==2)
	  {	   
	   token=strtok(buffer,":");
           token = strtok(NULL,":");  
	   remove_spaces(token);
	   exec_start=atof(token);
	 //  printf("%.2f\n",exec_start);
	  }   
	   if(k==3) 
         {		   
	   token=strtok(buffer,":");
           token = strtok(NULL,":");  
	   remove_spaces(token);
	   cpu_runtime=atof(token);
	  // printf("%.2f\n",cpu_runtime);
	 }  
	   if(k==4) 
          { 		   
	   token=strtok(buffer,":");
           token = strtok(NULL,":");  
	   remove_spaces(token);
	   vruntime=atof(token);
	  // printf("%.2f\n",vruntime);
	  }  
	}   
     if(k==5) 
	   break;  
       k++;	   
    }
   printf("\n%d\t\t%.2f\t\t%.2f\t\t%.2f\t\t%.2f\n" ,pid, exec_start ,cpu_runtime, exec_start+cpu_runtime ,vruntime );
 }   
	    sleep(5);
	    printf("\n");
 }
    exit(0);
}
