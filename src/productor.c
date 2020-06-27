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

void writeMemory(int i, float m);

void printMemory(int i, int magicNumber, float m);

void endProducer(float tiempo, int msg, float bloq, float wait);


message *buffer = NULL;
global_variables *variables = NULL;
struct timeval t1, t2, t3, t4, start, end;

int main(int argc, char *argv[]){
    

    //Validacion de argumentos
    if(argc != 3){
        printc("Numero incorrecto de argumentos. Introducir Nombre y Tiempo Medio\n", 1);
        exit(0);               
    }

    char *nameBuffer = argv[1];
    float seconds = atof(argv[2]);

    if(time <= 0){
    printc("Media de Tiempo Inválido.\n", 1);
    exit(0);
    }


    //Verificar la media de tiempo en segundos
    if (!isNumber(argv[2])){
        printc("Media de tiempo invalida. Debe ser un numero entero.\n", 1);
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
/////////////////////////////////////////////////////////////////////////////////////

    printf("Nombre: %s \n", nameBuffer);
    printf("Tiempo medio en segundos: %f \n", seconds);


    float bloq, wait;

    variables[0].producers ++;

    //Abrir los semaforos
    char *dir_name = concat("buffers/", nameBuffer);
    int semMem, semVacio, semLleno;
    semMem = abrirSem(dir_name, 1, 1);
    semVacio = abrirSem(dir_name, 2, 1);
    semLleno = abrirSem(dir_name, 3, 1);

    printf("SemLleno %i \n", semLleno);
    printf("SemVAcio %i \n", semVacio);
    printf("SemMem %i \n", semMem);

    int msg = 0;
    gettimeofday(&start, NULL); //inicia el contador de tiempo
    
    while(1){

        if(variables[0].end == -1){
            gettimeofday(&end, NULL);
            double tiempo = (end.tv_sec - start.tv_sec);
            variables[0].totalKernel += (end.tv_sec - start.tv_sec);
            endProducer(tiempo, msg, bloq, wait);
        }


        float m = ran_expo(seconds);
        sleep(m);

        //Disminuye el valor de memoria en que se puede escribir. Si ya no hay espacio, se bloquea el productor
        gettimeofday(&t3, NULL);
        bajarSem(semVacio,0);
        gettimeofday(&t4, NULL);
      
        //Entra en memoria compartida
        gettimeofday(&t1, NULL);
        bajarSem(semMem, 0); 
        gettimeofday(&t2, NULL);       
        

        writeMemory(variables[0].size, m);

        int semV1 = sem_get_value(semVacio, 0);
        printf("Espacios restantes: %i \n", semV1);
        
        variables[0].size ++;
        variables[0].produced ++;
    
        //Permite acceder nuevamente a memoria compartida
        subirSem(semMem, 0);

        //Permite la entrada de consumidores, ya existe algo para consumir.
        subirSem(semLleno,0);

        bloq += (t2.tv_sec - t1.tv_sec);
        wait += (t4.tv_sec - t3.tv_sec);

        variables[0].totalBloq += (t2.tv_sec - t1.tv_sec);
        variables[0].totalWait += (t4.tv_sec - t3.tv_sec);


        msg ++;
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
void writeMemory(int i, float m){

    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    char date[64];
    strftime(date, sizeof(date), "%c", tm);
    int magicNumber = rand() % 7;

    buffer[i].active = 1;
    buffer[i].pid = getpid();
    buffer[i].magic_number = magicNumber;
    strcpy(buffer[i].date, date);
    //Recordar a Mau que se puede poner fecha y hora en uno solo. Borrar el atributo hour.
    strcpy (buffer[i].hour, "");
    strcpy (buffer[i].text, "Hola");


    printMemory(i, magicNumber, m);
}

void printMemory(int i, int magicNumber, float m){
    printf("\n");
    printf("Espera de: %f \n",m);
    printf("\n");
    printf("Productores Activos: %i \n", variables[0].producers);
    printf("Consumidores Activos: %i \n", variables[0].consumers);
    printf("-- Escrito en el espacio de memoria %i --\n", i);
    printf("Contenido del mensaje escrito: \n");
    printf("Uso de la memoria: %i \n", buffer[i].active);
    printf("Pid del productor creador: %i \n", buffer[i].pid);
    printf("Valor de Magic Number asociado: %i \n", buffer[i].magic_number);
    printf("Fecha y hora de creacion: %s \n", buffer[i].date);
    printf("Texto del mensaje: %s \n", buffer[i].text);
    printf("\n");

}

void endProducer(float tiempo, int msg, float bloq, float wait){
    variables[0].producers -= 1;
    printf("\n");
    printf("Solicitud del Finalizador Recibida. \n");
    printf("Productor Finalizado. \n");
    printf("\n");
    printf("Resumen de actividad: \n");
    printf("Pid del Productor: %i \n", getpid());
    printf("Mensajes Producidos por este Productor: %i \n", msg);
    printf("Mensajes Totales Producidos: %i \n", variables[0].produced);
    printf("Tiempo en espera: %f \n", wait);
    printf("Tiempo en bloqueo: %f \n",bloq);
    printf("Tiempo total activo: %f \n",tiempo);
    exit(0);
}