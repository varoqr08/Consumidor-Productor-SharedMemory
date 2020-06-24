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
    printf("Media de tiempo Inválido.\n");
    exit(0);
    }

    printf("Nombre: %s \n", nameBuffer);
    printf("Tamano: %f \n", time);

    printf("%f\n", ran_expo(0.05));

    /////////////////////////////////////////////////////////////////////////

}
/*Generacion de números aleatorios a partir de una distribución exponencial
Tomado de StackOverflow: Generating random numbers of exponential distribution
*/
double ran_expo(double lambda){
    double u;
    u = rand() / (RAND_MAX + 1.0);
    return -log(1- u) / lambda;
}