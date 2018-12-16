#include <stdlib.h>
#include "synergy.h"

#define MAXNAME 128
#define TSPUT_ER		-106
#define TSGET_ER		-107
#define TSREAD_ER		-108

struct {
   char appname[MAXNAME];	/* Application system name */
   char appid[MAXNAME] ;	/* Application id (appbane+host+pid) */
   char **args;	/* arguments */
   int g ;  // Granularity
   int p ;	// # of processors (cores)
   int t ;	// threshold
   int d ;	// Debugging
   u_long host ;		/* return hostid */
   u_short port;        /* tsh port */
   u_short retport;		/* tsh return port */
   int sd;				/* socket for reading the retport */   
} sng_map_hd; 

char tpname[20];

double ituple_B[N][N];
double *ituple_A;
/* [0] = grain size
   [1] = values per rows
   ...
*/
double *otuple;
/* [0] = grain size 
   [1] = values per rows
   ...
*/
int ts_init(int, char **);
int tsput(char *, char*, int);
int tsget(char *, char*, int);
int tsread(char *, char*, int);
int ts_purge();
