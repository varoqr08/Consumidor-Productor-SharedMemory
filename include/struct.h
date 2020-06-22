#ifndef _STRUCT_H
#define _STRUCT_H


//Estructura para las variables globales//
typedef struct
{
  int producers;
  int consumers;
  int size;
  int kill;
} global_variables;


//Estructura para cada mensaje a enviar//
typedef struct
{
  int pid;
  int magic_number;
  char date[50];
  char hour[50];
} message;

#endif