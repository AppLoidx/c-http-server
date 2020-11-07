#include <dirent.h>
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>


#include "static.h"

void writeFile(char *filename) {
  FILE *file = fopen(filename, "r");
  int c;
  if (file) {
    while ((c = getc(file)) != EOF)
      putchar(c);
    fclose(file);
  } else {
    perror("Failder to read file: ");
  }
}

void static_files(char *files[]) {
  int counter = 0;
  fprintf(stderr, "Read files\n");
  list_dir("static", files, &counter);
}

void list_dir(const char *dir_name, char *files[], int *index) {

  DIR *d;

  d = opendir(dir_name);

  /* Check it was opened. */
  if (!d) {
    fprintf(stderr, "Cannot open directory '%s': %s\n", dir_name,
            strerror(errno));
    exit(EXIT_FAILURE);
  }

  while (1) {
    struct dirent *entry;
    const char *d_name;

    entry = readdir(d);
    if (!entry) {
      break;
    }

    d_name = entry->d_name;

    if (entry->d_type & DT_DIR) {

      if (strcmp(d_name, "..") != 0 && strcmp(d_name, ".") != 0) {
        int path_length;
        char path[PATH_MAX];

        path_length = snprintf(path, PATH_MAX, "%s/%s", dir_name, d_name);

        if (path_length >= PATH_MAX) {
          fprintf(stderr, "Path length has got too long.\n");
          exit(EXIT_FAILURE);
        }

        list_dir(path, files, index);
      }
    } else {
      /*
      You can't use here char[80] path, for example, because he doesn't create
      new pointer to path char

      For example,

      path static/another.html
      Write index : 0         (pointer : 0x7fffce78d800)

      path static/css/main/main.css
      Write index : 1         (pointer : 0x7fffce78b740)

      path static/css/third/third-party.css
      Write index : 2         (pointer : 0x7fffce78b740)

      path static/index.html
      Write index : 3         (pointer : 0x7fffce78d800)

      */
      char *path = (char *)malloc(sizeof(char) *
                                  (strlen(dir_name) + strlen(d_name) + 2));
      sprintf(path, "%s/%s", dir_name, d_name);
      files[*index] = path;
      *index += 1;
    }
  }

  if (closedir(d)) {
    fprintf(stderr, "Could not close '%s': %s\n", dir_name, strerror(errno));
    exit(EXIT_FAILURE);
  }
}