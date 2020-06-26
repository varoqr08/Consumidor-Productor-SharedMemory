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

int main(int argc, char *argv[]){

    /*Validacion de argumentos*/

    //Verifica que ingresen todos los argumentos esperados.
    if(argc != 3){
        printf("Numero incorrecto de argumentos. Introducir Nombre y Tamaño del Buffer\n");
        exit(0);
    }
    
    char *nameBuffer = argv[1];
    int sizeBuffer = atoi(argv[2]);

    //Verfica que el tamano del buffer sea un valor valido.
    if(sizeBuffer <= 0){
        printf("Tamaño de Buffer invalido. Debe ser positivo y diferente de cero.\n");
        exit(0);
    }

    //Verifica que el tamaño del buffer sea un numero.
    if (!isNumber(argv[2])){
        printf("Tamaño de Buffer invalido. Debe ser un numero entero.\n");
        exit(0);
    }

    //Imprime argumentos validos en consola.
    printf("Nombre: %s \n", nameBuffer);
    printf("Tamaño: %i \n", sizeBuffer);

    //Creacion de directorios.
    char *dir = "buffers";

    if (check_dir(dir)){
        if (make_dir(dir)){
            printf("Error al crear carpeta: %s\n", dir);
        }else
            printf("Se ha creado la carpeta: %s\n", dir);
    }

    char *dir_name = concat("buffers/", nameBuffer);
    
    if (check_dir(dir_name)){
        if (make_dir(dir_name)){
            printf("Error al crear carpeta: %s\n", dir_name);
        }else
            printf("Se ha creado la carpeta: %s\n", dir_name);
    }

    //Creacion de llaves.
    key_t key = ftok(dir_name, 's');

    if(key == -1){
        printf("Error en la creacion de la llave");
        exit(0);
    }

    //Creacion de la memoria.
    int mem_id;
    message *memoria = NULL;
    if (crearMemoria(&mem_id, key, sizeBuffer, &memoria)){
        printf("Error al crear la memoria");
        exit(0);
    }else{
        printf("Se generó el buffer %s de tamaño %d correctamente\n", nameBuffer, sizeBuffer);
    }

    //Creacion de memoria para variables globales.
    global_variables *memoriaGlobal = NULL;
    if(globalMemory(&memoriaGlobal)){
        printf("Error en variables globales.\n");
        exit(0);
    }

    memoriaGlobal[0].producers = 0;
    memoriaGlobal[0].consumers = 1;
    memoriaGlobal[0].size = 0;
    memoriaGlobal[0].id = mem_id;
    memoriaGlobal[0].end = 0;

    //Semaforos
    int semMem, semVacio, semLleno;
    
    //Borrar los Semaforos
    borrarSem(semMem);
    borrarSem(semVacio);
    borrarSem(semLleno);

    //Inicializacion de los Semaforos
 
    semMem = crearSem(dir_name, 1, 1, 1);
    semVacio = crearSem(dir_name, 2, 1, sizeBuffer);
    semLleno = crearSem(dir_name, 3, 1, 0);

    //Inicializacion de Memoria//
    
    for (int i = 0; i < sizeBuffer; i++){
        memoria[i].active = 0;
        memoria[i].pid = 0;
        memoria[i].magic_number = 0;
        strcpy(memoria[i].date, "");
        strcpy (memoria[i].hour, "");
        strcpy (memoria[i].text, "");
    }   

}
