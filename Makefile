# Export all variables for the rest of the build system to use
export

# Commands and options
MKD = mkdir -p
RMD = rm -rf
CC = gcc
CFLAGS = -Wall -g

# Compilation
OUT = bin
SA_SRC = $(wildcard *.c)
SA_OBJ = $(SA_SRC:.c=.o)
SA_DIR = $(shell dirname $(realpath $(lastword $(MAKEFILE_LIST))))

.PHONY: all example test clean

all: $(SA_OBJ) example test

$(SA_OBJ): $(SA_SRC)
	$(MKD) $(OUT)
	$(CC) -c $(CFLAGS) -I $(SA_DIR) $< -o $(OUT)/$@

example:
	$(MAKE) -C example

test:
	$(MAKE) -C test

clean:
	$(MAKE) -C example clean
	$(MAKE) -C test clean
	$(RMD) $(OUT) %.o
