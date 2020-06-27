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
        printc("Numero incorrecto de argumentos. Introducir Nombre y Tamaño del Buffer\n", 1);
        exit(0);
    }
    
    char *nameBuffer = argv[1];
    int sizeBuffer = atoi(argv[2]);

    //Verfica que el tamano del buffer sea un valor valido.
    if(sizeBuffer <= 0){
        printc("Tamaño de Buffer invalido. Debe ser positivo y diferente de cero.\n", 1);
        exit(0);
    }

    //Verifica que el tamaño del buffer sea un numero.
    if (!isNumber(argv[2])){
        printc("Tamaño de Buffer invalido. Debe ser un numero entero.\n", 1);
        exit(0);
    }

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
    }else{
        printc("El Nombre de Buffer seleccionado ya existe\n", 1);
        exit(0);
    }

    
    //Creacion de buffer LS
    if (check_dir("buffers/ls")){
        if (make_dir("buffers/ls")){
            printf("Error al crear carpeta: %s\n", "buffers/ls");
        }else
            printf("Se ha creado la carpeta: %s\n", "buffers/ls");
    }

    //Creacion de llaves.
    key_t key = ftok(dir_name, 's');

    if(key == -1){
        printc("Error en la creacion de la llave \n", 1);
        exit(0);
    }

    //Creacion de la memoria.
    int mem_id;
    message *memoria = NULL;
    if (crearMemoria(&mem_id, key, sizeBuffer, &memoria)){
        printc("Error al crear la memoria \n", 1);
        exit(0);
    }else{
        printf("Se generó el buffer %s de tamaño %d correctamente\n", nameBuffer, sizeBuffer);
    }


    //Imprime argumentos validos en consola.
    printf("Nombre: %s \n", nameBuffer);
    printf("Tamaño: %i \n", sizeBuffer);


    //Creacion de memoria para variables globales.
    global_variables *memoriaGlobal = NULL;
    int id_gm = 0;
    if(globalMemory(&id_gm, &memoriaGlobal)){
        printc("Error en variables globales.\n", 1);
        exit(0);
    }
 
    memoriaGlobal[0].producers = 0;
    memoriaGlobal[0].consumers = 0;
    memoriaGlobal[0].size = 0;
    memoriaGlobal[0].id = mem_id;
    memoriaGlobal[0].end = 0;
    memoriaGlobal[0].consumed = 0;
    memoriaGlobal[0].produced = 0;
    memoriaGlobal[0].key_deleted = 0;
    memoriaGlobal[0].totalWait = 0;
    memoriaGlobal[0].totalBloq = 0;
    memoriaGlobal[0].totalUser = 0;
    memoriaGlobal[0].totalKernel = 0;

    //Semaforos
    int semMem, semVacio, semLleno;
    
    //Borrar los Semaforos
    borrarSemIni(semMem);
    borrarSemIni(semVacio);
    borrarSemIni(semLleno);

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
