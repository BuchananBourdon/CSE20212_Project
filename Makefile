CPPFLAGS = -g -Wall -Werror

CPP_FILES = main.cpp Game.cpp

DEPENDS = $(CPP_FILES:.cpp=.d)
OBJECTS = $(CPP_FILES:.cpp=.o)

all: main

main: $(OBJECTS)
	g++ $(CPPFLAGS) -o $@ $^

%.d: %.cpp
	g++ -MM -MF $@ $<

%.o: %.cpp
	g++ -c $(CPPFLAGS) -o $@ $<

-include $(DEPENDS)

.PHONY: clean

clean:
	rm main
	rm $(DEPENDS)
	rm $(OBJECTS)

