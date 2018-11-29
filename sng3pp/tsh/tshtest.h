/*.........................................................................*/
/*                  TSHTEST.H ------> TSH test program                     */
/*                  February '13, Oct '18 updated by Justin Y. Shi         */
/*.........................................................................*/

#include "synergy.h"

char login[NAME_LEN];

void OpPut(/*void*/) ;
void OpGet(/*void*/) ;
void OpPurge();
void OpShell();
void OpExit(/*void*/) ;
void OpQuit(/*void*/) ;

int tshsock ;
int retsock ;

int connectTsh(u_short) ;
u_short drawMenu(/*void*/) ;

