#include "inc.h"
#include "stdio.h"
int main(){
	/*init();
	BOARD board[1];
	MOVELIST list[1];
	SEARCHINFO info[1];
	setPos("rnbqkb1r/pp1p1ppp/2p5/4P3/2B5/8/PPP1NnPP/RNBQK2R w KQkq - 0 6", board); // Invoegen fifty rule from fen
	info->depth = 6;
	initPvTable(board->pvTable);
	searchPos(board, info);
	getchar();
	return 0;*/
	init();
	uciLoop();
	return 0;
}
