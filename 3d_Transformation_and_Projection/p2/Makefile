main.out : main.o shape.o
	g++ -std=c++11 -Wall -g -o main main.o shape.o -lglut -lGL

main.o : main.cpp
	g++ -std=c++11 -Wall -g -c main.cpp

shape.o : shape.cpp shape.h
	g++ -Wall -g -c shape.cpp

clean :
	rm -f main main.o shape.o 