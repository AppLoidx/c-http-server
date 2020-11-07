GCC=gcc
COMPILE_FLAGS=-Wall -Werror -pedantic


all: server

clean:
	@rm -rf *.o
	@rm -rf server

clean-temp:
	@rm -rf *.o

server: main.o httplib.o static.o
	$(GCC) $(COMPILE_FLAGS) -o server $^
	make clean-temp

main.o: main.c httplib.h
	$(GCC) $(COMPILE_FLAGS) -c -o main.o main.c

httplib.o: httplib.c httplib.h
	$(GCC) $(COMPILE_FLAGS) -c -o httplib.o httplib.c

static.o: static.c static.h
	$(GCC) $(COMPILE_FLAGS) -c -o static.o static.c

