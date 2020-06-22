#ifndef _MEM_H
#define _MEM_H

#include <stdio.h>
#include <time.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <struct.h>

/* Library to create and manage shared memory */

int create_buffer(int *id, key_t mem_key, int mem_size);
int create_global(int *id, key_t mem_key);
int get_buffer(int *id, key_t mem_key, int mem_size);
int get_global(int *id, key_t mem_key);
int get_buffer_memory(int *id, message **memory);
int get_global_memory(int *id, global_variables **memory);

#endif