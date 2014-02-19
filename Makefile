CXXFLAGS = -g -Wall -Werror -Wno-parentheses

LIBS = -lSDL -lSDL_net

CXXSRCS = main.cpp Game.cpp GameSimulation.cpp LocalPlayer.cpp \
	NetworkPlayer.cpp Order.cpp

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

