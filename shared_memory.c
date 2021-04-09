/* shared_memory.c */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <syslog.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#include "shared_memory.h"

/* 
    Allocates a shared memory segment
    n : size of shared memory segment we want to allocate (in bytes)
    returns the id of the shared memory segment
*/
int shm_allocate(int n)
{
    assert(n>0); /* safety first */

    return shmget(IPC_PRIVATE, n, IPC_CREAT | SHM_R | SHM_W);
}

/*
    Maps a shared memory segment
    id : the id of the shared memory segment we want to map
    returns the address of the mapped shared memory segment
*/
void* shm_map(int id)
{
    void* addr;

    assert(id!=0); /* safety first */

    addr=shmat(id,NULL,0); /* attach the shared memory segment */
    shmctl(id, IPC_RMID, NULL); /* the segment is marked as destroyed,
                                   it will be destroyed after the last detach */  
    return addr;
}
