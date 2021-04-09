/* P1.c */

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
   
    idShMem=shm_allocate(sizeof(char)*200); /* create the id if it is not given */
    buf=(char*) shm_map(idShMem);

    /* At first, we initialise both vals with a 0 because both P1 and P2 are unable to write (which means that neither of them can exclusively access the
    shared memory segment). When it is P2's turn to type, it "unlocks" the first one so that it is able to write. */
    vals[SEM_USER_1]=0;
    vals[SEM_USER_2]=0;
    idSem = sem_set_create(2,vals);

    *((int*) buf)=idSem; /* the semaphore id is saved inside the shared memory segment so that both users (P1 and P2) can access it */

    mySem=SEM_USER_1;
    yourSem=SEM_USER_2;    
    printf("%d\n",idShMem);
    
    

    while(1)
    {
        sem_down(idSem, mySem); /* I can't type */ 

        if (strcmp(buf,"TERM\n")==0) break;

        if (strlen(buf)>0 && strcmp(buf,"\0")!=0) /* If there is a reply, print it*/
        {
            printf("\nReceived from P2: %s", buf);
        }
         /* The response: */
        printf("\nP1: ");
        fflush(stdin);
        fgets(buf, sizeof(char)*200, stdin);
        MD5(buf, sizeof(buf), checksum);
        fflush(stdin);

        sem_up(idSem, yourSem); /* the other's turn */ 

        /* Exit? */
        if (strcmp(buf,"TERM\n")==0) break;
    }

    if(mySem==SEM_USER_1) /* P1 has to free the semaphores */
    {
        sem_set_del(idSem);
    }
}