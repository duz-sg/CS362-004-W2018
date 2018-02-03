CFLAGS = -Wall -fpic -coverage -lm -std=c99

default: testme 

testme: testme.c 
	echo "Result for running testme tests:" > testme.txt 
	
	echo "testme.c:" >> testme.txt
	gcc -o testme testme.c $(CFLAGS)
	./testme >> testme.txt
	gcov testme.c >> testme.txt

clean:
	rm testme.txt testme

cleanall: 
	rm testme.txt testme *.gcda *.gcno *.gcov
