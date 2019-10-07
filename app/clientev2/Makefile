CC = gcc
CFLAGS = -g3 -Wall
MKDIR = mkdir -p

LIBS=-lm

directorios: 
	$(MKDIR) build dist

utils.o: directorios utils.c utils.h
	$(CC) $(CFLAGS) -c utils.c -o build/utils.o

main.o: directorios main.c
	$(CC) $(CFLAGS) -c main.c -o build/main.o

all: clean main.o utils.o
	$(CC) $(CFLAGS) $(LIBS) -o dist/cliente build/main.o build/utils.o
	rm -fr build

clean:
	rm -fr *.o a.out cliente dist build core

.DEFAULT_GOAL := all