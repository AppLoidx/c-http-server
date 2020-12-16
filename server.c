/*
 * MIT License
 * Copyright (C) 2020 Arthur Kupriyanov
 */

#include "httplib.h"
#include "static.h"

int main(int c, char **v)
{
  serve_forever("12913");
  return 0;
}

void route()
{
  ROUTE_START()

  POST_MAPPING("/")
  {
    response_ok();
    puts("Hey, Yo!");
  }

  ROUTE_END()
}
