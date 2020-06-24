 
#ifndef _SEMAPHORE_H
#define _SEMAPHORE_H


int crearSem(char *buffer, int identifier, int sem_num, int init_val);
void borrarSem(int id);
int abrirSem(char *buffer, int identifier, int sem_num);
void verdeSem(int id, int num);
void rojoSem(int id, int num);
int sem_get_value(int id, int num);

 
#endif