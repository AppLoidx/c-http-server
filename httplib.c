/*
 * MIT License
 * Copyright (C) 2020 Arthur Kupriyanov
 * 
 * HTTP Server inspired by : 
 * 
 * laobubu, Wuhan, P.R.China 
 * https://gist.github.com/laobubu/d6d0e9beb934b60b2e552c2d03e1409e
 * 
 * Abhijeet Rastogi, San Francisco Bay Area
 * http://blog.abhijeetr.com/2010/04/very-simple-http-server-writen-in-c.html
 * 
 */

#include "httplib.h"
#include "static.h"

#include <arpa/inet.h>
#include <fcntl.h>
#include <netdb.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define CONNMAX 1000
#define STATIC_MAX 100

static int listenfd, clients[CONNMAX];
static void startServer(const char *);
static void respond(int);

typedef struct {
  char *name, *value;
} header_t;
static header_t reqhdr[17] = {{"\0", "\0"}};
static int clientfd;

static char *buf;

char *files[STATIC_MAX];

void serve_forever(const char *PORT) {
  struct sockaddr_in clientaddr;
  socklen_t addrlen;

  int slot = 0;

  printf("Server started %shttp://127.0.0.1:%s%s\n", "\033[92m", PORT,
         "\033[0m");

  int i;
  for (i = 0; i < CONNMAX; i++)
    clients[i] = -1; // there is no client connected
  startServer(PORT);

  // Ignore SIGCHLD to avoid zombie threads
  signal(SIGCHLD, SIG_IGN);

  // ACCEPT connections
  while (1) {
    addrlen = sizeof(clientaddr);
    clients[slot] = accept(listenfd, (struct sockaddr *)&clientaddr, &addrlen);

    if (clients[slot] < 0) {
      perror("trying to accept requests");
    } else {
      if (fork() == 0) // executing on the another process
      {
        respond(slot);
        exit(0);
      }
    }

    while (clients[slot] != -1)
      slot = (slot + 1) % CONNMAX; // move client pointer
  }
}

// start server
void startServer(const char *port) {

  static_files(files);

  struct addrinfo hints, *res, *p;

  // getaddrinfo for host
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;
  if (getaddrinfo(NULL, port, &hints, &res) != 0) {
    perror("getaddrinfo() error");
    exit(1);
  }
  // socket and bind
  for (p = res; p != NULL; p = p->ai_next) {
    int option = 1;
    listenfd = socket(p->ai_family, p->ai_socktype, 0);
    setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));
    if (listenfd == -1)
      continue;
    if (bind(listenfd, p->ai_addr, p->ai_addrlen) == 0)
      break;
  }
  if (p == NULL) {
    perror("socket() or bind()");
    exit(1);
  }

  freeaddrinfo(res);

  // listen for incoming connections
  if (listen(listenfd, 1000000) != 0) {
    perror("listen() error");
    exit(1);
  }
}

// get request header
char *request_header(const char *name) {
  header_t *h = reqhdr;
  while (h->name) {
    if (strcmp(h->name, name) == 0)
      return h->value;
    h++;
  }
  return NULL;
}

void respond(int n) {
  int rcvd;

  buf = malloc(65535);
  rcvd = recv(clients[n], buf, 65535, 0);

  if (rcvd < 0) {

    // receive error
    fprintf(stderr, ("recv() error\n"));

  } else if (rcvd == 0) {

    fprintf(stderr, "Client disconnected upexpectedly.\n");

  } else {
    // message received

    buf[rcvd] = '\0';

    method = strtok(buf, " \t\r\n");
    uri = strtok(NULL, " \t");
    prot = strtok(NULL, " \t\r\n");

    fprintf(stderr, "\x1b[32m + [%s] %s\x1b[0m\n", method, uri);

    if ((qs = strchr(uri, '?')) != NULL) {
      *qs++ = '\0'; // split URI
    } else {
      qs = uri - 1; // use an empty string
    }

    header_t *h = reqhdr;
    char *t = (char *)malloc(sizeof(char));
    char *t2 = (char *)malloc(sizeof(char));

    while (h < reqhdr + 16) {
      char *k, *v, *t;
      k = strtok(NULL, "\r\n: \t");
      if (!k)
        break;
      v = strtok(NULL, "\r\n");
      while (*v && *v == ' ')
        v++;
      h->name = k;
      h->value = v;
      h++;
      fprintf(stderr, "[H] %s: %s\n", k, v);
      t = v + 1 + strlen(v);
      if (t[1] == '\r' && t[2] == '\n')
        break;
    }

    t++; // now the *t shall be the beginning of user payload
    t2 = request_header("Content-Length"); // and the related header if there is
    payload = t;
    payload_size = t2 ? atol(t2) : (rcvd - (t - buf));

    // bind clientfd to stdout, making it easier to write
    // easy, but not recommended, i think so :)
    clientfd = clients[n];
    dup2(clientfd, STDOUT_FILENO);
    close(clientfd);

    // call router
    route();

    // tidy up
    fflush(stdout);
    shutdown(STDOUT_FILENO, SHUT_WR);
    close(STDOUT_FILENO);
  }

  // Closing SOCKET
  shutdown(
      clientfd,
      SHUT_RDWR); // All further send and recieve operations are DISABLED...
  close(clientfd);
  clients[n] = -1;
}

/*
 * Write HTTP/1.1 OK (200) status to client socket
 * with Content-Type: text/html
 */
void response_ok() {
  printf("HTTP/1.1 200 OK\n");
  printf("Content-Type: text/html\r\n\r\n");
}

/*
 * Write HTTP/1.1 OK (200) status to client socket
 * with Content-Type: text/css
 */
void response_css_media() {
  printf("HTTP/1.1 200 OK\n");
  printf("Content-Type: text/css\r\n\r\n");
}

int ends_with(const char *str, const char *suffix) {
  if (!str || !suffix)
    return 0;
  size_t lenstr = strlen(str);
  size_t lensuffix = strlen(suffix);
  if (lensuffix > lenstr)
    return 0;
  return strncmp(str + lenstr - lensuffix, suffix, lensuffix) == 0;
}

int serve_static(char *uri) {

  char web_path[MAX_FILE_NAME_LENGTH];
  int index = 0;
  char *static_route_path = strcmp("/", uri) == 0 ? "index.html" : ++uri;
  sprintf(web_path, "static/%s", static_route_path);

  while (files[index] != NULL) {

    if (strcmp(files[index], web_path) == 0) {
      if (ends_with(web_path, ".css")) {
        response_css_media();
      } else {
        response_ok();
      }
      writeFile(files[index]);
      return 0;
    }

    index++;
  }

  return 1;
}
