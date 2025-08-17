CC = gcc
CFLAGS = -Wall -Wextra
TARGET = bin/mini-bash

all: $(TARGET)

$(TARGET): main.o utils.o bin
	$(CC) main.o utils.o -o bin/mini-bash

bin: 
	mkdir bin

main.o: main.c 
	$(CC) $(CFLAGS) main.c -c main.o

utils.o: src/utils.c
	$(CC) $(CFLAGS) src/utils.c -c utils.o
