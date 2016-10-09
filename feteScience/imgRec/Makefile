all: recContours.o mod1.o mod1

recContours.o : recContours.cpp variablesExt.h recContours.h
	g++ -o recContours.o -c recContours.cpp 
mod1.o : mod1.cpp variables.h recContours.h
	g++ `pkg-config --cflags opencv` -o mod1.o -c mod1.cpp `pkg-config --libs opencv`
mod1 : recContours.o mod1.o
	g++ `pkg-config --cflags opencv` -o mod1.exe mod1.o recContours.o `pkg-config --libs opencv`
clean:
	rm -rf *.o
