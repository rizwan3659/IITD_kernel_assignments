#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>
#include <string.h>

#define LSIZ 128 
#define RSIZ 10 



extern char *index(), *rindex();
char procbuf[512];
char statname[64];
char *Progname;
FILE *fp;

void usage()
{
    fprintf(stderr,"Usage: %s [-s sleeptime ] <pid>\n", Progname);
    exit(-1);
}

void get_stats(char *buf, unsigned int *run_ticks, unsigned int *wait_ticks,
    unsigned int *nran)
{
    char *ptr;

    if (!buf || !run_ticks || !wait_ticks || !nran)
	return;

    ptr = buf;
    while (*ptr && isblank(*ptr))
	ptr++;

    *run_ticks = atoi(ptr);
    while (*ptr && isdigit(*ptr))
	ptr++;
    while (*ptr && isblank(*ptr))
	ptr++;

    *wait_ticks = atoi(ptr);
    while (*ptr && isdigit(*ptr))
	ptr++;
    while (*ptr && isblank(*ptr))
	ptr++;

    *nran = atoi(ptr);
}

void get_id(char *buf, char *id)

{
    char *ptr;

    if (!buf || !id)
	return;

    ptr = index(buf, ')') + 1;
    *ptr = 0;
    strcpy(id, buf);
    *ptr = ' ';
    return;
}

int main(int argc, char *argv[])
{
    int c;
    unsigned int sleeptime = 1, pid = 0, verbose = 0;
    char id[32];
    unsigned int run_ticks, wait_ticks, nran;
    unsigned int orun_ticks=0, owait_ticks=0, oran=0;
     char buff[100];
     char buff3[100];
    int nice_value ;
   char buff1[256];
    FILE *fpipe;
    char cc = 0;
    int process[100];

  //   pid=3;

 char line[RSIZ][LSIZ];
        char fname[20];
    FILE *fptr = NULL;
    int i = 0;
    int tot = 0;
  nice_value=19;

   sprintf(buff, "ps -efl | awk -F' '  '$8 == %d {print  $4}' > a.txt",nice_value);
   system(buff);

    fptr = fopen("a.txt", "r");
    while(fgets(line[i], LSIZ, fptr))
        {
        line[i][strlen(line[i]) - 1] = '\0';
        i++;
    }
    tot = i;
    for(i = 0; i < tot; ++i)
    {
       process[i]= atoi(line[i]);
    }

    
printf("pid\taverage_run(ms)\taverage_wait(ms)\tnumber_of_timeslice\t");
while(1)
{	
   for(i = 0; i < tot; ++i)
  {	    
    pid=process[i];	  
    sprintf(statname,"/proc/%d/stat", pid);
    if (fp = fopen(statname, "r")) {
	if (fgets(procbuf, sizeof(procbuf), fp))
	    get_id(procbuf,id);
	fclose(fp);
    }
    sprintf(statname,"/proc/%d/schedstat", pid);
    while (fp = fopen(statname, "r")) {
	    if (!fgets(procbuf, sizeof(procbuf), fp))
		    break;

	    get_stats(procbuf, &run_ticks, &wait_ticks, &nran);

	    if (verbose)
		printf("%s %d(%d) %d(%d) %d(%d) %4.2f %4.2f\n",
		    id, run_ticks, run_ticks - orun_ticks,
		    wait_ticks, wait_ticks - owait_ticks,
		    nran, nran - oran,
		    nran - oran ?
			(double)(run_ticks-orun_ticks)/(nran - oran) : 0,
		    nran - oran ?
			(double)(wait_ticks-owait_ticks)/(nran - oran) : 0);
	    else
		printf("\n%d\t\t%4.2f\t\t%4.2f\t\t%d\n",
		    pid, nran - oran ?
			(double)(run_ticks-orun_ticks)/(nran - oran) : 0,
		    nran - oran ?
			(double)(wait_ticks-owait_ticks)/(nran - oran) : 0 , nran);
	    fclose(fp);
	    oran = nran;
	    orun_ticks = run_ticks;
	    owait_ticks = wait_ticks;
              break ;
    }
 }   
	    sleep(sleeptime);
 }	    
    if (id[0])
	printf("Process %s has exited.\n", id);
    else 
	printf("Process %d does not exist.\n", pid);
   
    exit(0);
}
