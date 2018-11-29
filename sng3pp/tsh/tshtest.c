/*.........................................................................*/
/*                  TSHTEST.C ------> TSH test program                     */
/*                                                                         */
/*                  By N. Isaac Rajkumar [April '93]                       */
/*                  February '13, updated by Justin Y. Shi                 */
/*                  November '18, updated by Justin Y. Shi for CIS5512     */
/*.........................................................................*/

#include "tshtest.h"
tsh_get_it out;  // For return port prep
int sd, status;
struct in_addr addr ; 

/*---------------------------------------------------------------------------
  Prototype   : u_short bind_socket2(int sd, u_short port)
  Parameters  : sd   - socket descriptor that has to be bound to port
                port - port # [0 if any port]
  Returns     : port (or) 0 if error
  Calls       : bzero, bind, getsockname, listen
  Notes       : The socket descriptor is bound to the port specified or any 
                available port.
  Date        : November 3, 2018
  Coded by    : Revised from bind_socket() by N. Isaac Rajkumar
  Modification:
---------------------------------------------------------------------------*/  

u_short bind_socket2(int sd, u_short port)
{
   struct sockaddr_in self ;
   int self_len ;
				/* bind socket to any port */
   bzero((char *) &self, sizeof(self)) ;
   self.sin_family = AF_INET ;
   self.sin_addr.s_addr = htonl(INADDR_ANY) ;
   self.sin_port = (port) ;  // Removed htons() call. JYS
   if (bind(sd, (struct sockaddr *)&self, sizeof(self)) == -1)
      return 0 ;
				/* determine bound port */
   bzero((char *) &self, sizeof(self)) ;
   self_len = sizeof(self) ;
   if (getsockname(sd, (struct sockaddr *)&self, &self_len) == -1)
      return 0 ;
   if (listen(sd, 5) == -1)
      return 0 ;
   
   return self.sin_port ;	/* return port bound */
}

int main(int argc, char **argv)
{	
	static void (*op_func[])() = 
	{
		OpPut, OpGet, OpGet, OpPurge, OpShell, OpExit
    } ;
	u_short this_op ;
   
	if (argc < 2)
    {
       printf("Usage : %s TS-port\n", argv[0]) ;
       exit(1) ;
    }
	// Check the availability of return Ret-port
	out.host = gethostid() ;	// Get return hostid (localhost)
	if ((sd = get_socket()) == -1)
    {
       perror("\nReturn sock failure::get_socket. Try a different port.\n") ; 
       exit(1);
    }
	
	if (!(out.port = bind_socket(sd, 0)))
    {
       perror("\nReturn sock failure::bind_socket. No port available\n") ;
       exit(1);
    }
	addr.s_addr = out.host;
				/* print return sock info */
	printf("\nReturn Port Info:\n") ;
	printf("\nhost : %s", inet_ntoa(addr)) ;
	printf("\nHOST (%d)", out.host);
	printf("\nport : %d\n", out.port) ;
	close(sd);  // To be rebuilt in GET
	getchar();
	while (TRUE)
    {
       this_op = drawMenu() + TSH_OP_MIN - 1 ;
       if (this_op >= TSH_OP_MIN && this_op <= TSH_OP_MAX)
	   {
		   this_op = htons(this_op) ;
		   tshsock = connectTsh(atoi(argv[1])) ;
		   // Send this_op to TSH
		   if (!writen(tshsock, (char *)&this_op, sizeof(this_op)))
			{
			   perror("main::writen\n") ;
			   exit(1) ;
			}
			printf("sent tsh op %d \n",this_op);
		   // Response processing
		   (*op_func[ntohs(this_op) - TSH_OP_MIN])() ;
		   close(sd); // Close return sock
		   close(tshsock) ;
	   }			/* validate operation & process */
       else return 0 ;
    }
}

void OpShell()
{
	system("clear");
	printf("TSH_OP_SHELL");
	printf("\n--------------\n");
	printf("\nEnter command(s)$");
	getchar();
	fgets(shell_in.cmd, sizeof(shell_in.cmd), stdin);
	printf("Read cmd(%s)\n",shell_in.cmd);
	shell_in.host = out.host;
	shell_in.port = out.port;
	// send shell_in
	if (!writen(tshsock, (char *)&shell_in, sizeof(shell_in)))
    {
       perror("\nOpShell:writen::failure\n") ;
       getchar() ;
       return ;
    }
	// read shell_out
	if (!readn(tshsock, (char *)&shell_out, sizeof(shell_out)))
    {
       perror("\nOpShell::readn::Failure\n") ;
       getchar() ;
       return ;
    }	
	// print return values
	printf("\n pid=(%d)\n", shell_out.pid);
	printf("\n status=(%d)\n", shell_out.status);
	printf("\n cwd=(%s)\n", shell_out.cwd);
	printf("\n username=(%s)\n", shell_out.username);
	printf("\n stdout=(%s)\n", shell_out.stdout);
	getchar();
	return;
}

void OpPurge()
{
	tsh_exit_ot out ;
	// Wait for ack
	readn(tshsock, (char *)&out, sizeof(tsh_exit_ot)) ;
	printf("ACK status(%d) error(%d)\n",out.status, out.error);
	getchar(); getchar();
	return;
}
void OpPut()
{
   tsh_put_it out ;
   tsh_put_ot in ;
   int tmp ;
   char *buff,*st ;

   status=system("clear") ;
   printf("TSH_OP_PUT") ;
   printf("\n----------\n") ;
				/* obtain tuple name, priority, length, */
   printf("\nEnter tuple name : ") ; /* and the tuple */
   status=scanf("%s", out.name) ;
   printf("Enter priority : ") ;
   status=scanf("%d", &tmp) ;
   out.priority = (u_short)tmp ;
   printf("Enter length : ") ;
   status=scanf("%d", &out.length) ;
   getchar() ;
   printf("Enter tuple : ") ;
   buff = (char *)malloc(out.length) ;
   st=fgets(buff, out.length, stdin) ;
				/* print data sent to TSH */
   printf("\n\nTo TSH :\n") ;
   printf("\nname : %s", out.name) ;
   printf("\npriority : %d", out.priority) ;
   printf("\nlength : %d", out.length) ;
   printf("\ntuple : %s\n", buff) ;

   out.priority = htons(out.priority) ;
   out.length = htonl(out.length) ;
				/* send data to TSH */
   if (!writen(tshsock, (char *)&out, sizeof(out)))
    {
       perror("\nOpPut::writen\n") ;
       getchar() ;
       free(buff) ;
       return ;
    }
				/* send tuple to TSH */
   if (!writen(tshsock, buff, ntohl(out.length)))
    {
       perror("\nOpPut::writen\n") ;
       getchar() ;
       free(buff) ;
       return ;
    }
				/* read result */
   if (!readn(tshsock, (char *)&in, sizeof(in)))
    {
       perror("\nOpPut::readn\n") ;
       getchar() ;
       return ;
    }
				/* print result from TSH */
   printf("\n\nFrom TSH :\n") ;
   printf("\nstatus : %d", ntohs(in.status)) ;
   printf("\nerror : %d\n", ntohs(in.error)) ;
   getchar() ;
}


void OpGet()
{

	tsh_get_ot1 in1 ;
	tsh_get_ot2 in2 ;
	int sock ;
	char *buff ;

	
	status=system("clear") ;
	printf("TSH_OP_GET") ;
	printf("\n----------\n") ;
				/* obtain tuple name/wild card */
	printf("\nEnter tuple name [wild cards ?, * allowed] : ") ;
	status=scanf("%s", out.expr) ;
				/* send data to TSH */
				// Rebuild sd
	if ((sd = get_socket()) == -1)
    {
       perror("\nReturn sock failure::get_socket. Try a different port.\n") ; 
       exit(1);
    }
	if (!(out.port = bind_socket2(sd, out.port)))
    {
       perror("\nReturn sock failure::bind_socket. Try a different port.\n") ;
       exit(1);
    }
	addr.s_addr = out.host;
				/* print return sock info */
	printf("\nGET Return Port Info:\n") ;
	printf("\nhost : %s", inet_ntoa(addr)) ;
	printf("\nHOST (%d)", out.host);
	printf("\nport : %d\n", out.port) ;
	getchar();
   if (!writen(tshsock, (char *)&out, sizeof(out)))
    {
       perror("\nOpGet::writen\n") ;
       getchar() ; getchar() ;
       close(sd) ;
       return ;
    }
				/* find out if tuple available */
   if (!readn(tshsock, (char *)&in1, sizeof(in1)))
    {
       perror("\nOpGet::readn\n") ;
       getchar() ; getchar() ;
       close(sd) ;
       return ;
    }
				/* print whether tuple available in TSH */
   printf("\n\nFrom TSH :\n") ;
   printf("\nstatus : %d", ntohs(in1.status)) ;
   printf("\nerror : %d\n", ntohs(in1.error)) ;
				/* if tuple is available read from the same */
   if (ntohs(in1.status) == SUCCESS) /* socket */
      sock = tshsock ;
   else				/* get connection on the return port */
      sock = get_connection(sd, NULL) ;
				/* read tuple details from TSH */
   if (!readn(sock, (char *)&in2, sizeof(in2)))
    {
       perror("\nOpGet::readn\n") ;
       getchar() ; getchar() ;
       close(sd) ;
       return ;
    }				/* print tuple details from TSH */
   printf("\nname : %s", in2.name) ;
   printf("\npriority : %d", ntohs(in2.priority)) ;
   printf("\nlength : %d", ntohl(in2.length)) ;
   buff = (char *)malloc(ntohl(in2.length)) ;
				/* read, print  tuple from TSH */
   if (!readn(sock, buff, ntohl(in2.length)))
      perror("\nOpGet::readn\n") ;
   else
      printf("\ntuple : %s\n", buff) ;

   close(sd) ;
   close(sock) ;
   free(buff) ;
   getchar() ; getchar() ;
}


void OpExit()
{
   tsh_exit_ot in ;
   
   status=system("clear") ;
   printf("TSH_OP_EXIT") ;
   printf("\n-----------\n") ;
				/* read TSH response */
   if (!readn(tshsock, (char *)&in, sizeof(in)))
    {
       perror("\nOpExit::readn\n") ;
       getchar() ;  getchar() ;
       return ;
    }
				/* print TSH response */
   printf("\n\nFrom TSH :\n") ;
   printf("\nstatus : %d", ntohs(in.status)) ;
   printf("\nerror : %d\n", ntohs(in.error)) ;
   getchar() ;  getchar() ;
}


int connectTsh(u_short port)
{
   struct hostent *host ;
   short tsh_port ;
   u_long tsh_host ;
   int sock ;

   // use local host 
   tsh_host = inet_addr("127.0.0.1");
   /*
   if ((host = gethostbyname("localhost")) == NULL)
	{
	   perror("connectTsh::gethostbyname\n") ;
	   exit(1) ;
	}
   tsh_host = *((long *)host->h_addr_list[0]) ;
   */
   tsh_port = htons(port); // Notice: htons() here
				/* get socket and connect to TSH */
   if ((sock = get_socket()) == -1)
    {
       perror("connectTsh::get_socket\n") ;
       exit(1) ;
    }
   if (!do_connect(sock, tsh_host, tsh_port))
    {
       perror("connectTsh::do_connect\n") ;
       exit(1) ;
    }      
   return sock ;
}


u_short drawMenu()
{
   int choice ;
				/* draw menu of user options */
   status=system("clear") ;
   printf("\n\n\n\t\t\t---------") ;
   printf("\n\t\t\tMAIN MENU") ;
   printf("\n\t\t\t---------") ;
   printf("\n\n\t\t\t 1. Put") ;
   printf("\n\t\t\t 2. Get") ;
   printf("\n\t\t\t 3. Read") ;
   printf("\n\t\t\t 4. Purge");
   printf("\n\t\t\t 5. OpShell");
   printf("\n\t\t\t 6. Exit (TSH)") ;
   printf("\n\t\t\t Anything else. Quit") ;
       
   printf("\n\n\n\t\t\tEnter Choice : ") ;

   status=scanf("%d", &choice) ;	/* return user choice */
   return choice ;
}
