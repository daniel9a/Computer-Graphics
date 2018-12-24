main : main.o Raytrace.o
	g++ -std=c++11 -Wall -g -o main main.o Raytrace.o -lglut -lGL

main.o : main.cpp
	g++ -std=c++11 -Wall -g -c main.cpp

Raytrace.o : Raytrace.cpp Raytrace.h
	g++ -Wall -g -c Raytrace.cpp

clean :
	rm -f main main.o Raytrace.o
