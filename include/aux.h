 
#ifndef _UTILITIES_H
#define _UTILITIES_H

void create_directory(char *buffer_name);
int split(char *str, char c, char ***arr);
int check_dir(char *path);
char *concat(const char *s1, const char *s2);
int check_bin_dir(void);
void create_dirs(char *buffer_name);

 
#endif