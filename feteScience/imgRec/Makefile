all: recContours.o mod1.o mod1 f.o f

recContours.o : recContours.cpp variablesExt.h recContours.h
	g++ -I . -o recContours.o -c recContours.cpp 
	
pathfinding.o : pathfinding.cpp variablesExt.h pathfinding.h
	g++ -I . -o pathfinding.o -c pathfinding.cpp 
	
mod1.o : mod1.cpp variables.h recContours.h
	g++ -I . `pkg-config --cflags opencv` -o mod1.o -c mod1.cpp `pkg-config --libs opencv`
	
mod1 : recContours.o pathfinding.o mod1.o
	g++ -I . `pkg-config --cflags opencv` -o mod1.exe mod1.o recContours.o pathfinding.o `pkg-config --libs opencv`

f.o : mod2.cpp variables.h recContours.h
	g++ -I . `pkg-config --cflags opencv` -o f.o -c mod2.cpp `pkg-config --libs opencv`
	
f : recContours.o pathfinding.o f.o
	g++ -I . `pkg-config --cflags opencv` -o f.exe f.o recContours.o pathfinding.o `pkg-config --libs opencv`

clean:
	rm -rf *.o
