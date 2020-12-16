
# MIT License
# Copyright (C) 2020 Arthur Kupriyanov


GCC=gcc
COMPILE_FLAGS=-Wall -Werror -pedantic -pthread

all: server

clean:
	@rm -rf *.o
	@rm -rf server

clean-temp:
	@rm -rf *.o

server: server.o httplib.o static.o
	$(GCC) $(COMPILE_FLAGS) -o server $^
	make clean-temp

server.o: server.c httplib.h
	$(GCC) $(COMPILE_FLAGS) -c -o server.o server.c

httplib.o: httplib.c httplib.h
	$(GCC) $(COMPILE_FLAGS) -c -o httplib.o httplib.c

static.o: static.c static.h
	$(GCC) $(COMPILE_FLAGS) -c -o static.o static.c

