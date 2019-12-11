CC= gcc 
TARGET= main
INCLUDE_DIR= ../../include
CPPFLAGS= -Wall -I$(INCLUDE_DIR)

default: main

main: main.c bigint.o
	$(CC) $(CPPFLAGS) -o $@ $^
bigint.o: ../../src/bigint.c $(INCLUDE_DIR)/bigint.h
	$(CC) $(CPPFLAGS) -o $@ -c $<
