CC=gcc

#CFLAGS=-O2 -lpthread
CFLAGS=-O0 -lpthread -g # para debug

hash_s.o: hash.h hash_s.c
	$(CC) $(CFLAGS) hash_s.c -c

hash.o: hash.h hash.c
	$(CC) $(CFLAGS) hash.c -c -lm

#modificar o Makefile a partir desta linha

main: main.c hash_s.o
	$(CC) $(CFLAGS) main.c hash_s.o -o main

main_1: main_1.c hash_s.o
	$(CC) $(CFLAGS) main_1.c hash_s.o -o main_1

main_t: main_t.c hash.o
	$(CC) $(CFLAGS) main_t.c hash.o -o main_t -lm -lpthread

all: main main_1

clean:
	rm -rf *o main_t main_1 main