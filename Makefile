CC = gcc
CFLAGS = -g
HEADERS = ff.h opcodedef.h
SOURCES = ff.c hashing.c monitor.c wordlist.c loadcore.c parse.c sysfun.c
OBJECTS = ff.o hashing.o monitor.o wordlist.o loadcore.o parse.o sysfun.o

all: ff

$(OBJECTS): $(HEADERS) Makefile

ff: $(OBJECTS) $(SOURCES) $(HEADERS)
	$(CC) $(CFLAGS) -o ff $(OBJECTS) 
	


