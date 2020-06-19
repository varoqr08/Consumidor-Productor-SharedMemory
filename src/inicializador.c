#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>



int main(int argc, char *argv[])
{
    if(argc != 3){
        printf("Numero incorrecto de argumentos. Introducir Nombre y Tamano del Buffer\n");
        exit(0);
    }



    char *nameBuffer = argv[1];
    int sizeBuffer = atoi(argv[2]);


    if(sizeBuffer == 0){
        printf("Tamano de Buffer invalido. Debe ser un numero entero.\n");
        exit(0);
    }

    printf("Nombre: %s \n", nameBuffer);
    printf("Tamano: %i \n", sizeBuffer);

    return 0;
}
