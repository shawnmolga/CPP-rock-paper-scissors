COMP = g++-5.3.0
OBJS = AlgorithmRegistration.o Cell.o main.o Player.o RPSGame.o  RPSBoard.o RPSFight.o RPSJokerChange.o RPSMove.o RPSPiecePosition.o RPSpoint.o AICell.o AIBoard.o TournamentManager.o PlayerAlgorithmInfo.o
EXEC = ex3
CPP_COMP_FLAG = -std=c++14 -Wall -Wextra \
-Werror -pedantic-errors -DNDEBUG -shared -fPIC -rdynamic -ldl -pthread -lpthread
LIBS = -ldl -lm

rps_tournament: $(OBJS)
		$(COMP) $(OBJS) -o $(EXEC) -ldl -pthread -lpthread -rdynamic 
main.o: main.cpp Constants.h RPSGame.h  #put dependencies here!
	$(COMP)  -rdynamic $(CPP_COMP_FLAG) -c $*.cpp 
Cell.o: Cell.cpp Cell.h Constants.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp -mcmodel=large
Player.o: Player.cpp Player.h Constants.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp -mcmodel=large
RPSGame.o: RPSGame.cpp RPSGame.h Cell.h Point.h RPSBoard.h RPSFight.h RPSMove.h RPSJokerChange.h RPSBoard.h RPSpoint.h PiecePosition.h  Player.h  PlayerAlgorithmInfo.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp -mcmodel=large
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
AIBoard.o: AIBoard.cpp AIBoard.h Board.h Constants.h AICell.h Point.h RPSpoint.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp -mcmodel=large
PlayerAlgorithmInfo.o:  PlayerAlgorithmInfo.cpp PlayerAlgorithmInfo.h PlayerAlgorithm.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp -mcmodel=large
		
rps_lib: RSPPlayer_204157861.cpp RSPPlayer_204157861.h Constants.h RPSPiecePosition.h RPSpoint.h RPSJokerChange.h PlayerAlgorithm.h AIBoard.h RPSMove.h AlgorithmRegistration.h 
		$(COMP) $(CPP_COMP_FLAG) RSPPlayer_204157861.cpp RPSPiecePosition.cpp RPSpoint.cpp RPSJokerChange.cpp AIBoard.cpp RPSBoard.cpp RPSMove.cpp AICell.cpp Cell.cpp -shared -fPIC -rdynamic -mcmodel=large -o RSPPlayer_204157861.so

	
clean:
	rm -f *.o *.so $(EXEC) 