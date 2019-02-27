CFLAGS= -Wall -ftest-coverage -fprofile-arcs -fno-exceptions -O0 -lm 

default: randomtestcard1 randomtestcard2 randomtestadventurer runtests 


randomtestcard1: rngs.o dominion.o randomtestcard1.o
	gcc -o randomtestcard1 -g dominion.o rngs.o randomtestcard1.o $(CFLAGS)

randomtestcard2: rngs.o dominion.o randomtestcard2.o
	gcc -o randomtestcard2 -g dominion.o rngs.o randomtestcard2.o $(CFLAGS)

randomtestadventurer: rngs.o dominion.o randomtestadventurer.o
	gcc -o randomtestadventurer -g dominion.o rngs.o randomtestadventurer.o $(CFLAGS)
	
rngs.o: rngs.h rngs.c
	gcc -c rngs.c -g  $(CFLAGS)

dominion.o: dominion.h dominion.c rngs.o
	gcc -c dominion.c -g  $(CFLAGS)

randomtestcard1.o: rngs.o dominion.o randomtestcard1.c
	gcc -c randomtestcard1.c -g $(CFLAGS)

randomtestcard2.o: rngs.o dominion.o randomtestcard2.c
	gcc -c randomtestcard2.c -g $(CFLAGS)

randomtestadventurer.o: rngs.o dominion.o randomtestadventurer.c
	gcc -c randomtestadventurer.c -g $(CFLAGS)

runtests: 
	./randomtestcard1 > /dev/null
	gcov -b dominion.c >> randomtestcard1.out 
	./randomtestcard2 > /dev/null
	gcov -b dominion.c >> randomtestcard2.out 
	./randomtestadventurer > /dev/null
	gcov -b dominion.c >> randomtestadventurer.out 
	
clean:
	rm -f *.o *.gcov *.gcda *.gcno randomtestcard1 randomtestcard2 randomtestadventurer 
