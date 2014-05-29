CC = gcc
CFLAGS = -Wall -lm
SRC = MemoryAllocator.c 
EXE = allocator

build:
	$(CC) -o $(EXE) $(SRC) $(CFLAGS)

clean :
	rm -f $(EXE) *~
