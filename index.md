# Simple HTTP Server

Repository build status: ![](https://github.com/AppLoidx/c-http-server/workflows/make-master/badge.svg)

Rus documentation available [here](index.ru.html)

### Build

```bash
make all
```

### Run

```bash
./server
```

### Static files

You can add static files to the `static` directory. 
They will be automatically processed and mapped to url paths

## Development

### Controller mappings

There is a simple controller mapping:
```c
  POST_MAPPING("/") {
    puts("HTTP/1.1 200 OK\r\n\r\n");
    puts("Post request received\r\n");
    puts("Fetch the data using `payload` variable.");
  }
```

```c
  POST_MAPPING("/anime") {
    puts("HTTP/1.1 200 OK\r\n\r\n");
    puts("Wanna see anime?");
  }
```

Feel free to use some util functions:
```c
  POST_MAPPING("/anime") {
    response_ok();
    puts("Wanna see anime?");
  }
```

### Server static files and another media

To add static files put them on the static folder. Authomaticly all files will be served as `text/html` files except of `css`

If you want to map some controller don't forget to use HTTP header for response. There is some methods to do this:
```c
response_ok();
```
or
```c
response_media_css();
```


