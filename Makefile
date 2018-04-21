# Base folder path, can be changed to absolute if needed
ROOT = .

# Source code
SRC_DIR = $(ROOT)/src
INCL_DIR = $(ROOT)/include

# Libraries and dependencies
INCLS = -I $(INCL_DIR)

# Testing
TEST_DIR = $(ROOT)/test
BUILD_DIR = $(ROOT)/build

# Executables
TARGET = $(BUILD_DIR)/simple_usage
all: $(TARGET)

# Compilation options
CC = gcc
CFLAGS_WARN = -Wall
CFLAGS_DEBUG = -g
CFLAGS = $(CFLAGS_WARN) $(CFLAGS_DEBUG) $(INCLS)
HEADERS = $(wildcard **/*.h)
OBJECTS = $(patsubst %.c,%.o,$(wildcard **/*.c))

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) $(CFLAGS_WARN) $(LIBS) -o $@

clean:
	-rm -f **/*.o $(TARGET)

.PHONY: clean all
