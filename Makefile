CC ?= gcc
CFLAGS ?= -std=c11 -Wall -Wextra -pedantic
AR ?= ar

all: libnu_malloc.a example memory_test

example: example.o nu_malloc.o
	$(CC) $(CFLAGS) -o $@ example.o nu_malloc.o

memory_test: memory_test.o nu_malloc.o
	$(CC) $(CFLAGS) -o $@ memory_test.o nu_malloc.o

example.o: example.c nu_malloc.h
	$(CC) $(CFLAGS) -c example.c

memory_test.o: memory_test.c nu_malloc.h
	$(CC) $(CFLAGS) -c memory_test.c

nu_malloc.o: nu_malloc.c nu_malloc.h
	$(CC) $(CFLAGS) -c nu_malloc.c

libnu_malloc.a: nu_malloc.o
	$(AR) rcs $@ nu_malloc.o
clean:
	rm -f example memory_test libnu_malloc.a *.o

test: example memory_test
	@./test_example.sh
	@./test_memory.sh

.PHONY: all clean test
