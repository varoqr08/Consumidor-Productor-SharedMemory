#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <unistd.h>
#include <sys/sem.h>
#include <string.h>
#include <semaphore.h>
#include <struct.h>

int main(int argc, char *argv[])
{
    int id_memory;

    key_t key_global;
    int gv_shm_id;
    global_variables *memory2 = NULL;


    //Validacion de argumentos
    if(argc != 3){
        printf("Numero incorrecto de argumentos. Introducir Nombre y Tamano del Buffer\n");
        exit(0);
    }

    char *nameBuffer = argv[1];
    int sizeBuffer = atoi(argv[2]);

    if(sizeBuffer <= 0){
        printf("Tamano de Buffer invalido. Debe ser un numero entero.\n");
        exit(0);
    }

    printf("Nombre: %s \n", nameBuffer);
    printf("Tamano: %i \n", sizeBuffer);
    
    

    return 0;
}
