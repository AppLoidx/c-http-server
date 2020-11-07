#include <dirent.h>
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#ifndef _STATIC_H___
#define _STATIC_H___

void writeFile(char *filename);
void static_files(char *files[]);
void list_dir(const char *dir_name, char *files[], int *index);

#endif