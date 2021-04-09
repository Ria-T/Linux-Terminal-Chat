/* semaphores.h */

enum 
{
    SEM_USER_1, /* P1's turn */
    SEM_USER_2  /* P2's turn */
};

union semun
{
    int val; /* SETVAL value */
    struct semid_ds* buf; /* IPC_STAT and IPC_SET buffer */
    unsigned short* array; /* SETALL array */
    struct seminfo* __buf; /* IPC_INFO buffer */
};

void sem_set_all(int id, short* vals);
int sem_set_create(int n, short* vals);
void sem_set_del(int id);
void sem_down(int id, int i);
void sem_up(int id, int i);