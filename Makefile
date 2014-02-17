CXXFLAGS = -g -Wall -Werror

LIBS = -lSDL

CXXSRCS = main.cpp game_loop.cpp

DEPS = $(CXXSRCS:.cpp=.d)
OBJS = $(CXXSRCS:.cpp=.o)

all: main

main: $(OBJS)
	$(CXX) -o $@ $^ $(LIBS)

%.d: %.cpp
	$(CXX) -MM -MF $@ $<

%.o: %.cpp
	$(CXX) -c $(CXXFLAGS) -o $@ $<

-include $(DEPS)

.PHONY: clean

clean:
	$(RM) main
	$(RM) $(DEPS)
	$(RM) $(OBJS)

