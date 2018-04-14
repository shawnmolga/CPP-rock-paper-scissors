If the game is started, it can be over due to "no moving pieces for player" reason if the *current* player 
that suppose to make a move, doesn't have any moving pieces.
It can effect the following senarios:
1. After positioing both players' pieces, If player 2 does not have any moving pieces, because this is player 1's turn,
if he has moving pieces he will try to move. Only when player 2's turn will come, and no other reason to finish the game has occurred,
then he will lose immidiatly since he doesnt have a piece to move.
2. After positioning both players' pieces, If player 2 does not have any moving pieces and player 1 either, player 1 will lose since 
this is his turn and he is not able to move.
3. After positioning both players' pieces, if player 2 does not have any moving piece and his move file is empty,
player one will play all of his moves in his move file. (since this was his turn to play and he was able to play, the game continues).
4. If current player made a move and left with no moving pieces, and next player's has moving pieces
but his next move is illegal (i.e. bad format), then since current player finished his move successfully, 
this is now next player's turn, he can move so we will try to commit his move but then will se the move is illegal -
so the game will be over since move file of that player was in wrong format.
5. if both move files are finished and both players has no pieces to move - the next player that was suppose to play will lose.
again, due to the rational that he was suppose to play but not able to.

Please notice that the explenation above is related only to no moving pieces situation and if the game suppose to be over due to
any other reason it will happen according to game rules.

Empty lines and spaces are ignored.
