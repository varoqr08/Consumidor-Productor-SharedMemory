#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <unistd.h>
#include <sys/sem.h>
#include <string.h>
#include <semaphore.h>
#include <struct.h>
#include <aux.h>
#include <mem.h>

int main(int argc, char *argv[])
{
    int id_memory;
    key_t key;
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
    
    /////////////////////////////////////////////////////////////////////////

    /* Check and init directories */
    create_dirs(nameBuffer);

    /* Shared Memory Buffer Initialization */
    char *key_route;
    if (check_bin_dir())
    {
        key_route = concat("../share_files/", nameBuffer);
    }
    else
    {
        key_route = concat("share_files/", nameBuffer);
    }

    check_dir(key_route);

    key = ftok(key_route, 33);
    if (key == -1)
    {
        printf("Buffer Key is Invalid\n");
        exit(0);
    }

    if (create_buffer(&id_memory, key, sizeBuffer) == 0)
    {
        printf("Can't create buffer memory\n");
        exit(0);
    }

    /* Shared Memory for Global Variables Initialization */
    if (check_bin_dir())
    {
        key_global = ftok("../share_files/global", 33);
    }
    else
    {
        key_global = ftok("share_files/global", 33);
    }

    if (key_global == -1)
    {
        printf("Global Variables Key is Invalid\n");
        exit(0);
    }

    if (create_global(&gv_shm_id, key_global) == 0)
    {
        printf("Can't create global memory\n");
        exit(0);
    }

    if (get_global_memory(&gv_shm_id, &memory2) == 0)
    {
        printf("Can't get global memory\n");
        exit(0);
    }

    memory2->size = sizeBuffer;

    /* Create Semaphore */

    if (check_bin_dir())
    {
        create_semaphore("../share_files/sem", sizeBuffer);
    }
    else
    {
        create_semaphore("share_files/sem", sizeBuffer);
    }





    return 0;
}
