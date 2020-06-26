#ifndef _STRUCT_H
#define _STRUCT_H


//Estructura para las variables globales//
typedef struct
{
  int producers;
  int consumers;
  int size;
  int id;
  int end;
  int consumed;
  int produced;
  int key_deleted;
} global_variables;


//Estructura para cada mensaje a enviar//
typedef struct
{
  int active;
  int pid;
  int magic_number;
  char date[50];
  char hour[50];
  char text[50];
} message;

#endif