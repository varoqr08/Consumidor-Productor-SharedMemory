#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <unistd.h>
#include <sys/sem.h>
#include <string.h>

int main(int argc, char *argv[])
{
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

    //Inicializacion de memoria
    key_t key;
    key = ftok(nameBuffer, 33);

    printf("Llave: %i \n", key);

    return 0;
}
