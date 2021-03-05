SOURCES := $(wildcard *.cpp)
OBJECTS := $(patsubst %.cpp,%.o,$(SOURCES))
DEPENDS := $(patsubst %.cpp,%.d,$(SOURCES))

WARNINGS := -Wall -Wextra

.PHONY: all clean

all: raytracor

clean:
	$(RM) $(OBJECTS) $(DEPENDS) raytracor

raytracor: $(OBJECTS)
	g++ $(WARNINGS) $^ -o $@

run: raytracor
	./raytracor $(ARGS) > image.ppm && feh image.ppm

-include $(DEPENDS)

%.o: %.cpp Makefile
	g++ $(WARNINGS) -MMD -MP -c $< -o $@
