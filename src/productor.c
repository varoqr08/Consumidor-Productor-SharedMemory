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

double ran_expo(double lambda);

int main(int argc, char *argv[]){
    

    //Validacion de argumentos
    if(argc != 3){
        printf("Numero incorrecto de argumentos. Introducir Nombre y Tiempo Medio\n");
        exit(0);               
    }

    char *nameBuffer = argv[1];
    float time = atof(argv[2]);

    if(time <= 0){
    printf("Media de Tiempo Inválido.\n");
    exit(0);
    }

    printf("Nombre: %s \n", nameBuffer);
    printf("Tamano: %f \n", time);

    //Verificar la media de tiempo en segundos
    if (!isNumber(argv[2])){
        printf("Tamano de Buffer invalido. Debe ser un numero entero.\n");
        exit(0);
    }

    printf("%f\n", ran_expo(time));

    /////////////////////////////////////////////////////////////////////////

    //Creacion de la clave para el buffer
    char * buffDir = concat("buffers/",argv[1]);
    key_t bufferKey;
    global_variables *variables = NULL;
    message *buffer = NULL;
    
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

    //Abrir los semaforos
    char *dir_name = concat("buffers/", nameBuffer);
    int semMem, semVacio, semLleno;
    semMem = abrirSem(dir_name, 1, 1);
    semVacio = abrirSem(dir_name, 2, 1);
    semLleno = abrirSem(dir_name, 3, 1);

    printf("SemLleno %i \n", semLleno);
    printf("SemVAcio %i \n", semVacio);
    printf("SemMem %i \n", semMem);

    borrarSem(semMem);
    borrarSem(semVacio);
    borrarSem(semLleno);

/* ipcs -s - Ver Semaforos */
/* ipcrm -s 0 - Borrar Semaforo con id 0 */


/*
while(1){
    float m = ran_expo(time);
    printf("Espera de: %f \n",m);
    sleep(m);
    

for(int i=1; i <=semVacio; i++){
    borrarSem(i);
}}*/
}

/*Generacion de números aleatorios a partir de una distribución exponencial
Tomado de StackOverflow: Generating random numbers of exponential distribution
*/
double ran_expo(double lambda){
    double u;
    u = rand() / (RAND_MAX + 1.0);
    return -log(1- u) / lambda;
}
