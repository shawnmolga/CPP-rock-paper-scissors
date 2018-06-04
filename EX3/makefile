COMP = g++-5.3.0
#put all your object files here
OBJS = AlgorithmRegistration.o Cell.o main.o Player.o RPSGame.o FilePlayerAlgorithm.o libRSPPlayer_204157861.so RPSBoard.o RPSFight.o RPSJokerChange.o RPSMove.o RPSPiecePosition.o RPSpoint.o AICell.o AIBoard.o TournamentManager.o PlayerAlgorithmInfo.o RSPPlayer_204157861.o
#The executabel filename DON'T CHANGE
EXEC = ex3
LIBS = -ldl

THREAD_FLAG = -pthread

CPP_COMP_FLAG = -std=c++14 -Wall -Wextra \
-Werror -pedantic-errors -DNDEBUG 

$(EXEC): $(OBJS)
	$(COMP) $(OBJS) -o $@ $(THREAD_FLAG) $(LIBS)
#a rule for building a simple c++ source file
#use g++ -MM main.cpp to see dependencies
main.o: main.cpp Constants.h RPSGame.h  #put dependencies here!
	$(COMP)  -rdynamic $(CPP_COMP_FLAG) -c $*.cpp 
Cell.o: Cell.cpp Cell.h Constants.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp -mcmodel=large
Player.o: Player.cpp Player.h Constants.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp -mcmodel=large
RPSGame.o: RPSGame.cpp RPSGame.h Cell.h Point.h RPSBoard.h RPSFight.h RPSMove.h RPSJokerChange.h RPSBoard.h RPSpoint.h PiecePosition.h FilePlayerAlgorithm.h Player.h  PlayerAlgorithmInfo.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp -mcmodel=large
FilePlayerAlgorithm.o: FilePlayerAlgorithm.cpp FilePlayerAlgorithm.h PlayerAlgorithm.h RPSPiecePosition.h RPSpoint.h FightInfo.h RPSMove.h RPSJokerChange.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp -mcmodel=large
#libcircle.so: circle.o
#	g++ -shared -Wl,-soname,libcircle.so -o libcircle.so circle.o

RSPPlayer_204157861.o: RSPPlayer_204157861.cpp RSPPlayer_204157861.h AICell.h PlayerAlgorithm.h Constants.h RPSPiecePosition.h RPSpoint.h RPSJokerChange.h AIBoard.h RPSMove.h 
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp -mcmodel=large 
libRSPPlayer_204157861.so: RSPPlayer_204157861.o
	$(COMP) $(CPP_COMP_FLAG) -shared -Wl,-soname,libRSPPlayer_204157861.so -o libRSPPlayer_204157861.so RSPPlayer_204157861.o
TournamentManager.o: TournamentManager.cpp TournamentManager.h PlayerAlgorithm.h PlayerAlgorithmInfo.h Constants.h RPSGame.h 
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp  -mcmodel=large
AlgorithmRegistration.o: AlgorithmRegistration.cpp AlgorithmRegistration.h PlayerAlgorithm.h TournamentManager.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp -mcmodel=large
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
#ThreadPool.o : ThreadPool.cpp
#	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp -mcmodel=large
AIBoard.o: AIBoard.cpp AIBoard.h Board.h Constants.h AICell.h Point.h RPSpoint.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp -mcmodel=large
PlayerAlgorithmInfo.o:  PlayerAlgorithmInfo.cpp PlayerAlgorithmInfo.h PlayerAlgorithm.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp -mcmodel=large
clean:
	rm -f $(OBJS) $(EXEC)