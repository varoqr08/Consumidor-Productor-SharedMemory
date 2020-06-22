#include <sys/sem.h>
#include <stdio.h>
#include <stdlib.h>

#if defined(__GNU_LIBRARY__) && !defined(_SEM_SEMUN_UNDEFINED)
// The union is already defined in sys/sem.h
#else
union semun {
  int val;
  struct semid_ds *buf;
  unsigned short int *array;
  struct seminfo *__buf;
};
#endif

/* Init and returns semaphore id */
int init_semaphore(char *buff_name, int buffer_size)
{
  key_t key;
  int id_semaphore;
  union semun arg;

  key = ftok(buff_name, 33);

  if (key == -1)
  {
    printf("Can't get semaphore key\n");
    exit(0);
  }

  id_semaphore = semget(key, buffer_size, 0600);

  if (id_semaphore == -1)
  {
    printf("Can't create semaphore\n");
    exit(0);
  }

  arg.val = 0;
  semctl(id_semaphore, 0, SETVAL, &arg);

  return id_semaphore;
}

/* Create semaphores and initializes it in UP */
void create_semaphore(char *route, int buffer_size)
{
  key_t key_semaphore;
  int id_semaphore;

  key_semaphore = ftok(route, 33);

  if (key_semaphore == -1)
  {
    printf("Can't get semaphore key\n");
    exit(0);
  }

  id_semaphore = semget(key_semaphore, buffer_size, 0600 | IPC_CREAT);

  if (id_semaphore == -1)
  {
    printf("Can't create semaphore\n");
    exit(0);
  }

  struct sembuf operation;

  operation.sem_op = 1;
  operation.sem_flg = 0;

  for (int i = 0; i < buffer_size; ++i)
  {
    operation.sem_num = i;

    semop(id_semaphore, &operation, 1);
  }
}