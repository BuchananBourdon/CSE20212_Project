RM = rm -f

CPPFLAGS = -g -Wall -Werror -Wno-parentheses

CPP_FILES = main.cpp Game.cpp IPaddress.cpp JoinMessage.cpp \
	JoinResponseMessage.cpp Map.cpp Message.cpp MoveUnitOrder.cpp \
	Order.cpp PlayerTurn.cpp PlayerTurnMessage.cpp Random.cpp Turn.cpp

DEPENDS = $(CPP_FILES:.cpp=.d)
OBJECTS = $(CPP_FILES:.cpp=.o)

LIBS = -lSDL -lSDL_net

all: main

main: $(OBJECTS)
	g++ $(CPPFLAGS) -o $@ $^ $(LIBS)

%.d: %.cpp
	g++ -MM -MF $@ $<

%.o: %.cpp
	g++ -c $(CPPFLAGS) -o $@ $<

-include $(DEPENDS)

.PHONY: clean

clean:
	$(RM) main
	$(RM) $(DEPENDS)
	$(RM) $(OBJECTS)

