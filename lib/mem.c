#include "../include/mem.h"

/* Creates the buffer for shared memory */
int create_buffer(int *id, key_t mem_key, int mem_size)
{
  *id = shmget(mem_key, sizeof(message) * mem_size, 0777 | IPC_CREAT);
  if (*id == -1)
  {
    printf("Error en el get del buffer\n");
    return 0;
  }

  return 1;
}

/* Returns the buffer id for shared memory */
int get_buffer(int *id, key_t mem_key, int mem_size)
{
  *id = shmget(mem_key, sizeof(message) * mem_size, 0777);
  if (*id == -1)
  {
    printf("Error en el get del buffer\n");
    return 0;
  }

  return 1;
}

/* Returns the buffer memory for shared memory */
int get_buffer_memory(int *id, message **memory)
{
  *memory = (message *)shmat(*id, (char *)0, 0);
  if (*memory == NULL)
  {
    printf("Error en el mat del buffer\n");
    return 0;
  }

  return 1;
}

/* Creates the global variables for shared memory */
int create_global(int *id, key_t mem_key)
{
  *id = shmget(mem_key, 1 * sizeof(global_variables), 0777 | IPC_CREAT);
  if (*id == -1)
  {
    printf("Error en el get del buffer\n");
    return 0;
  }

  return 1;
}

/* Returns the global variables id for shared memory */
int get_global(int *id, key_t mem_key)
{
  *id = shmget(mem_key, 1 * sizeof(global_variables), 0777);
  if (*id == -1)
  {
    printf("Error en el get del buffer\n");
    return 0;
  }

  return 1;
}

/* Returns the global variables memory for shared memory */
int get_global_memory(int *id, global_variables **memory)
{
  *memory = (global_variables *)shmat(*id, (char *)0, 0);
  if (*memory == NULL)
  {
    printf("Error en el mat del buffer\n");
    return 0;
  }

  return 1;
}