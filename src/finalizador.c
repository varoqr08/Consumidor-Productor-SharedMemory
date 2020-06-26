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
#include <math.h>
#include <time.h>
#include <sys/time.h>

message *buffer = NULL;

int main(int argc, char *argv[]){
    
    //Validacion de argumentos
    if(argc != 2){
        printf("Numero incorrecto de argumentos. Introducir Nombre.\n");
        exit(0);               
    }

    char *nameBuffer = argv[1];

    printf("Nombre: %s \n", nameBuffer);

    //Verificar la media de tiempo en segundos
    if (!isNumber(argv[2])){
        printf("Tamano de Buffer invalido. Debe ser un numero entero.\n");
        exit(0);
    }

    //Creacion de la clave para el buffer
    char * buffDir = concat("buffers/",argv[1]);
    key_t bufferKey;
    global_variables *variables = NULL;
    
    
    if (check_dir(buffDir)) {
        printf("El buffer no existe\n");
        exit(0);
    }else{
        bufferKey =  ftok (buffDir, 's');
        if (bufferKey == -1){
            printf("Error al obtener la clave para la memoria compartida\n");
            exit(0);
            }
    }

    //Obtener variables globales
    if (globalMemory(&variables)){
        printf("Error al leer las variables globales\n");
        exit(0);
    }

    //Memoria Compartida
    int mem_id;
    if (crearMemoria(&mem_id, bufferKey, variables[0].size, &buffer)){
        printf("Error al obtener el buffer de memoria compartida.\n");
        exit(0);
    }

    variables[0].end = 1;

    
    //Abrir los semaforos
    char *dir_name = concat("buffers/", nameBuffer);
    int semMem, semVacio, semLleno;
    semMem = abrirSem(dir_name, 1, 1);
    semVacio = abrirSem(dir_name, 2, 1);
    semLleno = abrirSem(dir_name, 3, 1);
 
    //Borrar los Semaforos
    borrarSem(semMem);
    borrarSem(semVacio);
    borrarSem(semLleno);


}





