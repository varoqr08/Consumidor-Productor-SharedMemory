#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <linux/limits.h>


/* Revisa si existe una carpeta, si no la crea. */
int check_dir(char *path){
  struct stat st = {0};
  if (stat(path, &st) == -1)
  {
    mkdir(path, 0700);
    printf("Se ha creado la carpeta: %s\n", path);
    return 0;
  }else{
    printf("La carpeta ya existe :%s\n", path);
    return 1;
  }
}

/*Revisa si un string es un numero.*/
int isNumber(char *text){
  int j;
  j = strlen(text);
  while(j--){
    if(text[j] > 47 && text[j] < 58)
      continue;
    return 0;
  }
  return 1;
}


/* Funcion que concatena dos Strings */
char *concat(const char *s1, const char *s2){
  char *result = (char *)malloc(strlen(s1) + strlen(s2) + 1);
  strcpy(result, s1);
  strcat(result, s2);
  return result;
}
