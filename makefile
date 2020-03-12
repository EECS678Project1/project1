prog: main.o Quash.o

	g++ -g -std=c++11 -Wall main.o Quash.o -o prog


main.o: main.cpp Quash.h

	g++ -g -std=c++11 -Wall -c main.cpp
	

Quash.o: Quash.cpp Quash.h

	g++ -g -std=c++11 -Wall -c Quash.cpp


clean:
	rm *.o prog
