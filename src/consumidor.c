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

int factorial(int num);

double poisson(int k);

void readMemory(int i, float m);

void printMemory(int i, int magicNumber, float m);


message *buffer = NULL;

int main(int argc, char *argv[]){
    
    //Validacion de argumentos
    if(argc != 4){
        printf("Numero incorrecto de argumentos. Introducir Nombre, Tiempo Medio y Modo de Operacion (1 o 0)\n");
        exit(0);               
    }

    char *nameBuffer = argv[1];
    float seconds = atof(argv[2]);

    if(time <= 0){
    printf("Media de Tiempo Inválido.\n");
    exit(0);
    }

    printf("Nombre: %s \n", nameBuffer);
    printf("Tiempo: %f \n", seconds);

    //Verificar la media de tiempo en segundos
    if (!isNumber(argv[2])){
        printf("Tamano de Buffer invalido. Debe ser un numero entero.\n");
        exit(0);
    }

    
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

    variables[0].consumers ++;
    
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

    int cons = 1;
    int tiempoEspera = 0;
    int mediaTiempo = 0;
    struct timeval t1, t2, t3, t4, start, end;

    float p = abs(poisson(seconds));
    printf("Poisson: %f \n", p);

    printf("Pid = %i \n", getpid());
    printf("Pid mod 6 = %i \n", (getpid()) % 6);
    
    while(1){
        float m = abs(poisson(seconds));
        tiempoEspera += m;
        sleep(m);
        
        bajarSem(semLleno,0);


        //Entra en memoria compartida
        bajarSem(semMem, 0);        
        
        //Leer de Memoria//
        int id = (getpid() % 6);

        variables[0].size --;
        
        if(id == buffer[variables[0].size].magic_number){
            printf("El Magic Number Coincide con el PID modulo 6.\n");
            printf("El proceso del Consumidor se va a cerrar.\n");
            printf("Datos de interes:\n");
            printf("Id del consumidor: %i \n", getpid());
            printf("Acumulado de tiempos esperados: %i \n", tiempoEspera);

            //variables[0].size --;
            variables[0].consumed ++;
            printf("Mensajes Totales Consumidos: %i \n" , variables[0].consumed);
            printf("Mensajes Consumidos por este consumidor: %i \n" , cons);
            

            subirSem(semMem, 0);
            subirSem(semVacio,0);

            exit(0);
        }
        
        //readMemory(variables[0].size, m);

        //int semV1 = sem_get_value(semVacio, 0);
        //printf("Espacios restantes: %i \n", semV1);
        
        //i++;
        cons ++;
        variables[0].consumed ++;
        subirSem(semMem, 0);
        subirSem(semVacio,0);
        sleep(1);
    }  
}


    //void write_msg(int data1, int data2, char *data3, int index, struct sembuf operation, int id, message *memory, global_variables *memory2, int buffer_size){
void readMemory(int i, float m){

    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    char date[64];
    strftime(date, sizeof(date), "%c", tm);
    int magicNumber = rand() % 6;



/*
    buffer[i].active = 1;
    buffer[i].pid = getpid();
    buffer[i].magic_number = magicNumber;
    strcpy(buffer[i].date, date);
    //Recordar a Mau que se puede poner fecha y hora en uno solo. Borrar el atributo hour.
    strcpy (buffer[i].hour, "");
    strcpy (buffer[i].text, "Hola");

*/
    printMemory(i, magicNumber, m);
} 
        


/*Generacion de números aleatorios a partir de una distribución de poisson*/
int factorial(int num){
    int fact = 1;
    for (int i = 1; i <= num; ++i){
        fact = fact * i;
    }
    return fact;
}

double poisson(int k){
    double e = 2.7182818284590452353602874713527;
    double lamda = 1;
    //printf("F: %d", factorial(k));
    //return factorial(k); 
    return pow(lamda, k) * pow(e,-lamda) / factorial(k);      
}



void printMemory(int i, int magicNumber, float m){
    printf("\n");
    printf("Espera de: %f \n",m);
    printf("\n");
    printf("Informacion de Mensaje en el espacio de memoria %i : \n", i);
    printf("Uso de la memoria: %i \n", buffer[i].active);
    printf("Pid: %i \n", buffer[i].pid);
    printf("Valor de Magic Number asociado: %i \n", buffer[i].magic_number);
    printf("Fecha y hora de creacion: %s \n", buffer[i].date);
    printf("Texto del mensaje: %s \n", buffer[i].text);
    printf("\n");

}