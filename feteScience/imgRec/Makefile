all: ex1 ex2 ex3 ex4 ex5 ex6
ex1 : test1.cpp
	g++ `pkg-config --cflags opencv` -o ex1 test1.cpp `pkg-config --libs opencv`
ex2 : test2.cpp
	g++ `pkg-config --cflags opencv` -o ex2 test2.cpp `pkg-config --libs opencv`
ex3 : test3.cpp
	g++ `pkg-config --cflags opencv` -o ex3 test3.cpp `pkg-config --libs opencv`
ex4 : test4.cpp
	g++ `pkg-config --cflags opencv` -o ex4 test4.cpp `pkg-config --libs opencv`
ex5 : test5.cpp
	g++ `pkg-config --cflags opencv` -o ex5 test5.cpp `pkg-config --libs opencv`
ex6 : test6.cpp
	g++ `pkg-config --cflags opencv` -o ex6 test6.cpp `pkg-config --libs opencv`
clean:
	rm -rf exemple
