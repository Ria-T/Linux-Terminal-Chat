/* semaphores.c */

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

#include "semaphores.h"

/*
    Initialises all the semaphores in the set with a given value.
*/
void sem_set_all(int id, short* vals)
{
    union semun arg;

    assert(vals!=NULL); /* safety first */

    arg.array=vals;

    semctl(id, 0, SETALL, arg);
}

/*
    Creates a new semaphore set.
    n : number of semaphores that we want in the set
    vals : starting/default values
    returns the id of the semaphore set
*/
int sem_set_create(int n, short* vals)
{
    union semun arg;

    int id;

    assert(n>0); /* We need one semaphore at least */
    assert(vals!=NULL); /* The semaphores must be initialised */

    id=semget(IPC_PRIVATE, n, SHM_R | SHM_W);

    arg.array=vals;

    semctl(id, 0, SETALL, arg);

    return id;
}

/*
    Deletes the semaphore set.
    id : the id of the set we want to delete
*/
void sem_set_del(int id)
{
    if(semctl(id, 0, IPC_RMID, NULL) == -1)
    {
        perror("Error in deleting semaphore set");
        exit(EXIT_FAILURE);
    }
}

/*
   Down one semaphore that is part of a set. If it is already down, then sleep.
   id : the id of the set the semaphore is part of
   i : the semaphore we want to down
*/
void sem_down(int id, int i)
{
    struct sembuf sb;
    sb.sem_num=i;
    sb.sem_op=-1;
    sb.sem_flg=SEM_UNDO;
    semop(id, &sb, 1);
}

/*
   Up one semaphore that is part of a set.
   id : the id of the set the semaphore is part of
   i : the semaphore we want to up
*/
void sem_up(int id, int i)
{
    struct sembuf sb;
    sb.sem_num=i;
    sb.sem_op=1;
    sb.sem_flg=SEM_UNDO;
    semop(id, &sb, 1);
}