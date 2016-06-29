CCFLAGS = -O3 -DNDEBUG --std=c11 
LIBS = -lz -lm

karp-rabin : karp-rabin.o 
	gcc $(CCFLAGS) karp-rabin.o $(LIBS) -o karp-rabin

karp-rabin.o: main.c
	gcc -c $(CCFLAGS) main.c -o karp-rabin.o $(LIBS)
