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

void readMemory(int i, float m, int n);

void printTimes(float waitTime, float userTime);

void endConsumer(float userTime);



message *buffer = NULL;    
global_variables *variables = NULL;

int main(int argc, char *argv[]){
    
    //Validacion de argumentos
    if(argc != 4){
        printc("Numero incorrecto de argumentos. Introducir Nombre, Tiempo Medio y Modo de Operacion (1 o 0)\n", 1);
        exit(0);               
    }

    char *nameBuffer = argv[1];
    float seconds = atof(argv[2]);
    int modo = atoi(argv[3]);

    if(time <= 0){
        printc("Media de Tiempo Inválido.\n", 1);
        exit(0);
    }
    

    //Verificar la media de tiempo en segundos
    if (!isNumber(argv[2])){
        printc("Media de tiempo invalida. Debe ser un numero entero.\n", 1);
        exit(0);
    }

    if (!isNumber(argv[3])){
        printc("Modo de operacion invalido. Debe ser 0 o 1.\n", 1);
        exit(0);
    }

    
    /////////////////////////////////////////////////////////////////////////

    //Creacion de la clave para el buffer
    char * buffDir = concat("buffers/",argv[1]);
    key_t bufferKey;

    
    
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

    variables[0].consumers ++;
    variables[0].totalConsumers ++;

///////////////////////////////////////////////////////////////////////////////////////
    printc("------------------------------------------ \n", 2);
    printc("CONSUMIDOR\n",5);
    printc("Nombre: ",5);
    printf("%s \n", nameBuffer);
    printc("Tiempo Medio en Segundos: ", 5);
    printf("%f \n", seconds);
    
    
    if(modo == 0){
        printc("Modo de operacion: Manual. \n", 5);
    }else{
        if(modo == 1){
           printc("Modo de operacion: Automatico. \n", 5);
        }else{
            printc("Modo Invalido. Ingrese 0 para Manual o 1 para Automatico. \n", 1);
            exit(0);
        }
    }
    
    
    //Abrir los semaforos
    char *dir_name = concat("buffers/", nameBuffer);
    int semMem, semVacio, semLleno;
    semMem = abrirSem(dir_name, 1, 1);
    semVacio = abrirSem(dir_name, 2, 1);
    semLleno = abrirSem(dir_name, 3, 1);

    //printf("SemLleno %i \n", semLleno);
    //printf("SemVAcio %i \n", semVacio);
    //printf("SemMem %i \n", semMem);

    int cons = 1;
    int n = 1;
    float tiempoEspera;
    struct timeval t1, t2, t3, t4, start, end;

    float p = abs(poisson(seconds));
    //printf("Poisson: %f \n", p);

    printc("Pid  del Consumidor = ",5);
    printf("%i \n", getpid());
    printc("Pid mod 6 = ", 5);
    printf("%i \n", (getpid()) % 6);

    printc("------------------------------------------ \n", 2);
    

    gettimeofday(&start, NULL);

    while(1){
        printc("------------------------------------------ \n", 4);
        if(!modo){
            printc("Precione la tecla Enter para consumir\n", 3);
            getchar();
        }

        if(variables[0].end == -1){
            gettimeofday(&end, NULL);
            float userTime = (end.tv_sec - start.tv_sec);
            variables[0].totalUser += (end.tv_sec - start.tv_sec);
            endConsumer(userTime);
        }
        
        
        float m = abs(poisson(seconds));
        sleep(m);
        
        //No permite que otro consumidor entre en el mismo espacio
        gettimeofday(&t3, NULL);
        gettimeofday(&t1, NULL);
        bajarSem(semLleno,0);
        gettimeofday(&t2, NULL);

        //Entra en memoria compartida
        bajarSem(semMem, 0);  
        gettimeofday(&t4, NULL);      
        
        //Leer de Memoria//
        int id = (getpid() % 6);

        variables[0].size --;
        variables[0].totalWait += (t4.tv_sec - t3.tv_sec);
        variables[0].totalBloq += (t2.tv_sec - t1.tv_sec);

        tiempoEspera += (t4.tv_sec - t3.tv_sec);
        
        if(id == buffer[variables[0].size].magic_number){
            n = 3;
            readMemory(variables[0].size, m, n);
            gettimeofday(&end, NULL);
            float tiempoUsuario = (end.tv_sec - start.tv_sec);
            variables[0].totalUser += (end.tv_sec - start.tv_sec);
            printf("\n");
            printc("El Magic Number Coincide con el PID modulo 6!!\n", 1);
            printf("\n");
            printc("Consumidor Finalizado.\n", 5);
            printf("\n");
            printf("Datos de interes:\n");
            printc("-> ",5);
            printf("Id del consumidor: %i \n", getpid());
            printc("-> ",5);
            printTimes(tiempoEspera,tiempoUsuario);
            variables[0].consumed ++;
            variables[0].consumers --;
            variables[0].key_deleted ++;
            printc("-> ",5);
            printf("Mensajes Totales Consumidos: %i \n" , variables[0].consumed);
            printc("-> ",5);
            printf("Mensajes Consumidos por este consumidor: %i \n" , cons);
            
            //Permite que se acceda nuevamente a memoria compartida
            subirSem(semMem, 0);

            //Se agrega el espacio de memoria liberado a los espacios disponibles
            subirSem(semVacio,0);

            exit(0);
        }
        
        readMemory(variables[0].size, m, n);

        //Cantidad de mensajes consumidos por este consumidor
        cons ++;
        //Aumenta mensajes totales consumidas
        variables[0].consumed ++;


        subirSem(semMem, 0);

        subirSem(semVacio,0);
        sleep(1);
    }  
}


    //void write_msg(int data1, int data2, char *data3, int index, struct sembuf operation, int id, message *memory, global_variables *memory2, int buffer_size){
void readMemory(int i, float m, int n){
    printf("\n");
    printf("Espera de: %f \n",m);
    printf("\n");
    printc("-> ",2);
    printf("Productores Activos: %i \n", variables[0].producers);
    printc("-> ",2);
    printf("Consumidores Activos: %i \n", variables[0].consumers);
    printc("-> ",2);
    printf("Leido del Bloque de memoria # %i \n", i);
    printc("-> ",2);
    printf("Contenido del mensaje leido: \n");
    printc("-> ",2);
    printf("Uso de la memoria: %i \n", buffer[i].active);
    printc("-> ",2);
    printf("Pid del productor creador: %i \n", buffer[i].pid);
    printc("-> ",2);
    printf("Valor de Magic Number asociado: %i", buffer[i].magic_number);
    printc("<- \n",n);
    printc("-> ",2);
    printf("Fecha y hora de creacion: %s \n", buffer[i].date);
    printc("-> ",2);
    printf("Texto del mensaje: %s \n", buffer[i].text);
    printf("\n");

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

void printTimes(float waitTime, float userTime){
    printf("Tiempo de Espera: %f \n", waitTime);
    printc("-> ",5);
    printf("Tiempo de Usuario: %f \n", userTime);
}

void endConsumer(float userTime){
    printf("\n");
    printc("Solicitud del Finalizador Recibida!! \n",1);
    printf("\n");
    printc("---Consumidor Finalizado--- \n", 5);
    printc("-> ",2);
    printf("Tiempo de Usuario: %f  \n", userTime);
    exit(0);
}