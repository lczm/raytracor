all: main.cpp
	g++ -Wall -O2 -o main main.cpp

run: main
	./main ${ARGS} > image.ppm && feh image.ppm

clean:
	$(RM) main

