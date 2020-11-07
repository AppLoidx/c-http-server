
#include "static.h"

/* List the files in "dir_name". */

int main() {
  char *files[100] = {NULL};
  int c = 0;
  list_dir("static", files, &c);

  int index = 0;
  while (files[index] != NULL) {
    printf("File[%d]: %s\n", index, files[index]);
    index++;
  }

  return 0;
}