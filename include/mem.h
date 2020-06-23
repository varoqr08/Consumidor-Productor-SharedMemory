#ifndef _MEM_H
#define _MEM_H

#include <stdio.h>
#include <time.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <struct.h>

/* Library to create and manage shared memory */

int crearMemoria(int *shmid, key_t key, int size, message **Memoria);
int globalMemory(global_variables **memoria);

#endif