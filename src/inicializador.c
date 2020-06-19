#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>



int main(int argc, char *argv[])
{
    if(argc != 3){
        //printf("El resultado de la suma es :%f\n", suma (a, b));
        printf("Uso incorrecto del llamado\n");
        exit(0);
    }



    char *nameBuffer = argv[1];
    int sizeBuffer = atoi(argv[2]);


    if(sizeBuffer == 0){
        printf("Tamano de Buffer invalido. Debe ser un entero.\n");
        exit(0);
    }

    printf("Nombre: %s \n", nameBuffer);
    printf("Tamano: %i \n", sizeBuffer);

    return 0;
}