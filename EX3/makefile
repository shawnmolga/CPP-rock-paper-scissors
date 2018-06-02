COMP = g++-5.3.0
#put all your object files here
OBJS = Cell.o main.o Player.o RPSGame.o FilePlayerAlgorithm.o RSPPlayer_204157861.so RPSBoard.o RPSFight.o RPSJokerChange.o RPSMove.o RPSPiecePosition.o RPSpoint.o AICell.o AIBoard.o ThreadPool.o TournamentManager.o PlayerAlgorithmInfo.o
#The executabel filename DON'T CHANGE
EXEC = ex2
LIBS = -ldl

CPP_COMP_FLAG = -std=c++14 -Wall -Wextra \
-Werror -pedantic-errors -DNDEBUG

$(EXEC): $(OBJS)
	$(COMP) $(OBJS) -o $@
#a rule for building a simple c++ source file
#use g++ -MM main.cpp to see dependencies
main.o: main.cpp Constants.h RPSGame.h  #put dependencies here!
	$(COMP)  -rdynamic $(CPP_COMP_FLAG) -c $*.cpp $(LIBS)
Cell.o: Cell.cpp Cell.h Constants.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp -mcmodel=large
Player.o: Player.cpp Player.h Constants.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp -mcmodel=large
RPSGame.o: RPSGame.cpp Cell.h Point.h RPSBoard.h RPSFight.h RPSMove.h RPSJokerChange.h RPSBoard.h RPSpoint.h PiecePosition.h FilePlayerAlgorithm.h Player.h 
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp -mcmodel=large
FilePlayerAlgorithm.o: FilePlayerAlgorithm.cpp FilePlayerAlgorithm.h PlayerAlgorithm.h RPSPiecePosition.h RPSpoint.h FightInfo.h RPSMove.h RPSJokerChange.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp -mcmodel=large
#libcircle.so: circle.o
#	g++ -shared -Wl,-soname,libcircle.so -o libcircle.so circle.o
RSPPlayer_204157861.so: RSPPlayer_204157861.cpp RPSPiecePosition.h Constants.h
	$(COMP) -shared -Wl,-soname,RSPPlayer_204157861.so $(CPP_COMP_FLAG) 
	#-c $*.cpp -mcmodel=large
RPSBoard.o: RPSBoard.cpp RPSBoard.h Board.h Constants.h Cell.h Point.h RPSpoint.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp -mcmodel=large
RPSFight.o: RPSFight.cpp RPSFight.h FightInfo.h Point.h RPSpoint.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp -mcmodel=large
RPSJokerChange.o: RPSJokerChange.cpp RPSJokerChange.h JokerChange.h  RPSPiecePosition.h  Point.h RPSpoint.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp -mcmodel=large
RPSMove.o: RPSMove.cpp RPSMove.h Move.h Point.h RPSpoint.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp -mcmodel=large
RPSPiecePosition.o: RPSPiecePosition.cpp RPSPiecePosition.h PiecePosition.h RPSpoint.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp -mcmodel=large
RPSpoint.o: RPSpoint.cpp RPSpoint.h Point.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp -mcmodel=large
AICell.o: AICell.cpp AICell.h Cell.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp -mcmodel=large
ThreadPool.o : ThreadPool.cpp
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp -mcmodel=large
AIBoard.o: AIBoard.cpp AIBoard.h Board.h Constants.h AICell.h Point.h RPSpoint.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp -mcmodel=large
PlayerAlgorithmInfo.o:  PlayerAlgorithmInfo.cpp PlayerAlgorithm.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp -mcmodel=large
TournamentManager.o: TournamentManager.cpp PlayerAlgorithm.h PlayerAlgorithmInfo.h Constants.h RPSGame.h ThreadPool.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp -mcmodel=large
clean:
	rm -f $(OBJS) $(EXEC)