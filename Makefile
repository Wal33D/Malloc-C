# Build settings
CC ?= gcc
CFLAGS ?= -std=c11 -Wall -Wextra -pedantic -fPIC -Iinclude
AR ?= ar
PREFIX ?= /usr/local
INCLUDEDIR ?= $(PREFIX)/include
LIBDIR ?= $(PREFIX)/lib
BUILD_DIR ?= build
SRC_DIR := src
TEST_DIR := tests

LIB_STATIC := $(BUILD_DIR)/libnu_malloc.a
LIB_SHARED := $(BUILD_DIR)/libnu_malloc.so
EXAMPLE := $(BUILD_DIR)/example
MEM_TEST := $(BUILD_DIR)/memory_test

OBJS := $(BUILD_DIR)/nu_malloc.o
EXAMPLE_OBJ := $(BUILD_DIR)/example.o
MEM_TEST_OBJ := $(BUILD_DIR)/memory_test.o

all: $(LIB_STATIC) $(LIB_SHARED) $(EXAMPLE) $(MEM_TEST)

$(BUILD_DIR):
	mkdir -p $@

$(EXAMPLE): $(EXAMPLE_OBJ) $(OBJS) | $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $@ $(EXAMPLE_OBJ) $(OBJS)

$(MEM_TEST): $(MEM_TEST_OBJ) $(OBJS) | $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $@ $(MEM_TEST_OBJ) $(OBJS)

$(EXAMPLE_OBJ): $(SRC_DIR)/example.c include/nu_malloc.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(MEM_TEST_OBJ): $(TEST_DIR)/memory_test.c include/nu_malloc.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/nu_malloc.o: $(SRC_DIR)/nu_malloc.c include/nu_malloc.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(LIB_STATIC): $(BUILD_DIR)/nu_malloc.o | $(BUILD_DIR)
	$(AR) rcs $@ $(BUILD_DIR)/nu_malloc.o

$(LIB_SHARED): $(BUILD_DIR)/nu_malloc.o | $(BUILD_DIR)
	$(CC) $(CFLAGS) -shared -o $@ $(BUILD_DIR)/nu_malloc.o

clean:
	rm -rf $(BUILD_DIR)

test: $(EXAMPLE) $(MEM_TEST)
	@$(TEST_DIR)/test_example.sh ./$(EXAMPLE)
	@$(TEST_DIR)/test_memory.sh ./$(MEM_TEST)

install: $(LIB_STATIC) $(LIB_SHARED)
	install -d $(DESTDIR)$(INCLUDEDIR)
	install -m 644 include/nu_malloc.h $(DESTDIR)$(INCLUDEDIR)/
	install -d $(DESTDIR)$(LIBDIR)
	install -m 644 $(LIB_STATIC) $(DESTDIR)$(LIBDIR)/
	install -m 755 $(LIB_SHARED) $(DESTDIR)$(LIBDIR)/

uninstall:
	rm -f $(DESTDIR)$(INCLUDEDIR)/nu_malloc.h
	rm -f $(DESTDIR)$(LIBDIR)/libnu_malloc.a
	rm -f $(DESTDIR)$(LIBDIR)/libnu_malloc.so

.PHONY: all clean test install uninstall
