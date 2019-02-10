CFLAGS= -Wall -ftest-coverage -fprofile-arcs -O0 -lm 

default: unittest1 unittest2 unittest3 unittest4 cardtest1 cardtest2 cardtest3 cardtest4  

unittest1: rngs.o dominion.o unittest1.o
	gcc -o unittest1 -g dominion.o rngs.o unittest1.o $(CFLAGS)

unittest2: rngs.o dominion.o unittest2.o
	gcc -o unittest2 -g dominion.o rngs.o unittest2.o $(CFLAGS)

unittest3: rngs.o dominion.o unittest3.o
	gcc -o unittest3 -g dominion.o rngs.o unittest3.o $(CFLAGS)

unittest4: rngs.o dominion.o unittest4.o
	gcc -o unittest4 -g dominion.o rngs.o unittest4.o $(CFLAGS)

cardtest1: rngs.o dominion.o cardtest1.o
	gcc -o cardtest1 -g dominion.o rngs.o cardtest1.o $(CFLAGS)

cardtest2: rngs.o dominion.o cardtest2.o
	gcc -o cardtest2 -g dominion.o rngs.o cardtest2.o $(CFLAGS)

cardtest3: rngs.o dominion.o cardtest3.o
	gcc -o cardtest3 -g dominion.o rngs.o cardtest3.o $(CFLAGS)

cardtest4: rngs.o dominion.o cardtest4.o
	gcc -o cardtest4 -g dominion.o rngs.o cardtest4.o $(CFLAGS)
	
rngs.o: rngs.h rngs.c
	gcc -c rngs.c -g  $(CFLAGS)

dominion.o: dominion.h dominion.c rngs.o
	gcc -c dominion.c -g  $(CFLAGS)

unittest1.o:
	gcc -c unittest1.c -g $(CFLAGS)

unittest2.o:
	gcc -c unittest2.c -g $(CFLAGS)

unittest3.o:
	gcc -c unittest3.c -g $(CFLAGS)

unittest4.o:
	gcc -c unittest4.c -g $(CFLAGS)

cardtest1.o: rngs.o dominion.o cardtest1.c
	gcc -c cardtest1.c -g $(CFLAGS)

cardtest2.o: rngs.o dominion.o cardtest2.c
	gcc -c cardtest2.c -g $(CFLAGS)

cardtest3.o: rngs.o dominion.o cardtest3.c
	gcc -c cardtest3.c -g $(CFLAGS)	

cardtest4.o: rngs.o dominion.o cardtest4.c
	gcc -c cardtest4.c -g $(CFLAGS)

unittestresults: default
	echo "Running unittest1...." &> unittestresults.out
	./unittest1
	cat unittest1.out >> unittestresults.out
	gcov -b unittest1.c >> unittestresults.out
	echo " " >> unittestresults.out
	echo "Running unittest2..." >> unittestresults.out
	./unittest2
	cat unittest2.out >> unittestresults.out
	gcov -b unittest2.c >> unittestresults.out
	echo " " >> unittestresults.out
	echo "Running unittest3..." >> unittestresults.out
	./unittest3
	cat unittest3.out >> unittestresults.out
	gcov -b unittest3.c >> unittestresults.out
	echo " " >> unittestresults.out
	echo "Running unittest4..." >> unittestresults.out
	./unittest4
	cat unittest4.out >> unittestresults.out
	gcov -b unittest4.c >> unittestresults.out
	echo " " >> unittestresults.out
	echo "Running cardtest1..." >> unittestresults.out
	./cardtest1
	cat cardtest1.out >> unittestresults.out
	gcov -b cardtest1.c >> unittestresults.out
	echo " " >> unittestresults.out
	echo "Running cardtest2..." >> unittestresults.out
	./cardtest2
	cat cardtest2.out >> unittestresults.out
	gcov -b cardtest2.c >> unittestresults.out
	echo " " >> unittestresults.out
	echo "Running cardtest3..." >> unittestresults.out
	./cardtest3
	cat cardtest3.out >> unittestresults.out
	gcov -b cardtest3.c >> unittestresults.out
	echo " " >> unittestresults.out
	echo "Running cardtest4..." >> unittestresults.out
	./cardtest4
	cat cardtest4.out >> unittestresults.out
	gcov -b cardtest4.c >> unittestresults.out
	echo "Finished running tests..."

	
clean:
	rm -f *.o *.gcov *.gcda *.gcno unittest1 unittest2 unittest3 unittest4 cardtest1 cardtest2 cardtest3 cardtest4
