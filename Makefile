CC = gcc
CFLAGS = -Wall -lm
SRC = tema4.c 
EXE = allocator

build:
	$(CC) -o $(EXE) $(SRC) $(CFLAGS)

clean :
	rm -f $(EXE) *~
