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

#ifndef _HTTPD_LIB___
#define _HTTPD_LIB___

#ifndef MAX_FILE_NAME_LENGTH
#define MAX_FILE_NAME_LENGTH 100
#endif

#include <stdio.h>
#include <string.h>

void serve_forever(const char *PORT);

char *method, *uri,
    *qs,   // url params
    *prot; // protocol (usually HTTP/1.1)

char *payload; // usually used for POST (see REST rules for more information)
int payload_size;

char *request_header(const char *name);

/*
 * Server will call this function on each client request
 * here it have to handle request using uri, method and other
 * params stated below.
 *
 * If you worked with redux you can think about this
 * method like a "root reducer" in your store
 * 
 * Read more : https://apploidx.github.io/c-http-server/route.html
 */
void route();

/*
 * This method will be called when nothing is mapped in
 * this uri.
 *
 * He will use the static folder (read more about this here: https://apploidx.github.io/c-http-server/)
 *
 * In short, it will write to client socket static files located in uri path
 */
int serve_static(char *uri);

/*
 * Write success status to a clent socket
 */
void response_ok();

/*
 * Write css media response to a client socket
 */
void response_css_media();

// some interesting macro for `route()`
#define ROUTE_START() if (0) {
#define REQUEST_MAPPING(METHOD, URI)                                           \
  }                                                                            \
  else if (strcmp(URI, uri) == 0 && strcmp(METHOD, method) == 0) {
#define GET_MAPPING(URI) REQUEST_MAPPING("GET", URI)
#define POST_MAPPING(URI) REQUEST_MAPPING("POST", URI)
#define ROUTE_END()                                                            \
  }                                                                            \
  else if (serve_static(uri) != 0) {                                           \
    puts("HTTP/1.1 404 Not Found\r\n\r\n"                                      \
         "Page is not found.\r\n");                                            \
  }

#endif
