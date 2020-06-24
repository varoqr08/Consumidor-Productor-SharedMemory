#include <sys/shm.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <string.h>
#include <errno.h>

struct sembuf operation;
/**
 * Creacion del semaphore 
 * char* buffer --> key 
 * int identifier --> identificador
 * int sem_num --> semaforos por crear
 * int init_val --> Valor inicial, 0 is rojo y 1 verde
 * Example: sem_create("../bin/ls",33, 10,1);
*/
int crearSem(char *buffer, int identifier, int sem_num, int init_val) {
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

    
    union semun arg;
    int id;
    key_t key;

    key = ftok(buffer, identifier);

    if (key == -1){
        printf("No se pudo obtener la llave del semaforo\n");
        exit(0);
    }

    id = semget(key, sem_num, 0600 | IPC_CREAT);

    if (id == -1){
        printf("No se pudo crear el semaforo\n");
        exit(0);
    }

    for (int i = 0; i <= sem_num; i++){
      arg.val = init_val;
      semctl(id,i, SETVAL, arg);
    } 

    operation.sem_flg = 0;

    return id;
}

/**
 * Funcion para borrar un semaforo
 * int id --> id del set de semaforos
*/
void borrarSem(int id) {
    semctl(id, 2, IPC_RMID, NULL);
    if (errno == EINVAL){
        printf("No se pudo borrar el semaforo\n");
    }else{
        printf("Semaforo cerrado %d\n", id);
    }
}

/**
 * Funcion para abrir un semaforo existente
 * char* buffer --> key 
 * int identifier --> identificador
 * int sem_num --> numero de semaforos
 * Example: sem_open("../bin/ls",33, 10);
*/

int abrirSem(char *buffer, int identifier, int sem_num) {
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

    //union semun arg;
    int id;
    key_t key;

    key = ftok(buffer, identifier);

    if (key == -1){
        printf("No se pudo obtener la llave en abrirSem\n");
        exit(0);
    }

    id = semget(key, sem_num, 0600);

    if (id == -1){
        printf("No se pudo abrir el semaforo en abrirSem\n");
        exit(0);
    }

    operation.sem_flg = 0;

    return id;
}

/**
 * Up operation 
 * int id --> id del set se semaforos
 * int num --> numero especifico del semaforo
 * Example: sem_up(sem1,0);
*/
void verdeSem(int id, int num) {
    struct sembuf operation;

    operation.sem_num = num;
    operation.sem_op = 1;
    semop(id, &operation, 1);
}
/**
 * Down operation 
 * int id --> id del set se semaforos
 * int num --> umero especifico del semaforo
 * Example: sem_down(sem1,0);
*/
void rojoSem(int id, int num) {
    struct sembuf operation;

    operation.sem_num = num;
    operation.sem_op = -1;
    semop(id, &operation, 1);
}

/**
 * Get the value of the especific semaphore 
 * int id --> id of the set of semaphores
 * int num --> number of the especific semaphore
 * Example: sem_get_value(sem1,0);
*/
int sem_get_value(int id, int num) {
    int value;
    value = semctl(id, num, GETVAL, 0);
    return value;
}
