CC=gcc

all: parallel_min_max process_memory parallel_sum

parallel_min_max : utils.o find_min_max.o utils.h find_min_max.h
	$(CC) -o parallel_min_max utils.o find_min_max.o parallel_min_max.c -I.

process_memory :
	$(CC) -o process_memory process_memory.c -Wall -Wno-format

parallel_sum : utils.o utils.h
	$(CC) -o parallel_sum utils.o parallel_sum.c -lpthread -I.

clean :
	rm utils.o find_min_max.o parallel_min_max process_memory parallel_sum

