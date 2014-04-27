RM = rm -f

CPPFLAGS = -g -Wall -Werror -Wno-parentheses

CPP_FILES = main.cpp ActionBar.cpp AttackUnitOrder.cpp BlackHole.cpp Bunny.cpp \
	CreateUnitOrder.cpp Game.cpp HangupMessage.cpp IPaddress.cpp \
	JoinMessage.cpp JoinResponseMessage.cpp Map.cpp Message.cpp \
	MoveUnitOrder.cpp Order.cpp Path.cpp PlayerTurn.cpp \
	PlayerTurnMessage.cpp Random.cpp Robot.cpp SpawnBunny.cpp Turn.cpp \
	Unit.cpp View.cpp

DEPENDS = $(CPP_FILES:.cpp=.d)
OBJECTS = $(CPP_FILES:.cpp=.o)

LIBS = -lSDL -lSDL_net -lSDL_gfx -lSDL_image

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

