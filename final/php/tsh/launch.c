/*.........................................................................*/
/*                  LAUNCH.H ------> asks tsh to launch a cmd              */
/*                  Modified from tshtest.c                                */
/*.........................................................................*/

#include "tshtest.h"
tsh_get_it out; // For return port prep
int sd, status;
struct in_addr addr;

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
    struct sockaddr_in self;
    int self_len;
    /* bind socket to any port */
    bzero((char *)&self, sizeof(self));
    self.sin_family = AF_INET;
    self.sin_addr.s_addr = htonl(INADDR_ANY);
    self.sin_port = (port); // Removed htons() call. JYS
    if (bind(sd, (struct sockaddr *)&self, sizeof(self)) == -1)
        return 0;
    /* determine bound port */
    bzero((char *)&self, sizeof(self));
    self_len = sizeof(self);
    if (getsockname(sd, (struct sockaddr *)&self, &self_len) == -1)
        return 0;
    if (listen(sd, 5) == -1)
        return 0;

    return self.sin_port; /* return port bound */
}

int main(int argc, char **argv)
{
    u_short this_op = htons(5 + TSH_OP_MIN - 1);

    if (argc < 3)
    {
        printf("Usage : %s TS-port program [arg1, ]\n", argv[0]);
        exit(1);
    }
    tshsock = connectTsh(atoi(argv[1]));
    // Send this_op to TSH
    if (!writen(tshsock, (char *)&this_op, sizeof(this_op)))
    {
        perror("main::writen\n");
        exit(1);
    }
    printf("sent tsh op %d \n", this_op);
    // Response processing
    OpShell(argc - 2, argv + 2);
    close(tshsock);
    return 0;
}

void OpShell(int argc, char **argv)
{
    //system("clear");
    //printf("TSH_OP_SHELL");
    //printf("\n--------------\n");
    //printf("\nEnter command(s)$");
    //getchar();
    memset(shell_in.cmd, 0, sizeof(shell_in.cmd));
    for (int i = 0; i < argc; ++i)
    {
        strcat(shell_in.cmd, argv[i]);
        strcat(shell_in.cmd, " ");
    }
    strcat(shell_in.cmd, "&"); // append the cmd with & to ask tsh to run it in the background

    printf("launch is going to send cmd(%s)\n", shell_in.cmd);
    shell_in.host = out.host;
    shell_in.port = out.port;
    // send shell_in
    if (!writen(tshsock, (char *)&shell_in, sizeof(shell_in)))
    {
        perror("\nOpShell:writen::failure\n");
        getchar();
        return;
    }
    // read shell_out
    if (!readn(tshsock, (char *)&shell_out, sizeof(shell_out)))
    {
        perror("\nOpShell::readn::Failure\n");
        getchar();
        return;
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

int connectTsh(u_short port)
{
    struct hostent *host;
    short tsh_port;
    u_long tsh_host;
    int sock;

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
        perror("connectTsh::get_socket\n");
        exit(1);
    }
    if (!do_connect(sock, tsh_host, tsh_port))
    {
        perror("connectTsh::do_connect\n");
        exit(1);
    }
    return sock;
}
