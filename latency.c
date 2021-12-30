#include <stdio.h>
#include <getopt.h>

extern char *index(), *rindex();
char procbuf[512];
char statname[64];
char *Progname;
FILE *fp;


void get_stats(char *buf, unsigned int *es, unsigned int *cpu_r,
    unsigned int *vruntime)
{
    char *ptr;

    /* sanity */
    if (!buf || !run_ticks || !wait_ticks || !nran)
	return;

    /* leading blanks */
    ptr = buf;
    while (*ptr && isblank(*ptr))
	ptr++;

    /* first number -- run_ticks */
    *run_ticks = atoi(ptr);
    while (*ptr && isdigit(*ptr))
	ptr++;
    while (*ptr && isblank(*ptr))
	ptr++;

    /* second number -- wait_ticks */
    *wait_ticks = atoi(ptr);
    while (*ptr && isdigit(*ptr))
	ptr++;
    while (*ptr && isblank(*ptr))
	ptr++;

    /* last number -- nran */
    *nran = atoi(ptr);
}


main(int argc, char *argv[])
{
    int c;
    unsigned int sleeptime = 5, pid = 0, verbose = 0;
    char id[32];
    float exec_start, cpu_run_time, vruntime;
    float oexec_start=0,  ovruntime=0;

    while ((c = getopt(argc,argv,"s:v")) != -1) {
	switch (c) {
	    case 's':
		sleeptime = atoi(optarg);
		break;
	    case 'v':
		verbose++;
		break;
	    default:
		usage();
	}
    }

    if (optind < argc) {
	pid = atoi(argv[optind]);
    }

    if (!pid)
	usage();

    sprintf(statname,"/proc/%d/schedstat", pid);
    while (fp = fopen(statname, "r")) {
	    if (!fgets(procbuf, sizeof(procbuf), fp))
		    break;

	    get_stats(procbuf, &exec_start, &cpu_run_time, &vruntime);

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
		printf("%s avgrun=%4.2fms avgwait=%4.2fms\n",
		    id, nran - oran ?
			(double)(run_ticks-orun_ticks)/(nran - oran) : 0,
		    nran - oran ?
			(double)(wait_ticks-owait_ticks)/(nran - oran) : 0);
	    fclose(fp);
	    oran = nran;
	    orun_ticks = run_ticks;
	    owait_ticks = wait_ticks;
	    sleep(sleeptime);
	    fp = fopen(statname,"r");
	    if (!fp)
		    break;
    }
    if (id[0])
	printf("Process %s has exited.\n", id);
    else 
	printf("Process %d does not exist.\n", pid);
    exit(0);
}
