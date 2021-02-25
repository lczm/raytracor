all: main.cpp
	g++ -Wall -o main main.cpp vec3.h color.h

run: main
	./main ${ARGS} > image.ppm && feh image.ppm

clean:
	$(RM) main
