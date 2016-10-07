all: mod1

mod1 : mod1.cpp
	g++ `pkg-config --cflags opencv` -o mod1 mod1.cpp `pkg-config --libs opencv`
clean:
	rm -rf exemple
