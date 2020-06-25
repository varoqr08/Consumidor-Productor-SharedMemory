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

double ran_expo(double lambda);

void writeMemory(int i);

void printMemory(int i, int magicNumber);


message *buffer = NULL;

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
    printf("Tiempo: %f \n", time);

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

    //writeMemory(1);
    //writeMemory(2);
    /*int semV = sem_get_value(semVacio, 0);
    printf("Prueba %i \n", semV);

    bajarSem(semVacio, 0);

    int semV1 = sem_get_value(semVacio, 0);
    printf("Prueba %i \n", semV1);
*/
    

/* ipcs -s - Ver Semaforos */
 //ipcrm -s 0 - Borrar Semaforo con id 0 



    int i =0;
    
    while(1){
        float m = ran_expo(time);
        printf("Espera de: %f \n",m);
        sleep(m);

        subirSem(semVacio,0);


        //Entra en memoria compartida
        bajarSem(semMem, 0);        
        

        //readMemory(i); ???????????


        /*
        buffer[i].active = 0;
        buffer[i].pid = 0;
        buffer[i].magic_number = 0;
        strcpy(buffer[i].date, "");
        strcpy (buffer[i].hour, "");
        strcpy (buffer[i].text, "Hola");
        */
        //printf("Se");

        i++;

        if(i == 10){
            exit(0);
        }

        sleep(8);
        subirSem(semMem, 0);
        bajarSem(semLleno,0);

    }   
        
}


/*Generacion de números aleatorios a partir de una distribución exponencial
Tomado de StackOverflow: Generating random numbers of exponential distribution
*/
double ran_expo(double lambda){
    double u;
    u = rand() / (RAND_MAX + 1.0);
    return -log(1- u) / lambda;
}

//void write_msg(int data1, int data2, char *data3, int index, struct sembuf operation, int id, message *memory, global_variables *memory2, int buffer_size){
void writeMemory(int i){

    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    char date[64];
    strftime(date, sizeof(date), "%c", tm);
    int magicNumber = rand() % 6;

    buffer[i].active = 1;
    buffer[i].pid = getpid();
    buffer[i].magic_number = magicNumber;
    strcpy(buffer[i].date, date);
    //Recordar a Mau que se puede poner fecha y hora en uno solo. Borrar el atributo hour.
    strcpy (buffer[i].hour, "");
    strcpy (buffer[i].text, "Hola");


    printMemory(i, magicNumber);
}

void printMemory(int i, int magicNumber){
    printf("\n");
    printf("Informacion de Mensaje en el espacio de memoria %i : \n", i);
    printf("Uso de la memoria: %i \n", buffer[i].active);
    printf("Pid: %i \n", buffer[i].pid);
    printf("Valor de Magic Number asociado: %i \n", buffer[i].magic_number);
    printf("Fecha y hora de creacion: %s \n", buffer[i].date);
    printf("Texto del mensaje: %s \n", buffer[i].text);
    printf("\n");


}