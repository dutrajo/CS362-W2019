CFLAGS= -Wall  

default: runtest 

testme: testme.o 
	gcc -o testme -g testme.o $(CFLAGS)

testme.o: testme.c
	gcc -c testme.c -g  $(CFLAGS)

runtest: testme
	./testme
	
clean:
	rm -f *.o testme 
