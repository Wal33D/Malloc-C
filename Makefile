CC ?= gcc
CFLAGS ?= -std=c11 -Wall -Wextra -pedantic -fPIC
AR ?= ar
PREFIX ?= /usr/local
INCLUDEDIR ?= $(PREFIX)/include
LIBDIR ?= $(PREFIX)/lib

all: libnu_malloc.a libnu_malloc.so example memory_test

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

libnu_malloc.so: nu_malloc.o
	$(CC) $(CFLAGS) -shared -o $@ nu_malloc.o
clean:
	rm -f example memory_test libnu_malloc.a libnu_malloc.so *.o

test: example memory_test
	@./test_example.sh
	@./test_memory.sh

install: libnu_malloc.a libnu_malloc.so
	install -d $(DESTDIR)$(INCLUDEDIR)
	install -m 644 nu_malloc.h $(DESTDIR)$(INCLUDEDIR)/
	install -d $(DESTDIR)$(LIBDIR)
	install -m 644 libnu_malloc.a $(DESTDIR)$(LIBDIR)/
	install -m 755 libnu_malloc.so $(DESTDIR)$(LIBDIR)/

.PHONY: all clean test install
