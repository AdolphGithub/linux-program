all: shm1 shm2

CC=gcc

shm1.o: shm1.c shm_com.h
	$(CC) -c shm1.c shm_com.h

shm2.o: shm2.c shm_com.h
	$(CC) -c shm2.c shm_com.h

shm1: shm1.o
	$(CC) -o shm1 shm1.o

shm2: shm2.o
	$(CC) -o shm2 shm2.o

clean:
	rm -r shm1.o shm2.o shm_com.h.gch