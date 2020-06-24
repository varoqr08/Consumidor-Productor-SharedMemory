#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include <string.h>

/* Funcion para verificar la existencia de  una carpeta*/
int check_dir(const char* path){
  struct stat sb;
  if (stat(path, &sb) == 0 && S_ISDIR(sb.st_mode)) {
    return 0;
  }else {
    return 1;
  }   
}

/*Funcion para crear una carpeta */
int make_dir(const char* path){
  if(mkdir(path, 0700) == -1){
    return 1;
  }
  return 0;
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

