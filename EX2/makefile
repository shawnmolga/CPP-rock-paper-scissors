COMP = g++-5.3.0
#put all your object files here
OBJS = Cell.o main.o Player.o RPSGame.o FilePlayerAlgorithm.o RPSBoard.o RPSFight.o RPSJokerChange.o RPSMove.o RPSPiecePosition.o RPSpoint.o
#The executabel filename DON'T CHANGE
EXEC = ex2
CPP_COMP_FLAG = -std=c++14 -Wall -Wextra \
-Werror -pedantic-errors -DNDEBUG

$(EXEC): $(OBJS)
	$(COMP) $(OBJS) -o $@
#a rule for building a simple c++ source file
#use g++ -MM main.cpp to see dependencies
main.o: main.cpp Constants.h RPSGame.h #put dependencies here!
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp
Cell.o: Cell.cpp Cell.h Constants.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp
Player.o: Player.cpp Player.h Constants.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp
RPSGame.o: RPSGame.cpp Cell.h Point.h RPSBoard.h RPSFight.h RPSMove.h RPSJokerChange.h RPSBoard.h RPSpoint.h PiecePosition.h FilePlayerAlgorithm.h Player.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp
FilePlayerAlgorithm.o: FilePlayerAlgorithm.cpp FilePlayerAlgorithm.h PlayerAlgorithm.h RPSPiecePosition.h RPSpoint.h FightInfo.h RPSMove.h RPSJokerChange.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp
RPSBoard.o: RPSBoard.cpp RPSBoard.h Board.h Constants.h Cell.h Point.h RPSpoint.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp
RPSFight.o: RPSFight.cpp RPSFight.h FightInfo.h Point.h RPSpoint.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp
RPSJokerChange.o: RPSJokerChange.cpp RPSJokerChange.h JokerChange.h Point.h RPSpoint.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp
RPSMove.o: RPSMove.cpp RPSMove.h Move.h Point.h RPSpoint.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp
RPSPiecePosition.o: RPSPiecePosition.cpp RPSPiecePosition.h PiecePosition.h RPSpoint.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp
RPSpoint.o: RPSpoint.cpp RPSpoint.h Point.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp
clean:
	rm -f $(OBJS) $(EXEC)