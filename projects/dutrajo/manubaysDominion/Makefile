CFLAGS= -Wall -ftest-coverage -fprofile-arcs -fno-exceptions -O0 -lm -std=c99 

default: MultiTestCardAdventurer MultiTestCardSmithy MultiTestCardSeaHag MultiTestCardVillage RUNTESTS 

MultiTestCardAdventurer: rngs.o dominion.o MultiTestCardAdventurer.o
	gcc -o MultiTestCardAdventurer -g dominion.o rngs.o MultiTestCardAdventurer.o $(CFLAGS)

MultiTestCardSmithy: rngs.o dominion.o MultiTestCardSmithy.o
	gcc -o MultiTestCardSmithy -g dominion.o rngs.o MultiTestCardSmithy.o $(CFLAGS)

MultiTestCardSeaHag: rngs.o dominion.o MultiTestCardSeaHag.o
	gcc -o MultiTestCardSeaHag -g dominion.o rngs.o MultiTestCardSeaHag.o $(CFLAGS)

MultiTestCardVillage: rngs.o dominion.o MultiTestCardVillage.o
	gcc -o MultiTestCardVillage -g dominion.o rngs.o MultiTestCardVillage.o $(CFLAGS)
	
MultiTestCardAdventurer.o: rngs.o dominion.o MultiTestCardAdventurer.c
	gcc -c MultiTestCardAdventurer.c -g $(CFLAGS)

MultiTestCardSmithy.o: rngs.o dominion.o MultiTestCardSmithy.c
	gcc -c MultiTestCardSmithy.c -g $(CFLAGS)

MultiTestCardSeaHag.o: rngs.o dominion.o MultiTestCardSeaHag.c
	gcc -c MultiTestCardSeaHag.c -g $(CFLAGS)

MultiTestCardVillage.o: rngs.o dominion.o MultiTestCardVillage.c
	gcc -c MultiTestCardVillage.c -g $(CFLAGS)

rngs.o: rngs.h rngs.c
	gcc -c rngs.c -g  $(CFLAGS)

dominion.o: dominion.h dominion.c rngs.o
	gcc -c dominion.c -g  $(CFLAGS)

RUNTESTS: MultiTestCardAdventurer MultiTestCardSmithy MultiTestCardVillage MultiTestCardSeaHag
	chmod +x getDominionAdventurerCoverage.sh 
	./MultiTestCardAdventurer > /dev/null
	./getDominionAdventurerCoverage.sh >> MultiTestCardAdventurer.out
	chmod +x getDominionSmithyCoverage.sh
	./MultiTestCardSmithy > /dev/null
	./getDominionSmithyCoverage.sh >> MultiTestCardSmithy.out
	chmod +x getDominionVillageCoverage.sh
	./MultiTestCardVillage > /dev/null
	./getDominionVillageCoverage.sh >> MultiTestCardVillage.out
	chmod +x getDominionSeaHagCoverage.sh
	./MultiTestCardSeaHag > /dev/null
	./getDominionSeaHagCoverage.sh >> MultiTestCardSeaHag.out

	
clean:
	rm -f *.o *.out *.gcov *.gcda *.gcno MultiTestCardAdventurer MultiTestCardSmithy MultiTestCardSeaHag MultiTestCardVillage
 
