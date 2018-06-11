COMP = g++-5.3.0
#put all your object files here
OBJS = Piece.o main.o #GameBoard.o
#The executabel filename DON'T CHANGE
EXEC = ex4
CPP_COMP_FLAG = -std=c++14 -Wall -Wextra \
-Werror -pedantic-errors -DNDEBUG

$(EXEC): $(OBJS)
	$(COMP) $(OBJS) -o $@
#a rule for building a simple c++ source file
#use g++ -MM main.cpp to see dependencies
main.o: main.cpp GameBoard.h Piece.h unit_test_util.h 
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp
Piece.o: Piece.cpp Piece.h 
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp
#GameBoard.o: GameBoard.h
#	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp
clean:
	rm -f $(OBJS) $(EXEC)