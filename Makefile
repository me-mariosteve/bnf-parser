CC = gcc
override CFLAGS += -Wall -Wpedantic -Wextra -Wno-unused

LIBS = BNF.o utils.o

all: $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -o $(@) -c $(^)

