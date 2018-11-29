/***************** tslib functions *******************/
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

int ts_init(int argc, char *argv[])
{
	sng_map_hd.host = inet_addr("127.0.0.1");
	if ((sng_map_hd.sd = get_socket()) == -1) {
		perror("ts_init: get socket error");
		exit(0);
	};
	sng_map_hd.retport = bind_socket(sng_map_hd.sd, 0); // get return port

	if (argc > 1) // 
	{
		if (argc > 2) {
			sng_map_hd.port = atoi(argv[1]); // ntohs()???
			sng_map_hd.g = atoi(argv[2]);
		} else {
			printf("Usage %s tsport granularity \n", argv[0]);
			exit(0);
		}
  
		close(0);
        printf("cnf_init done. host (%lu) \n",sng_map_hd.host);
        return (0);
    }
	printf("Usage %s tsport granularity \n",argv[0]);
	exit(0);
}

int ts_purge() {
	int sock;
	tsh_exit_ot out ;
	u_short this_op = TSH_OP_PURGE;
	
	if ((sock = get_socket()) == -1)
	{
		perror("connectTsh::get_socket\n") ;
		return(TSPUT_ER);
	}
	if (!do_connect(sock, (sng_map_hd.host), 
		htons(sng_map_hd.port)))
	{
		perror("connectTsh::do_connect\n") ;
		close(sock);
		return(TSPUT_ER) ;
	}      
	if (!writen(sock, (char *)&this_op, sizeof(u_short)))
	{
		perror("cnf_tsput: Op code send error\n") ;
		close(sock);
		return(TSPUT_ER) ;
	}

	// Wait for ack
	readn(sock, (char *)&out, sizeof(tsh_exit_ot)) ;
	printf("ACK status(%d) error(%d)\n",out.status, out.error);
	return 1;
}
int  tsput( tpname, tpvalue, tpsize )
    int    tpsize;
    char   *tpname; 
    char   *tpvalue;
{
    int    sock;
    u_short this_op = TSH_OP_PUT;
    tsh_put_it out;
    tsh_put_ot in;
   
	this_op = htons(this_op) ;
	if ((sock = get_socket()) == -1)
	{
		perror("connectTsh::get_socket\n") ;
		return(TSPUT_ER);
	}
	if (!do_connect(sock, (sng_map_hd.host), 
		htons(sng_map_hd.port)))
	{
		perror("connectTsh::do_connect\n") ;
		close(sock);
		return(TSPUT_ER) ;
	}      
	if (!writen(sock, (char *)&this_op, sizeof(u_short)))
	{
		perror("cnf_tsput: Op code send error\n") ;
		close(sock);
		return(TSPUT_ER) ;
	}
	strcpy(out.name,tpname);
	out.priority = 1; 	/* Saved for later implementation */
	out.length = tpsize;
	out.host = sng_map_hd.host; /* gethostid(); */
	out.priority = htons(out.priority) ;
	out.length = htonl(out.length) ;
	out.proc_id = htonl(getpid());
				/* send data to TSH */
	if (!writen(sock, (char *)&out, sizeof(tsh_put_it)))
	{
		perror("cnf_tsput: Length send error\n") ;
		close(sock);
		return(TSPUT_ER);
	}
				/* send tuple to TSH */
	if (!writen(sock, tpvalue, tpsize))
	{
		perror("cnf_tsput: Value send error\n");
		close(sock);
		return(TSPUT_ER);
	}
				/* read result */
	if (!readn(sock, (char *)&in, sizeof(tsh_put_ot)))
	{
		perror("cnf_tsput: read status error\n") ;
		close(sock);
		return(TSPUT_ER) ;
	}
				/* print result from TSH */
	close(sock);
	return((int)ntohs(in.error));
}

int  tsget( tpname, tpvalue, tpsize )
    int    tpsize;
    char   *tpname; 
    char   *tpvalue;
{
    int     sock;
    u_short this_op = TSH_OP_GET;
    tsh_get_it out;
    tsh_get_ot1 in1;
    tsh_get_ot2 in2;
 
	this_op = htons(this_op) ;
	if ((sock = get_socket()) == -1)
	{
		perror("cnf_tsget: get_socket error\n") ;
		close(sock);
		return(TSGET_ER) ;
	}
	if (!do_connect(sock, (sng_map_hd.host), 
		htons(sng_map_hd.port)))
	{
		perror("cnf_tsget: TSH connection error\n") ;
		close(sock);
		return(TSGET_ER) ;
	}      
	if (!writen(sock, (char *)&this_op, sizeof(u_short)))
	{
		perror("cnf_tsget: Op code send error\n") ;
		close(sock);
		return(TSGET_ER) ;
	}
	strcpy(out.expr,tpname);
	out.host = sng_map_hd.host; /* gethostid(); */ 
	out.port = sng_map_hd.retport;  // Wait on ret_port
	out.len = htonl(tpsize);
	out.proc_id = htonl(getpid());
//	sprintf(mapid, "sng$cid$%s", getpwuid(getuid())->pw_name);
//	out.cidport = pmd_getmap(mapid, sng_map_hd.host, (u_short)PMD_PROT_TCP);
				/* send data to TSH */
	if (!writen(sock, (char *)&out, sizeof(tsh_get_it)))
	{
		perror("cnf_tsget: Length send error\n") ;
		close(sock);
		return(TSGET_ER);
	}
				/* read result */
	if (!readn(sock, (char *)&in1, sizeof(tsh_get_ot1)))
	{
		perror("cnf_tsget: read status error\n") ;
		close(sock);
		return(TSGET_ER) ;
	}
	             /* get connection in the return port */
	if (ntohs(in1.status) != SUCCESS) // Not immediately available
	{
		close(sock);
		if (tpsize == -1) return(0); /* async read */
		sock = get_connection(sng_map_hd.sd, 0) ; // wait until available
	}
                                /* read tuple details from TSH */
	if (!readn(sock, (char *)&in2, sizeof(tsh_get_ot2)))
	{
       		perror("cnf_tsget: read result length error\n") ;
       		close(sock) ;
		return(-1);
	}                           /* print tuple details from TSH */
	strcpy(tpname,in2.name) ;
	tpsize = ntohl(in2.length) ;
                                /* read, print  tuple from TSH */
	if (!readn(sock, tpvalue, tpsize))
	{
		perror("cnf_tsget: tuple read error\n") ;
		close(sock);
		return(TSGET_ER);
	} 
/*
printf(" CNF_TSGET. Sent in host(%ul) port(%d) tpname(%s)\n",
		out.host, out.cidport, tpname);
*/
	close(sock) ;
	return(tpsize);
}
int  tsread( tpname, tpvalue, tpsize )
    int    tpsize;
    char   *tpname; 
    char   *tpvalue;
{
    int    sock;
    char mapid[MAP_LEN] ;
    u_short this_op = TSH_OP_READ;
   // sng_map *link_pt;
    tsh_get_it out;
    tsh_get_ot1 in1;
    tsh_get_ot2 in2;
 
	this_op = htons(this_op) ;

	if ((sock = get_socket()) == -1)
	{
		perror("cnf_tsread: get_socket error\n") ;
		close(sock);
		return(TSREAD_ER) ;
	}
	if (!do_connect(sock, (sng_map_hd.host), 
		htons(sng_map_hd.port)))  // Same treatment
	{
		perror("cnf_tsread: TSH connection error\n") ;
		close(sock);
		return(TSREAD_ER) ;
	}      
	if (!writen(sock, (char *)&this_op, sizeof(u_short)))
	{
		perror("cnf_tsread: Op code send error\n") ;
		close(sock);
		return(TSREAD_ER) ;
	}
	strcpy(out.expr,tpname);
	out.host = sng_map_hd.host; /* gethostid(); */ 
	out.port = sng_map_hd.retport;
        out.len  = htonl(tpsize);
	out.proc_id = htonl(getpid());
				/* send data to TSH */
	if (!writen(sock, (char *)&out, sizeof(tsh_get_it)))
	{
		perror("cnf_tsread: Length send error\n") ;
		close(sock);
		return(TSREAD_ER);
	}
				/* read result */
	if (!readn(sock, (char *)&in1, sizeof(tsh_get_ot1)))
	{
		perror("cnf_tsread: read status error\n") ;
		close(sock);
		return(TSREAD_ER) ;
	}
	             /* get connection in the return port */
	if (ntohs(in1.status) != SUCCESS) 
	{
		close(sock);
		if (tpsize == -1) return(0); /* non-blocking read */
		sock = get_connection(sng_map_hd.sd,0) ;
	}
                                /* read tuple details from TSH */
	if (!readn(sock, (char *)&in2, sizeof(tsh_get_ot2)))
	{
       		perror("cnf_tsread: read result length error\n") ;
       		close(sock) ;
		return(-1);
	}                           /* print tuple details from TSH */
	strcpy(tpname,in2.name) ;
	tpsize = ntohl(in2.length) ;
                                /* read, print  tuple from TSH */
	if (!readn(sock, tpvalue, tpsize))
	{
		perror("cnf_tsread: tuple read error\n") ;
		close(sock);
		return(TSREAD_ER);
	} 
	close(sock) ;
	return(tpsize);
}


