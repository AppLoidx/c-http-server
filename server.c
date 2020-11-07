/*
 * MIT License
 * Copyright (C) 2020 Arthur Kupriyanov
 */

#include "httplib.h"
#include "static.h"

int main(int c, char **v) {
  serve_forever("12913");
  return 0;
}

void route() {
  ROUTE_START()

  POST_MAPPING("/") {
    printf("HTTP/1.1 200 OK\r\n\r\n");
    printf("Wow, seems that you POSTed %d bytes. \r\n", payload_size);
    printf("Fetch the data using `payload` variable.");
  }

  ROUTE_END()
}
