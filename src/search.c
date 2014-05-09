#include "inc.h"
#include "stdio.h"

#define INF 30000
#define MATE 29000

static void checkUp(SEARCHINFO *info){
	if(info->timeset == 1 && getTimeMs() > info->stoptime){
		info->stopped = 1;
	}
	readInput(info);
}

static void pickNextMove(int nMove, MOVELIST *list){
	MOVE temp;
	int i = 0;
	int bestScore = 0;
	int bestNum = nMove;
	for(i=nMove;i<list->count;++i){
		if(list->moves[i].score > bestScore){
			bestScore = list->moves[i].score;
			bestNum = i;
		}
	}
		temp = list->moves[nMove];
		list->moves[nMove] = list->moves[bestNum];
		list->moves[bestNum] = temp;
}

static int isRep(const BOARD *board){
	int i=0;
	for(i=board->hisPly-board->fifty;i<board->hisPly-1;++i){
		if(board->POS_KEY==board->history[i].POS_KEY){
			return 1;
		}
	}
	return 0;
}

static void clearSearch(BOARD *board, SEARCHINFO *info){
	int i=0;
	int j=0;
	for(i=0;i<13;i++){
		for(j=0;j<64;++j){
			board->searchHistory[i][j] = 0;
		}
	}
	for(i=0;i<2;i++){
		for(j=0;j<MAX_DEPTH;++j){
			board->searchKillers[i][j] = 0;
		}
	}
	clearPvTable(board->pvTable);
	board->ply=0;
	info->starttime = getTimeMs();
	info->stopped = 0;
	info->nodes = 0;
}

static int quiescence(int alpha, int beta, BOARD *board, SEARCHINFO *info){
	if((info->nodes & 2047) == 0){
		checkUp(info);
	}
	info->nodes++;
	if((isRep(board) || board->fifty >= 100) && board->ply){
		return 0;
	}
	if(board->ply > MAX_DEPTH - 1){
	      return evalPos(board);
	}

	int score = evalPos(board);
	if(score >= beta){
		return beta;
	}
	if(score > alpha){
		alpha = score;
	}
	MOVELIST list[1];
	genCapMoves(board,list);
	int nMove = 0,legal = 0;
	int oldAlpha = alpha;
	int bestMove = 0;
	score = -INF;
	int pvMove = probePvTable(board);
	if( pvMove != 0 ) {
		for(nMove = 0; nMove < list->count; ++nMove) {
			if( list->moves[nMove].move == pvMove) {
				list->moves[nMove].score = 2000000;
				break;
			}
		}
	}

	for(nMove=0;nMove<list->count;nMove++){
		pickNextMove(nMove,list);
		//if(CAPTURED(list->moves[nMove].move) != 0){
			if(!makeMove(board,list->moves[nMove].move)){
				continue;
			}
			legal++;
			score = -quiescence(-beta,-alpha,board,info);
			takeMove(board);
			if(info->stopped == 1){
				return 0;
			}else if(score > alpha){
				if(score >= beta){
					return beta;
				}
				alpha = score;
				bestMove = list->moves[nMove].move;
			}
		//}else{
		//	continue;
		//}
	}
	
	if(alpha != oldAlpha){
		storePvMove(board, bestMove);
	}
	return alpha;
}

static int alphaBeta(int alpha, int beta, int depth, BOARD *board, SEARCHINFO *info, int doNull){
	if(depth == 0){
		info->nodes++;
		return quiescence(alpha, beta, board, info);
	}
	if((info->nodes & 2047) == 0){
		checkUp(info);
	}
	info->nodes++;
	if((isRep(board) || board->fifty >= 100) && board->ply){
		return 0;
	}
	if(board->ply > MAX_DEPTH - 1){
	      return evalPos(board);
	}
	MOVELIST list[1];
	genMoves(board,list);
	int inCheck = sqAttacked(board->kings[board->side],board->side^1,board);
	if(inCheck) depth++;
	
	int nMove;
	int legal = 0;
	int oldAlpha = alpha;
	int bestMove = 0;
	int score = -INF;
	int pvMove = probePvTable(board);
	
	if(pvMove != 0){
		for(nMove=0;nMove<list->count;++nMove){
			if(list->moves[nMove].move == pvMove){
				list->moves[nMove].score = 2000000;
			}
		}
	}
	for(nMove=0;nMove<list->count;nMove++){
		pickNextMove(nMove,list);
		if(!makeMove(board,list->moves[nMove].move)){
			continue;
		}
		legal++;
		score = -alphaBeta(-beta,-alpha,depth-1,board,info,1);
		takeMove(board);
		if(info->stopped == 1){
			return 0;
		}
		if(score > alpha){
			if(score >= beta){
				//if(legal == 1){
				//	info->fhf++;
				//}
				//info->fh++;
				if(!(list->moves[nMove].move & MFLAGCAP)){
					board->searchKillers[1][board->ply] = board->searchKillers[0][board->ply];
					board->searchKillers[0][board->ply] = list->moves[nMove].move;
				}
				return beta;
			}
			alpha = score;
			bestMove = list->moves[nMove].move;
			if(!(list->moves[nMove].move & MFLAGCAP)){
				board->searchHistory[pieceSq(board, FROMSQ(bestMove))][TOSQ(bestMove)] += depth;
			}
		}
	}
	
	if(legal == 0){
		if(inCheck){
			return -MATE + board->ply;
		}else{
			return 0;
		}
	}
	if(alpha != oldAlpha){
		storePvMove(board, bestMove);
	}
	return alpha;
}


void searchPos(BOARD *board, SEARCHINFO *info){
	int bestMove = 0;
	int bestScore = -INF;
	int currentDepth;
	int pvMoves = 0;
	int nPv = 0;
	clearSearch(board,info);
	
	for(currentDepth=1;currentDepth<=info->depth;currentDepth++){
		bestScore = alphaBeta(-INF,INF,currentDepth,board,info,1);
		if(info->stopped == 1){
			break;
		}
		pvMoves = getPvLine(currentDepth, board);
		bestMove = board->pvArray[0];
		printf("info score cp %d depth %d nodes %ld time %ld ",
			bestScore,currentDepth,info->nodes,getTimeMs()-info->starttime);
			
		pvMoves = getPvLine(currentDepth, board);	
		printf("pv");		
		for(nPv = 0; nPv< pvMoves; ++nPv) {
			printf(" %s",printMove(board->pvArray[nPv]));
		}
		printf("\n");
		//printf("Ordering:%.2f\n",(info->fhf/info->fh));
	}
	printf("bestmove %s\n", printMove(bestMove));
}
