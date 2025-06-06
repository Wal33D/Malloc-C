CC ?= gcc
CFLAGS ?= -std=c11 -Wall -Wextra -pedantic

all: example

example: example.o nu_malloc.o
	$(CC) $(CFLAGS) -o $@ example.o nu_malloc.o

example.o: example.c nu_malloc.h
	$(CC) $(CFLAGS) -c example.c

nu_malloc.o: nu_malloc.c nu_malloc.h
	$(CC) $(CFLAGS) -c nu_malloc.c

clean:
	rm -f example *.o

test: example
	@echo "Running example..." && \
	output=`./example` && \
	if [ "$$output" = "Value: 42" ]; then \
		echo "$$output"; \
	else \
		echo "Unexpected output: $$output"; exit 1; \
	fi

.PHONY: all clean test
