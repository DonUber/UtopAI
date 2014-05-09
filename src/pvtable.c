#include "inc.h"
#include "stdio.h"

const int pvTableSize = 0x100000 * 2;

void storePvMove(const BOARD *board, const int move){
	int i = board->POS_KEY % board->pvTable->nEntries;
	board->pvTable->pTable[i].move = move;
	board->pvTable->pTable[i].POS_KEY = board->POS_KEY;
}

int getPvLine(const int depth, BOARD *board){
	int move = probePvTable(board);
	int count = 0;
	while(move != 0 && count<depth){
		if(moveExists(board,move)){
			makeMove(board,move);
			board->pvArray[count++] = move;
		}else{
			break;
		}
		move = probePvTable(board);
	}
	while(board->ply > 0){
		takeMove(board);
	}
	return count;
}

void clearPvTable(PVTABLE *table){
	PVENTRY *pvEntry;
	for(pvEntry=table->pTable;pvEntry<table->pTable+table->nEntries;pvEntry++){
		pvEntry->POS_KEY = 0ULL;
		pvEntry->move = 0;
	}
	
}

void initPvTable(PVTABLE *table){
	table->nEntries = pvTableSize / sizeof(PVENTRY);
	table->nEntries -= 2;
	//free(table->pTable);
	table->pTable = (PVENTRY *) malloc(table->nEntries * sizeof(PVENTRY));
	clearPvTable(table);
	printf("PvTable init complete with %d entries\n",table->nEntries);
}

int probePvTable(const BOARD *board){
	int i = board->POS_KEY % board->pvTable->nEntries;
	if(board->pvTable->pTable[i].POS_KEY == board->POS_KEY){
		return board->pvTable->pTable[i].move;
	}
	return 0;
}
