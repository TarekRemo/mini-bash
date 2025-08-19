CC = gcc
CFLAGS = -Wall -Wextra
TARGET = bin/mini-bash

all: $(TARGET)

$(TARGET): main.o utils.o parser.o builtin.o bin
	$(CC) main.o utils.o parser.o builtin.o -o bin/mini-bash

bin: 
	mkdir bin

main.o: main.c 
	$(CC) $(CFLAGS) main.c -c main.o

utils.o: src/utils.c
	$(CC) $(CFLAGS) src/utils.c -c utils.o

parser.o: src/parser.c
	$(CC) $(CFLAGS) src/parser.c -c parser.o

builtin.o: src/builtin.c
	$(CC) $(CFLAGS) src/builtin.c -c builtin.o