/*.........................................................................*/
/*                     TSH.H ------> Tuple Space Handler                   */
/*                     February '13, updated by Justin Y. Shi              */
/*.........................................................................*/

#include "synergy.h"

/*  Tuples data structure.  */

struct t_space1 {
   char name[TUPLENAME_LEN] ;	/* tuple name */
   char *tuple ;		/* pointer to tuple */
   u_short priority ;		/* priority of the tuple */
   u_long length ;		/* length of tuple */
   struct t_space1 *next ;
   struct t_space1 *prev ;
} ;
typedef struct t_space1 space1_t ;

/*  Backup tuple list. FSUN 09/94 */
/*  host1(tp) -> host2(tp) -> ... */
struct t_space2 {
        char name[TUPLENAME_LEN];
        char *tuple;
        u_short priority;
        u_long length;
        u_long host;
        u_short port;
	u_short cidport;  /* for dspace. ys'96 */
	int proc_id;
        int fault;
        struct t_space2 *next;
};
typedef struct t_space2 space2_t;

/*  Pending requests data structure.  */

struct t_queue {
   char expr[TUPLENAME_LEN] ;	/* tuple name */
   u_long host ;		/* host from where the request came */
   u_short port ;		/* corresponding port # */
   u_short cidport ;		/* for dspace. ys'96 */
   int proc_id;			/* FSUN 10/94. For FDD */
   u_short request ;		/* read/get */
   struct t_queue *next ;
   struct t_queue *prev ;
} ;
typedef struct t_queue queue1_t ;


/*  Tuple space data structure.  */

struct {
   char appid[NAME_LEN] ;	/* application id */
   char name[NAME_LEN] ;	/* name of the tuple space */
   u_short port ;		/* port where it receives commands */
   
   space1_t *space ;		/* list of tuples */
   space2_t *retrieve ;         /* list of tuples propobly retrieved. FSUN 09/94 */
   queue1_t *queue_hd ;		/* queue of waiting requests */
   queue1_t *queue_tl ;		/* new requests added at the end */
} tsh ;

queue1_t *tid_q;
int oldsock ;			/* socket on which requests are accepted */
int newsock ;			/* new socket identifying a connection */
u_short this_op ;		/* the current operation that is serviced */
char mapid[MAP_LEN];
int EOT = 0;  			/* End of task tuples mark */
int TIDS = 0;
int total_fetched = 0;

/*  Prototypes.  */

void OpPut(/*void*/) ;
void OpGet(/*void*/) ;
void OpExit(/*void*/) ;
void OpPurge();
void OpShell();


int initCommon(u_short);
void start(/*void*/);
space1_t *createTuple(char *, char *, u_long, u_short);
int consumeTuple(space1_t *);
short int storeTuple(space1_t *, int);
space1_t *findTuple(char *);
void deleteTuple(space1_t *, tsh_get_it *);
int storeRequest(tsh_get_it);
int sendTuple(queue1_t*, space1_t*);
void deleteSpace(/*void*/);
void deleteQueue(/*void*/);
queue1_t *findRequest(char *);
void deleteRequest(queue1_t *);
void sigtermHandler(/*void*/);
int getTshport(u_short);
int match(char *, char *);
int guardf(u_long, int);
