SOURCES := $(wildcard *.cpp)
OBJECTS := $(patsubst %.cpp,%.o,$(SOURCES))
DEPENDS := $(patsubst %.cpp,%.d,$(SOURCES))

WARNINGS := -Wall -Wextra
FLAGS := -O2

.PHONY: all clean

all: raytracor

clean:
	$(RM) $(OBJECTS) $(DEPENDS) raytracor

raytracor: $(OBJECTS)
	g++ $(WARNINGS) $(FLAGS) $^ -o $@

run: raytracor
	./raytracor $(ARGS) > image.ppm && convert image.ppm image.png && feh image.png

-include $(DEPENDS)

%.o: %.cpp Makefile
	g++ $(WARNINGS) $(FLAGS) -MMD -MP -c $< -o $@
