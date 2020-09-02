CC = gcc

# warnings that we don't want to bother with that have been added since 1993 and seems reasonably safe to ignore...
SUPPRESS =  -Wno-format -Wno-endif-labels

CFLAGS = -g $(SUPPRESS)
LDFLAGS = /usr/local/lib/libdmalloc.a
HEADERS = ff.h opcodedef.h
SOURCES = ff.c hashing.c monitor.c wordlist.c loadcore.c parse.c sysfun.c
OBJECTS = ff.o hashing.o monitor.o wordlist.o loadcore.o parse.o sysfun.o

all: ff

$(OBJECTS): $(HEADERS) Makefile

ff: $(OBJECTS) $(SOURCES) $(HEADERS)
	$(CC) $(CFLAGS) -o ff $(LDFLAGS) $(OBJECTS) $(LDFLAGS)



