/* P2.c */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <syslog.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <openssl/md5.h>
#include "semaphores.h"
#include "shared_memory.h"


int main(int argc, char* argv[])
{
    int idShMem; /* shared memory identity */
    int idSem; /* semaphore set identity */
    char* buf; /* the address of the shared memory's buffer */
    short vals[2]; /* initialising values for the semaphores */
    int mySem; /* P1's semaphore */
    int yourSem; /* P2's semaphore */

    char checksum[MD5_DIGEST_LENGTH];
    
    /* The id of the shared memory is typed in the command line */
   
    
    idShMem=atoi(argv[1]);
    buf=(char*) shm_map(idShMem);
    idSem=*((int*) buf);

    mySem=SEM_USER_2;
    yourSem=SEM_USER_1;
    
    memset(buf,'\0',1); /* an empty string is placed in the shared memory segment */

    sem_up(idSem, yourSem);


    while(1)
    {
        sem_down(idSem, mySem); /* I can't type */ 

        if (strcmp(buf,"TERM\n")==0) break;

        if (strlen(buf)>0 && strcmp(buf,"\0")!=0) /* If there is a reply, print it*/
        {
            printf("\nReceived from P1: %s", buf);
        }
         /* The response: */
        printf("\nP2: ");
        fflush(stdin);
        fgets(buf, sizeof(char)*200, stdin);
        MD5(buf, sizeof(buf), checksum);
        fflush(stdin);

        sem_up(idSem, yourSem); /* the other's turn */ 

        /* Exit? */
        if (strcmp(buf,"TERM\n")==0) break;
    }
}