CC=gcc

all: parallel_min_max process_memory parallel_sum

utils.o : utils.h
	$(CC) -o utils.o -c utils.c $(CFLAGS)

sum.o: sum.h
	$(CC) -o sum.o -c sum.c

parallel_min_max : utils.o find_min_max.o utils.h find_min_max.h
	$(CC) -o parallel_min_max utils.o find_min_max.o parallel_min_max.c -I.

process_memory :
	$(CC) -o process_memory process_memory.c -Wall -Wno-format

parallel_sum : utils.o utils.h sum.o sum.h
	$(CC) -o parallel_sum sum.o utils.o parallel_sum.c -lpthread -I.

clean :
	rm utils.o sum.o find_min_max.o parallel_min_max process_memory parallel_sum
