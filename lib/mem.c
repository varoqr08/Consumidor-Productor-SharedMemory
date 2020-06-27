#include <stdio.h>
#include <time.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <aux.h>

#include "../include/mem.h"

#define ARRAY_SIZE   4000
#define MALLOC_SIZE 10000
#define SHM_SIZE    10000
#define SHM_MODE    (SHM_R | SHM_W)     /* read/write */

/*    Creacion de la memoria compartida.
*    - El manejador de memoria es interno
*    - Manda mensajes de error por salida de error estandar.
*/
int crearMemoria(int *shmid, key_t key, int size, message **Memoria){
  if ((*shmid = shmget(key, sizeof(message)*size, 0777 | IPC_CREAT)) < 0) {
    printc("Error de shmget() en el buffer\n", 1);
    return 1;
   } 
   if ((*Memoria=( message *)shmat(*shmid, (char *)0, 0)) == (void *) -1) {
    printc("Error de shmat() en el buffer\n", 1);
    return 1;
   }
   return 0;
}

/* Creacion de la memoria compartida para variables globales */
int globalMemory(int *shmid, global_variables **memoria){
  key_t key = ftok ("/bin", 33);
	if (key == -1){
		printc("Error en la clave de las variables globales\n", 1);
		return 1;
	}
  if ((*shmid=shmget(key, sizeof(global_variables), 0777 | IPC_CREAT))<0) {
    printc("Error de shmget() en el buffer de las variables globales\n", 1);
    return 1;
   } 
   if ((*memoria=( global_variables *)shmat(*shmid, (char *)0, 0)) == (void *) -1) {
    printc("Error de shmat() en el buffer de las variables globales\n", 1);
    return 1;
   }
   return 0;
}

/* Borrar Memoria: 1:error, 0:funciono */
int borrarMemoria(int shmid){
  if (shmctl(shmid, IPC_RMID, 0) < 0) {
    return 1;
  } else
    return 0;
}