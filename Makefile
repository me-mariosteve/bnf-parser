CC = gcc
override CFLAGS += -Wall -Wpedantic -Wextra -Wno-unused -Werror

LIBS = BNF.o utils.o Array.o

all: $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -o $(@) -c $(^)

.PHONY: clean

clean:
	rm -f $(LIBS)

