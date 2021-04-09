objs = semaphores.o shared_memory.o
md5lib = -lssl -lcrypto

all: $(objs) P1.o P2.o
	gcc $(objs) P1.o -o P1 $(md5lib)
	gcc $(objs) P2.o -o P2 $(md5lib)

P1.o: P1.c
	gcc -c P1.c $(md5lib)

P2.o: P2.c
	gcc -c P2.c $(md5lib)

semaphores.o: semaphores.c
	gcc -c semaphores.c

shared_memory.o: shared_memory.c
	gcc -c shared_memory.c

clean:
	rm -f $(objs) P1 P2 P1.o P2.o