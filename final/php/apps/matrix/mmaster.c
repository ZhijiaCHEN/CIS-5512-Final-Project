/*-----------------------------------------------------------------------------
                       Matrix Multiplication Master

This program has the following steps:

a) Generation of two input matrices A and B.  
b) Assemble working tuples according to a given chunk size.
c) Send the working tuples to tuple space "problem."
d) Wait for results from tuple space object "results."
e) Assemble the results as C.

date:  08/06/94

Modified by Feijian Sun, 08/22/94 for cosmetic changes + xdr.
Modified by Yuan Shi, 9/10/94 for factoring.
Modified by Justin Shi, 10/11/2016
Modified by Justin Shi, 11/17/2018 for CIS5512
-----------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <signal.h> // sigaction(), sigsuspend(), sig*()
#include <unistd.h> // alarm()
#include "matrix.h"
#include <time.h>

void handle_alarm(int signal);
void handle_sigalarm(int signal);

double C[N][N];				/* result Matrix. */
double A[N][N];
int _M[N][2], G, tsd; 		/* result marker vector */
double wall_clock();
int timeout = 0; 			// No timeout yet

int main(int argc, char *argv[])
{ 	struct sigaction sa;
	sigset_t mask;
	
	char host[128];
	int i, j, k, received;
	int ix, iy, tplength, status;
	int R, P, res, x;
	double t0, t1;
	float F;
	FILE *fd;
	time_t timer;
	char timeStamp[26];
	struct tm* tm_info;

	time(&timer);
	tm_info = localtime(&timer);
	strftime(timeStamp, 26, "%Y:%m:%d %H:%M:%S", tm_info);

	
	gethostname(host, sizeof(host));
	t0 = wall_clock();
	ix = 0;

	printf("Before ts_init() ... \n");
	ts_init(argc, argv);
	G = atoi(argv[2]); // Get chunk size
    printf(" mtclnt.  Chunk size (%d) \n",G);
	R = N;

    tplength = (1+N*N)*sizeof(double);
	// Building Matrix A and B
	for (i = 0; i < N; i++) {
			for (j = 0; j < N; j++)
			{
					ituple_B[i][j] = (double) i * j;
					A[i][j] = (double) i * j;
			}
			_M[i][0] = 0; // Reset all
			_M[i][1] = 0;
	}
	sprintf(tpname,"B%d",0);

	status = tsput(tpname, (char *)ituple_B, tplength);
	tplength = (1+ G*N) * sizeof(double);


	//printf("tplength = (%d) \n", tplength);
	if ((ituple_A = (double *) malloc(tplength)) == NULL) exit(1);

	while (R > 0) {
		if (R < G) G = R;
		R = R - G ;
		//printf(" mtclnt. G(%d) R(%d) \n", G,R);
		ituple_A[0] = G;
		for (x = 0; x < G; x++)
			for (j = 0; j < N; j++) 
				ituple_A[x*N+j+1] = A[ix+x][j];
		sprintf(tpname,"A%d",ix);
		_M[ix][0] = 0; // Reset ACK
		_M[ix][1] = G; // Remember G
		status = tsput(tpname, (char *)ituple_A, tplength);
		ix += G;
	}
	free(ituple_A);

	/* now receive the result  */
	received = i = 0;
	tplength = (1+N*N)*sizeof(double);
	if ((otuple = (double *)malloc(tplength)) == NULL)
		exit(1);
	while (received < N) {
		strcpy(tpname,"C*");
		printf(" mtclnt.  waiting for a tuple \n");
		// Setup timer here
		sa.sa_handler = &handle_sigalarm; // intercept SIGALRM
		sa.sa_flags = SA_RESETHAND; // Reset	
		sigfillset(&sa.sa_mask);
		sigaction(SIGALRM, &sa, NULL);
		sigprocmask(0, NULL, &mask);
		sigdelset(&mask, SIGALRM); 
		alarm(N/2); // Wait time = f(N). Only trigger at the very end
		tplength = tsget(tpname, (char *)otuple, 0);
		alarm(0); // Cancel the alarm regardless
		if (timeout) {
			timeout = 0;
			continue; // Skip remaining of the loop to continue receive
		}
		G = (int) otuple[0];
		ix = atoi(&tpname[1]);
		iy = 1;
		printf(" mtclnt.  tuple %d received %d) \n", ix, received);
					/* reassemble the result matrix */
		if (_M[ix][0] == 0) {
			_M[ix][0] = 1; // Received ix
			for (i= 0; i < G; i++) {
				received ++;
				for (j=0; j < N; j++) {
						C[ix][j] = otuple[iy];
						iy++;
				}
				ix ++;
			}
		} // Dropping redundant results
	}
	free(otuple);

	printf(" mtclnt.  received everything\n");
	/* insert zero size tuple as termination signal */
	tplength = sizeof(double);
	if ((ituple_A = (double *)malloc(tplength)) == NULL)
		exit(1);

	ituple_A[0]  = -1;
	sprintf(tpname, "A%d",N*N);
	status = tsput(tpname, (char *)ituple_A, tplength);
	// Purge all tuples
	ts_purge();
	free(ituple_A);
	t1 = wall_clock() - t0;
	fd = fopen("matrix.par.time", "a");
	fprintf(fd,"%s: (%s) (%f)sec. P(%d) f(%d) n(%d) ",timeStamp,
			host, t1/1000000, P, G,  N*1);
	if (t1>0) fprintf(fd, " (%f) MFLOPS.\n", (float) N*N/t1*N);
	else fprintf(fd,"%s: MFLOPS: Not measured.\n", timeStamp);
	fclose(fd);
	return 0;
}

void handle_sigalarm (int signal) {
	int x, j, status, tplength;
	char tpname2[20];

	if (signal != SIGALRM) {
		fprintf(stderr, "Caught wrong signal: %d\n", signal);
	}
	int ix=0;
	printf ("Master timedout: \n");
	while (ix < N) {
		if (_M[ix][0] == 0) {
			G = _M[ix][1];  // Retrieve G
			ituple_A[0] = G; // Define G
			for (x = 0; x < G; x++) // Assemble A[ix]
			for (j = 0; j < N; j++) 
				ituple_A[x*N+j+1] = A[ix+x][j];
			printf("Resending %d\n", ix);
 			tplength = (1+ G*N) * sizeof(double);
			sprintf(tpname2,"A%d",ix);
			status = tsput(tpname2, (char *)ituple_A, tplength);
		}
		ix+=G;
	}
	timeout = 1;
	return; 
}
