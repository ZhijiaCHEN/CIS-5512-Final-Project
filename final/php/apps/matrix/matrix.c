/***************************************************************************
Matrix multiplication seq program
****************************************************************************/
#include <stdio.h>
#include <unistd.h>
#include <time.h>

double C[N][N];
double A[N][N], B[N][N];
double wall_clock();

int main(int argc, char * argv[])
{
	char host[128];
	int i, j, k;
	double t0, t1;
	FILE *fd;
	int log = 0;

	gethostname(host, sizeof(host));
	if (argc > 1) log = 1;
	for (i=0; i < N ; i++)
	{
		for (j=0; j < N; j++)
		{
			C[i][j] = 0;
			B[i][j] = (double) i * j ; 
			A[i][j] = (double) i * j ;
		}
	}
	t0 = wall_clock();

	/* Switch j and k can give better results */
	for (i=0; i < N; i++)
	    for (j=0; j < N; j++)
		for (k=0; k < N; k++)
			C[i][j] += A[i][k]*B[k][j];

	t1 = wall_clock() - t0;
  	printf ("Elapsed (us):(%f) MFLOPS:(%f)\n", t1, ((N*N)/t1)*N);
/*
	if (log) {
	for (i=0; i<N; i++)
	{
		for (j=0; j<N; j++) fprintf(fd, "%8.1f ",C[i][j]);
		fprintf(fd,"\n");
	}
	}
	fclose(fd);
*/
}


/* 		Wall_clock()		*/
/* Author: Wenhai Jiang			*/
/* Date:   October 1994			*/
#include <sys/time.h>
#include <sys/resource.h>

double wall_clock()
{
struct timeval tp;
struct timezone tzp;
double t;
	gettimeofday(&tp, &tzp);
	t = (tzp.tz_minuteswest*60 + tp.tv_sec)*1.0e6 + (tp.tv_usec)*1.0;
	return t;
}
