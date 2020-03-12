quash: main.o

	g++ -g -std=c++11 -Wall main.o -o quash


main.o: main.cpp

	g++ -g -std=c++11 -Wall -c main.cpp


clean:
	rm *.o quash
