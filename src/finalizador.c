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
        printc("Numero incorrecto de argumentos. Introducir Nombre del buffer a finalizar.\n", 1);
        exit(0);               
    }

    char *nameBuffer = argv[1];

    printf("Nombre: %s \n", nameBuffer);

    //Creacion de la clave para el buffer
    char * buffDir = concat("buffers/",argv[1]);
    key_t bufferKey;
    global_variables *variables = NULL;
    
    
    if (check_dir(buffDir)) {
        printc("El buffer no existe\n", 1);
        exit(0);
    }else{
        bufferKey =  ftok (buffDir, 's');
        if (bufferKey == -1){
            printc("Error al obtener la clave para la memoria compartida\n", 1);
            exit(0);
            }
    }

    //Obtener variables globales
    int id_gm = 0;
    if (globalMemory(&id_gm, &variables)){
        printc("Error al leer las variables globales\n", 1);
        exit(0);
    }

    //Memoria Compartida
    int mem_id;
    if (crearMemoria(&mem_id, bufferKey, variables[0].size, &buffer)){
        printc("Error al obtener el buffer de memoria compartida.\n", 1);
        exit(0);
    }

    //Abrir los semaforos
    char *dir_name = concat("buffers/", nameBuffer);
    int semMem, semVacio, semLleno;
    semMem = abrirSem(dir_name, 1, 1);
    semVacio = abrirSem(dir_name, 2, 1);
    semLleno = abrirSem(dir_name, 3, 1);

    //Levantar Bandera para cerrar productores y consumidores
    variables[0].end = 1;

    //Borrar Mem Global
    if(borrarMemoria(id_gm)){
        printc("Error al borrar Memoria Global", 1);
    }

    //Borrar Mem Buffers
    if(borrarMemoria(mem_id)){
        printc("Error al borrar Memoria de Buffers", 1);
    }

    //Borrar los Semaforos
    borrarSem(semMem);
    borrarSem(semVacio);
    borrarSem(semLleno);

    //Borrar la Carpeta Creada
    if (!check_dir(dir_name)){
        system(concat("rm -rf ", dir_name));
        printf("Se eliminaron con exito los buffers\n");
    }

    //Envia la variable para finalizar todos los procesos
    variables[0].end = -1;
    sleep(2);
    //MOSTRAR UN MONTON DE INFO XD
    printf("Total de Mensajes Producidos:  %i \n", variables[0].produced);
    //Mensajes en el Buffer
    printf("Total de Productores:  %i \n", variables[0].producers);
    printf("Total de Consumidores:  %i \n", variables[0].consumers);
    printf("Total de Consumidores Eliminados con una Key:  %i \n", variables[0].key_deleted);
    printf("Tiempo total en estado de Espera: %f \n", variables[0].totalWait);
    //Tiempo de Usuario Final
    printf("Tiempo total en estado de Usuario: %f \n", variables[0].totalUser);
    //Tiempo bloqueado Total
    printf("Tiempo total en estado de bloqueo: %f \n", variables[0].totalBloq);
    //Tiempo de Kernel Total
    printf("Tiempo total del Kernel: %f \n", variables[0].totalKernel);

    
    return 0;

}





