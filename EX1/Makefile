COMP = g++-5.3.0
#put all your object files here
OBJS = Cell.o main.o Player.o RCPgame.o RCPgameManager.o
#The executabel filename DON'T CHANGE
EXEC = ex1
CPP_COMP_FLAG = -std=c++14 -Wall -Wextra \
-Werror -pedantic-errors -DNDEBUG

$(EXEC): $(OBJS)
	$(COMP) $(OBJS) -o $@
#a rule for building a simple c++ source file
#use g++ -MM main.cpp to see dependencies
main.o: main.cpp Constants.h RCPgameManager.h #put dependencies here!
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp
Cell.o: Cell.cpp Cell.h Constants.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp
Player.o: Player.cpp Player.h Constants.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp
RCPgame.o: RCPgame.cpp Cell.h Player.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp
RCPgameManager.o: RCPgameManager.cpp Constants.h Cell.h Player.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp
clean:
	rm -f $(OBJS) $(EXEC)