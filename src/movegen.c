#include "inc.h"
#include "stdio.h"

static int mvvLvaScore[13][13];
const int victimScore[13]={0,100,200,300,400,500,600,100,200,300,400,500,600};

void initMvvLva(){
	int att;
	int vic;
	for(att=wP;att<=bK;++att){
		for(vic=wP;vic<=bK;++vic){
			mvvLvaScore[vic][att] = victimScore[vic]*100+6-victimScore[att];
		}
	}
}

int pieceSq(const BOARD *board, const int i){
	if(1ULL<<i & board->WHITE_PAWN){
		return wP;
	}else if(1ULL<<i & board->WHITE_KNIGHT){
		return wN;
	}else if(1ULL<<i & board->WHITE_BISHOP){
		return wB;
	}else if(1ULL<<i & board->WHITE_ROOK){
		return wR;
	}else if(1ULL<<i & board->WHITE_QUEEN){
		return wQ;
	}else if(1ULL<<i & board->WHITE_KING){
		return wK;
	}else if(1ULL<<i & board->BLACK_PAWN){
		return bP;
	}else if(1ULL<<i & board->BLACK_KNIGHT){
		return bN;
	}else if(1ULL<<i & board->BLACK_BISHOP){
		return bB;
	}else if(1ULL<<i & board->BLACK_ROOK){
		return bR;
	}else if(1ULL<<i & board->BLACK_QUEEN){
		return bQ;
	}else if(1ULL<<i & board->BLACK_KING){
		return bK;
	}else{
		return EMPTY;
	}

}

static void AddQuietMove(const BOARD *board, int move, MOVELIST *list ) {
	list->moves[list->count].move = move;
	//list->moves[list->count].score = pieceSq(board, FROMSQ(move));
	if(board->searchKillers[0][board->ply] == move){
		list->moves[list->count].score = 900000;
	}else if(board->searchKillers[1][board->ply] == move){
		list->moves[list->count].score = 800000;
	}else{
		//list->moves[list->count].score = board->searchHistory[pieceSq(board, FROMSQ(move))][TOSQ(move)];
		list->moves[list->count].score = pieceSq(board, FROMSQ(move)) + board->searchHistory[pieceSq(board, FROMSQ(move))][TOSQ(move)];
	}
	list->count++;
}

static void AddCaptureMove(const BOARD *board, int move, MOVELIST *list ) {
	list->moves[list->count].move = move;
	list->moves[list->count].score = mvvLvaScore[CAPTURED(move)][pieceSq(board,FROMSQ(move))] + 9000001;
	list->count++;
}

static void genKnightMoves(BOARD *board, MOVELIST *list, int i, int f, int r){
	int tsq,tc;
	tsq = i+6;
	if(f > 1 && r != 7){
		tc = pieceSq(board, tsq);
		if(tc != EMPTY){
			if(board->side == 1){
				if(tc > 6){
					AddCaptureMove(board, BITMOVE(i,tsq,tc,0,0), list);
				}
			}else{
				if(tc < 7){
					AddCaptureMove(board, BITMOVE(i,tsq,tc,0,0), list);
				}
			}
		}else{
			AddQuietMove(board, BITMOVE(i,tsq,0,0,0), list);
		}
	}
	tsq = i+10;
	if(f < 6 && r != 7){
		tc = pieceSq(board, tsq);
		if(tc != EMPTY){
			if(board->side == 1){
				if(tc > 6){
					AddCaptureMove(board, BITMOVE(i,tsq,tc,0,0), list);
				}
			}else{
				if(tc < 7){
					AddCaptureMove(board, BITMOVE(i,tsq,tc,0,0), list);
				}
			}
		}else{
			AddQuietMove(board, BITMOVE(i,tsq,0,0,0), list);
		}
	}
	tsq = i+15;
	if(f != 0 && r < 6){
		tc = pieceSq(board, tsq);
		if(tc != EMPTY){
			if(board->side == 1){
				if(tc > 6){
					AddCaptureMove(board, BITMOVE(i,tsq,tc,0,0), list);
				}
			}else{
				if(tc < 7){
					AddCaptureMove(board, BITMOVE(i,tsq,tc,0,0), list);
				}
			}
		}else{
			AddQuietMove(board, BITMOVE(i,tsq,0,0,0), list);
		}
	}										
	tsq = i+17;
	if(f != 7 && r < 6){
		tc = pieceSq(board, tsq);
		if(tc != EMPTY){
			if(board->side == 1){
				if(tc > 6){
					AddCaptureMove(board, BITMOVE(i,tsq,tc,0,0), list);
				}
			}else{
				if(tc < 7){
					AddCaptureMove(board, BITMOVE(i,tsq,tc,0,0), list);
				}
			}
		}else{
			AddQuietMove(board, BITMOVE(i,tsq,0,0,0), list);
		}
	}	
	tsq = i-6;
	if(f < 6 && r != 0){
		tc = pieceSq(board, tsq);
		if(tc != EMPTY){
			if(board->side == 1){
				if(tc > 6){
					AddCaptureMove(board, BITMOVE(i,tsq,tc,0,0), list);
				}
			}else{
				if(tc < 7){
					AddCaptureMove(board, BITMOVE(i,tsq,tc,0,0), list);
				}
			}
		}else{
			AddQuietMove(board, BITMOVE(i,tsq,0,0,0), list);
		}
	}
	tsq = i-10;
	if(f > 1 && r != 0){
		tc = pieceSq(board, tsq);
		if(tc != EMPTY){
			if(board->side == 1){
				if(tc > 6){
					AddCaptureMove(board, BITMOVE(i,tsq,tc,0,0), list);
				}
			}else{
				if(tc < 7){
					AddCaptureMove(board, BITMOVE(i,tsq,tc,0,0), list);
				}
			}
		}else{
			AddQuietMove(board, BITMOVE(i,tsq,0,0,0), list);
		}
	}
	tsq = i-15;
	if(f != 7 && r > 1){
		tc = pieceSq(board, tsq);
		if(tc != EMPTY){
			if(board->side == 1){
				if(tc > 6){
					AddCaptureMove(board, BITMOVE(i,tsq,tc,0,0), list);
				}
			}else{
				if(tc < 7){
					AddCaptureMove(board, BITMOVE(i,tsq,tc,0,0), list);
				}
			}
		}else{
			AddQuietMove(board, BITMOVE(i,tsq,0,0,0), list);
		}
	}		
	tsq = i-17;
	if(f != 0 && r > 1){
		tc = pieceSq(board, tsq);
		if(tc != EMPTY){
			if(board->side == 1){
				if(tc > 6){
					AddCaptureMove(board, BITMOVE(i,tsq,tc,0,0), list);
				}
			}else{
				if(tc < 7){
					AddCaptureMove(board, BITMOVE(i,tsq,tc,0,0), list);
				}
			}
		}else{
			AddQuietMove(board, BITMOVE(i,tsq,0,0,0), list);
		}
	}	
}

static void genRookMoves(BOARD *board, MOVELIST *list, int i, int f, int r){
	int tsq,mv=1,tc;
	while((7-r) >= mv){
		tsq = i+mv*8;
		tc = pieceSq(board, tsq);
		if(tc != EMPTY){
			if(board->side == 1){
				if(tc > 6){
					AddCaptureMove(board, BITMOVE(i,tsq,tc,0,0), list);
				}
			}else{
				if(tc < 7){
					AddCaptureMove(board, BITMOVE(i,tsq,tc,0,0), list);
				}
			}
			break;
		}else{
			AddQuietMove(board, BITMOVE(i,tsq,0,0,0), list);
		}
		mv++;
	}
	mv=1;
	while((7-f) >= mv){
		tsq = i+mv*1;
		tc = pieceSq(board, tsq);
		if(tc != EMPTY){
			if(board->side == 1){
				if(tc > 6){
					AddCaptureMove(board, BITMOVE(i,tsq,tc,0,0), list);
				}
			}else{
				if(tc < 7){
					AddCaptureMove(board, BITMOVE(i,tsq,tc,0,0), list);
				}
			}
			break;
		}else{
			AddQuietMove(board, BITMOVE(i,tsq,0,0,0), list);
		}
		mv++;
	}
	mv=1;
	while(r >= mv){
		tsq = i-mv*8;
		tc = pieceSq(board, tsq);
		if(tc != EMPTY){
			if(board->side == 1){
				if(tc > 6){
					AddCaptureMove(board, BITMOVE(i,tsq,tc,0,0), list);
				}
			}else{
				if(tc < 7){
					AddCaptureMove(board, BITMOVE(i,tsq,tc,0,0), list);
				}
			}
			break;
		}else{
			AddQuietMove(board, BITMOVE(i,tsq,0,0,0), list);
		}
		mv++;
	}
	mv=1;
	while(f >= mv){
		tsq = i-mv;
		tc = pieceSq(board, tsq);
		if(tc != EMPTY){
			if(board->side == 1){
				if(tc > 6){
					AddCaptureMove(board, BITMOVE(i,tsq,tc,0,0), list);
				}
			}else{
				if(tc < 7){
					AddCaptureMove(board, BITMOVE(i,tsq,tc,0,0), list);
				}
			}
			break;
		}else{
			AddQuietMove(board, BITMOVE(i,tsq,0,0,0), list);
		}
		mv++;
	}
}

static void genKingMoves(BOARD *board, MOVELIST *list, int i, int f, int r){
	int tsq, tc;
	tsq=i+8;
	tc = pieceSq(board, tsq);
	if(r != 7){
		if(tc != EMPTY){
			if(board->side == 1){
				if(tc > 6){
					AddCaptureMove(board, BITMOVE(i,tsq,tc,0,0), list);
				}
			}else{
				if(tc < 7){
					AddCaptureMove(board, BITMOVE(i,tsq,tc,0,0), list);
				}
			}
		}else{
			AddQuietMove(board, BITMOVE(i,tsq,0,0,0), list);
		}
	}
	tsq=i-8;
	tc = pieceSq(board, tsq);
	if(r != 0){
		if(tc != EMPTY){
			if(board->side == 1){
				if(tc > 6){
					AddCaptureMove(board, BITMOVE(i,tsq,tc,0,0), list);
				}
			}else{
				if(tc < 7){
					AddCaptureMove(board, BITMOVE(i,tsq,tc,0,0), list);
				}
			}
		}else{
			AddQuietMove(board, BITMOVE(i,tsq,0,0,0), list);
		}
	}
	tsq=i+1;
	tc = pieceSq(board, tsq);
	if(f != 7){
		if(tc != EMPTY){
			if(board->side == 1){
				if(tc > 6){
					AddCaptureMove(board, BITMOVE(i,tsq,tc,0,0), list);
				}
			}else{
				if(tc < 7){
					AddCaptureMove(board, BITMOVE(i,tsq,tc,0,0), list);
				}
			}
		}else{
			AddQuietMove(board, BITMOVE(i,tsq,0,0,0), list);
		}
	}
	tsq=i-1;
	tc = pieceSq(board, tsq);
	if(f != 0){
		if(tc != EMPTY){
			if(board->side == 1){
				if(tc > 6){
					AddCaptureMove(board, BITMOVE(i,tsq,tc,0,0), list);
				}
			}else{
				if(tc < 7){
					AddCaptureMove(board, BITMOVE(i,tsq,tc,0,0), list);
				}
			}
		}else{
			AddQuietMove(board, BITMOVE(i,tsq,0,0,0), list);
		}
	}
	tsq=i+7;
	tc = pieceSq(board, tsq);
	if(f != 0 && r != 7){
		if(tc != EMPTY){
			if(board->side == 1){
				if(tc > 6){
					AddCaptureMove(board, BITMOVE(i,tsq,tc,0,0), list);
				}
			}else{
				if(tc < 7){
					AddCaptureMove(board, BITMOVE(i,tsq,tc,0,0), list);
				}
			}
		}else{
			AddQuietMove(board, BITMOVE(i,tsq,0,0,0), list);
		}
	}
	tsq=i+9;
	tc = pieceSq(board, tsq);
	if(f != 7 && r != 7){
		if(tc != EMPTY){
			if(board->side == 1){
				if(tc > 6){
					AddCaptureMove(board, BITMOVE(i,tsq,tc,0,0), list);
				}
			}else{
				if(tc < 7){
					AddCaptureMove(board, BITMOVE(i,tsq,tc,0,0), list);
				}
			}
		}else{
			AddQuietMove(board, BITMOVE(i,tsq,0,0,0), list);
		}
	}
	tsq=i-7;
	tc = pieceSq(board, tsq);
	if(f != 7 && r != 0){
		if(tc != EMPTY){
			if(board->side == 1){
				if(tc > 6){
					AddCaptureMove(board, BITMOVE(i,tsq,tc,0,0), list);
				}
			}else{
				if(tc < 7){
					AddCaptureMove(board, BITMOVE(i,tsq,tc,0,0), list);
				}
			}
		}else{
			AddQuietMove(board, BITMOVE(i,tsq,0,0,0), list);
		}
	}
	tsq=i-9;
	tc = pieceSq(board, tsq);
	if(f != 0 && r != 0){
		if(tc != EMPTY){
			if(board->side == 1){
				if(tc > 6){
					AddCaptureMove(board, BITMOVE(i,tsq,tc,0,0), list);
				}
			}else{
				if(tc < 7){
					AddCaptureMove(board, BITMOVE(i,tsq,tc,0,0), list);
				}
			}
		}else{
			AddQuietMove(board, BITMOVE(i,tsq,0,0,0), list);
		}
	}
}

static void genBishopMoves(BOARD *board, MOVELIST *list, int i, int f, int r){
	int tsq,mv=1,tc;
	while((7-f) >= mv && (7-r) >= mv){
		tsq = i+mv*9;
		tc = pieceSq(board, tsq);
		if(tc != EMPTY){
			if(board->side == 1){
				if(tc > 6){
					AddCaptureMove(board, BITMOVE(i,tsq,tc,0,0), list);
				}
			}else{
				if(tc < 7){
					AddCaptureMove(board, BITMOVE(i,tsq,tc,0,0), list);
				}
			}
			break;
		}else{
			AddQuietMove(board, BITMOVE(i,tsq,0,0,0), list);
		}
		mv++;
	}
	mv=1;
	while(f >= mv && (7-r) >= mv){
		tsq = i+mv*7;
		tc = pieceSq(board, tsq);
		if(tc != EMPTY){
			if(board->side == 1){
				if(tc > 6){
					AddCaptureMove(board, BITMOVE(i,tsq,tc,0,0), list);
				}
			}else{
				if(tc < 7){
					AddCaptureMove(board, BITMOVE(i,tsq,tc,0,0), list);
				}
			}
			break;
		}else{
			AddQuietMove(board, BITMOVE(i,tsq,0,0,0), list);
		}
		mv++;
	}
	mv=1;
	while(f >= mv && r >= mv){
		tsq = i-mv*9;
		tc = pieceSq(board, tsq);
		if(tc != EMPTY){
			if(board->side == 1){
				if(tc > 6){
					AddCaptureMove(board, BITMOVE(i,tsq,tc,0,0), list);
				}
			}else{
				if(tc < 7){
					AddCaptureMove(board, BITMOVE(i,tsq,tc,0,0), list);
				}
			}
			break;
		}else{
			AddQuietMove(board, BITMOVE(i,tsq,0,0,0), list);
		}
		mv++;
	}
	mv=1;
	while((7-f) >= mv && r >= mv){
		tsq = i-mv*7;
		tc = pieceSq(board, tsq);
		if(tc != EMPTY){
			if(board->side == 1){
				if(tc > 6){
					AddCaptureMove(board, BITMOVE(i,tsq,tc,0,0), list);
				}
			}else{
				if(tc < 7){
					AddCaptureMove(board, BITMOVE(i,tsq,tc,0,0), list);
				}
			}
			break;
		}else{
			AddQuietMove(board, BITMOVE(i,tsq,0,0,0), list);
		}
		mv++;
	}
}

void genMoves(BOARD *board, MOVELIST *list){
	list->count = 0;
	int res,f,r,i,tsq,tc;
	for(i=0;i<64;i++){
		res = board->BOTH>>i & 1;
		f = i & 7;
		r = (i-f) / 8;
		if(res != 0){
			if(board->side == 1){
				if(1ULL<<i & board->WHITE_PAWN){
					tsq = i + 8;
					if(r != 6){
						
						if(!(board->BOTH & 1ULL << tsq)){
							AddQuietMove(board, BITMOVE(i,tsq,0,0,0), list);
							if(r == 1 && pieceSq(board, tsq+8) == EMPTY){
								AddQuietMove(board, BITMOVE(i,tsq+8,0,0,MFLAGPS), list);	
							}
						}
						
						tc = pieceSq(board, tsq-1);
						if(tc > 6 && f != 0){
							AddCaptureMove(board, BITMOVE(i,tsq-1,tc,0,0), list);
						}
						
						tc = pieceSq(board, tsq+1);
						if(tc > 6 && f != 7){
							AddCaptureMove(board, BITMOVE(i,tsq+1,tc,0,0), list);
							
						}
						
						if(board->enPas != 0){
							if(f != 0 && pieceSq(board, tsq-9) == bP && tsq-1==board->enPas){
									AddCaptureMove(board, BITMOVE(i,tsq-1,0,0,MFLAGEP), list);
							}else if(f != 7 && pieceSq(board, tsq-7) == bP && tsq+1==board->enPas){
									AddCaptureMove(board, BITMOVE(i,tsq+1,0,0,MFLAGEP), list);
							}
						}
						
					}else{
						if(!(board->BOTH & 1ULL << tsq)){
							AddQuietMove(board, BITMOVE(i,tsq,0,wN,0), list);
							AddQuietMove(board, BITMOVE(i,tsq,0,wB,0), list);
							AddQuietMove(board, BITMOVE(i,tsq,0,wQ,0), list);
							AddQuietMove(board, BITMOVE(i,tsq,0,wR,0), list);
						}
						tc = pieceSq(board, tsq-1);
						if(tc > 6 && f != 0){
							AddCaptureMove(board, BITMOVE(i,tsq-1,tc,wN,0), list);
							AddCaptureMove(board, BITMOVE(i,tsq-1,tc,wB,0), list);
							AddCaptureMove(board, BITMOVE(i,tsq-1,tc,wQ,0), list);
							AddCaptureMove(board, BITMOVE(i,tsq-1,tc,wR,0), list);
						}
						tc = pieceSq(board, tsq+1);
						if(tc > 6 && f != 7){
							AddCaptureMove(board, BITMOVE(i,tsq+1,tc,wN,0), list);
							AddCaptureMove(board, BITMOVE(i,tsq+1,tc,wB,0), list);
							AddCaptureMove(board, BITMOVE(i,tsq+1,tc,wQ,0), list);
							AddCaptureMove(board, BITMOVE(i,tsq+1,tc,wR,0), list);
						}
					}

				}else if(1ULL<<i & board->WHITE_KNIGHT){
					genKnightMoves(board, list, i, f, r);
				}else if(1ULL<<i & board->WHITE_BISHOP){
					genBishopMoves(board, list, i, f, r);
				}else if(1ULL<<i & board->WHITE_ROOK){
					genRookMoves(board, list, i, f, r);
				}else if(1ULL<<i & board->WHITE_QUEEN){
					genRookMoves(board, list, i, f, r);
					genBishopMoves(board, list, i, f, r);
				}else if(1ULL<<i & board->WHITE_KING){
					genKingMoves(board, list, i, f, r);
					if(board->castlePerm & 1){
						if(!(board->BOTH & 4) && !(board->BOTH & 2)){
							if(!sqAttacked(3,0,board) && !sqAttacked(2,0,board)){
								AddQuietMove(board, BITMOVE(3, 1, EMPTY, EMPTY, MFLAGC), list);

							}
						}
					}
					if(board->castlePerm & 2){
						if(!(board->BOTH & 16) && !(board->BOTH & 32) && !(board->BOTH & 64)){
							if(!sqAttacked(3,0,board) && !sqAttacked(4,0,board)){
								AddQuietMove(board, BITMOVE(3, 5, EMPTY, EMPTY, MFLAGC), list);
							}
						}
					}
				}
			}else{
				if(1ULL<<i & board->BLACK_PAWN){
					tsq = i - 8;
					if(r != 1){
						if(!(board->BOTH & 1ULL << tsq)){
							AddQuietMove(board, BITMOVE(i,tsq,0,0,0), list);
							if(r == 6 && pieceSq(board, tsq-8) == EMPTY){
								AddQuietMove(board, BITMOVE(i,tsq-8,0,0,MFLAGPS), list);	
							}
						}
						tc = pieceSq(board, tsq-1);
						if(tc != EMPTY && tc < 7 && f != 0){
							AddCaptureMove(board, BITMOVE(i,tsq-1,tc,0,0), list);
						}

						tc = pieceSq(board, tsq+1);
						if(tc != EMPTY && tc < 7 && f != 7){
							AddCaptureMove(board, BITMOVE(i,tsq+1,tc,0,0), list);
						}
						if(board->enPas != 0){
							if(f != 0 && pieceSq(board, tsq+7) == wP && tsq-1==board->enPas){
								AddCaptureMove(board, BITMOVE(i,tsq-1,0,0,MFLAGEP), list);
							}else if(f != 7 && pieceSq(board, tsq+9) == wP && tsq+1==board->enPas){
								AddCaptureMove(board, BITMOVE(i,tsq+1,0,0,MFLAGEP), list);
							}
						}
					}else{
						if(!(board->BOTH & 1ULL << tsq)){
							AddQuietMove(board, BITMOVE(i,tsq,0,bN,0), list);
							AddQuietMove(board, BITMOVE(i,tsq,0,bB,0), list);
							AddQuietMove(board, BITMOVE(i,tsq,0,bQ,0), list);
							AddQuietMove(board, BITMOVE(i,tsq,0,bR,0), list);
						}
						tc = pieceSq(board, tsq-1);
						if(tc != EMPTY && tc < 7 && f != 0){
							AddCaptureMove(board, BITMOVE(i,tsq-1,tc,bN,0), list);
							AddCaptureMove(board, BITMOVE(i,tsq-1,tc,bB,0), list);
							AddCaptureMove(board, BITMOVE(i,tsq-1,tc,bQ,0), list);
							AddCaptureMove(board, BITMOVE(i,tsq-1,tc,bR,0), list);
						}
						tc = pieceSq(board, tsq+1);
						if(tc != EMPTY && tc < 7 && f != 7){
							AddCaptureMove(board, BITMOVE(i,tsq+1,tc,bN,0), list);
							AddCaptureMove(board, BITMOVE(i,tsq+1,tc,bB,0), list);
							AddCaptureMove(board, BITMOVE(i,tsq+1,tc,bQ,0), list);
							AddCaptureMove(board, BITMOVE(i,tsq+1,tc,bR,0), list);
						}
					}

				}else if(1ULL<<i & board->BLACK_KNIGHT){
					genKnightMoves(board, list, i, f, r);
				}else if(1ULL<<i & board->BLACK_BISHOP){
					genBishopMoves(board, list, i, f, r);
				}else if(1ULL<<i & board->BLACK_ROOK){
					genRookMoves(board, list, i, f, r);
				}else if(1ULL<<i & board->BLACK_QUEEN){
					genRookMoves(board, list, i, f, r);
					genBishopMoves(board, list, i, f, r);
				}else if(1ULL<<i & board->BLACK_KING){
					genKingMoves(board, list, i, f, r);
					if(board->castlePerm &  4) {
						if(!(board->BOTH & 1ULL << 58) && !(board->BOTH & 1ULL << 57)) {
							if(!sqAttacked(59,1,board) && !sqAttacked(58,1,board) ) {
								AddQuietMove(board, BITMOVE(59, 57, EMPTY, EMPTY, MFLAGC), list);
							}
						}
					}
					if(board->castlePerm &  8) {
						if(!(board->BOTH & 1ULL << 60) && !(board->BOTH & 1ULL << 61) && !(board->BOTH & 1ULL << 62)) {
							if(!sqAttacked(59,1,board) && !sqAttacked(60,1,board) ) {
								AddQuietMove(board, BITMOVE(59, 61, EMPTY, EMPTY, MFLAGC), list);
							}
						}
					}		
				}
			}
		}
	}
}

static void genKnightCapMoves(BOARD *board, MOVELIST *list, int i, int f, int r){
	int tsq,tc;
	tsq = i+6;
	if(f > 1 && r != 7){
		tc = pieceSq(board, tsq);
		if(tc != EMPTY){
			if(board->side == 1){
				if(tc > 6){
					AddCaptureMove(board, BITMOVE(i,tsq,tc,0,0), list);
				}
			}else{
				if(tc < 7){
					AddCaptureMove(board, BITMOVE(i,tsq,tc,0,0), list);
				}
			}
		}
	}
	tsq = i+10;
	if(f < 6 && r != 7){
		tc = pieceSq(board, tsq);
		if(tc != EMPTY){
			if(board->side == 1){
				if(tc > 6){
					AddCaptureMove(board, BITMOVE(i,tsq,tc,0,0), list);
				}
			}else{
				if(tc < 7){
					AddCaptureMove(board, BITMOVE(i,tsq,tc,0,0), list);
				}
			}
		}
	}
	tsq = i+15;
	if(f != 0 && r < 6){
		tc = pieceSq(board, tsq);
		if(tc != EMPTY){
			if(board->side == 1){
				if(tc > 6){
					AddCaptureMove(board, BITMOVE(i,tsq,tc,0,0), list);
				}
			}else{
				if(tc < 7){
					AddCaptureMove(board, BITMOVE(i,tsq,tc,0,0), list);
				}
			}
		}
	}										
	tsq = i+17;
	if(f != 7 && r < 6){
		tc = pieceSq(board, tsq);
		if(tc != EMPTY){
			if(board->side == 1){
				if(tc > 6){
					AddCaptureMove(board, BITMOVE(i,tsq,tc,0,0), list);
				}
			}else{
				if(tc < 7){
					AddCaptureMove(board, BITMOVE(i,tsq,tc,0,0), list);
				}
			}
		}
	}	
	tsq = i-6;
	if(f < 6 && r != 0){
		tc = pieceSq(board, tsq);
		if(tc != EMPTY){
			if(board->side == 1){
				if(tc > 6){
					AddCaptureMove(board, BITMOVE(i,tsq,tc,0,0), list);
				}
			}else{
				if(tc < 7){
					AddCaptureMove(board, BITMOVE(i,tsq,tc,0,0), list);
				}
			}
		}
	}
	tsq = i-10;
	if(f > 1 && r != 0){
		tc = pieceSq(board, tsq);
		if(tc != EMPTY){
			if(board->side == 1){
				if(tc > 6){
					AddCaptureMove(board, BITMOVE(i,tsq,tc,0,0), list);
				}
			}else{
				if(tc < 7){
					AddCaptureMove(board, BITMOVE(i,tsq,tc,0,0), list);
				}
			}
		}
	}
	tsq = i-15;
	if(f != 7 && r > 1){
		tc = pieceSq(board, tsq);
		if(tc != EMPTY){
			if(board->side == 1){
				if(tc > 6){
					AddCaptureMove(board, BITMOVE(i,tsq,tc,0,0), list);
				}
			}else{
				if(tc < 7){
					AddCaptureMove(board, BITMOVE(i,tsq,tc,0,0), list);
				}
			}
		}
	}		
	tsq = i-17;
	if(f != 0 && r > 1){
		tc = pieceSq(board, tsq);
		if(tc != EMPTY){
			if(board->side == 1){
				if(tc > 6){
					AddCaptureMove(board, BITMOVE(i,tsq,tc,0,0), list);
				}
			}else{
				if(tc < 7){
					AddCaptureMove(board, BITMOVE(i,tsq,tc,0,0), list);
				}
			}
		}
	}	
}

static void genRookCapMoves(BOARD *board, MOVELIST *list, int i, int f, int r){
	int tsq,mv=1,tc;
	while((7-r) >= mv){
		tsq = i+mv*8;
		tc = pieceSq(board, tsq);
		if(tc != EMPTY){
			if(board->side == 1){
				if(tc > 6){
					AddCaptureMove(board, BITMOVE(i,tsq,tc,0,0), list);
				}
			}else{
				if(tc < 7){
					AddCaptureMove(board, BITMOVE(i,tsq,tc,0,0), list);
				}
			}
			break;
		}
		mv++;
	}
	mv=1;
	while((7-f) >= mv){
		tsq = i+mv*1;
		tc = pieceSq(board, tsq);
		if(tc != EMPTY){
			if(board->side == 1){
				if(tc > 6){
					AddCaptureMove(board, BITMOVE(i,tsq,tc,0,0), list);
				}
			}else{
				if(tc < 7){
					AddCaptureMove(board, BITMOVE(i,tsq,tc,0,0), list);
				}
			}
			break;
		}
		mv++;
	}
	mv=1;
	while(r >= mv){
		tsq = i-mv*8;
		tc = pieceSq(board, tsq);
		if(tc != EMPTY){
			if(board->side == 1){
				if(tc > 6){
					AddCaptureMove(board, BITMOVE(i,tsq,tc,0,0), list);
				}
			}else{
				if(tc < 7){
					AddCaptureMove(board, BITMOVE(i,tsq,tc,0,0), list);
				}
			}
			break;
		}
		mv++;
	}
	mv=1;
	while(f >= mv){
		tsq = i-mv;
		tc = pieceSq(board, tsq);
		if(tc != EMPTY){
			if(board->side == 1){
				if(tc > 6){
					AddCaptureMove(board, BITMOVE(i,tsq,tc,0,0), list);
				}
			}else{
				if(tc < 7){
					AddCaptureMove(board, BITMOVE(i,tsq,tc,0,0), list);
				}
			}
			break;
		}
		mv++;
	}
}

static void genKingCapMoves(BOARD *board, MOVELIST *list, int i, int f, int r){
	int tsq, tc;
	tsq=i+8;
	tc = pieceSq(board, tsq);
	if(r != 7){
		if(tc != EMPTY){
			if(board->side == 1){
				if(tc > 6){
					AddCaptureMove(board, BITMOVE(i,tsq,tc,0,0), list);
				}
			}else{
				if(tc < 7){
					AddCaptureMove(board, BITMOVE(i,tsq,tc,0,0), list);
				}
			}
		}
	}
	tsq=i-8;
	tc = pieceSq(board, tsq);
	if(r != 0){
		if(tc != EMPTY){
			if(board->side == 1){
				if(tc > 6){
					AddCaptureMove(board, BITMOVE(i,tsq,tc,0,0), list);
				}
			}else{
				if(tc < 7){
					AddCaptureMove(board, BITMOVE(i,tsq,tc,0,0), list);
				}
			}
		}
	}
	tsq=i+1;
	tc = pieceSq(board, tsq);
	if(f != 7){
		if(tc != EMPTY){
			if(board->side == 1){
				if(tc > 6){
					AddCaptureMove(board, BITMOVE(i,tsq,tc,0,0), list);
				}
			}else{
				if(tc < 7){
					AddCaptureMove(board, BITMOVE(i,tsq,tc,0,0), list);
				}
			}
		}
	}
	tsq=i-1;
	tc = pieceSq(board, tsq);
	if(f != 0){
		if(tc != EMPTY){
			if(board->side == 1){
				if(tc > 6){
					AddCaptureMove(board, BITMOVE(i,tsq,tc,0,0), list);
				}
			}else{
				if(tc < 7){
					AddCaptureMove(board, BITMOVE(i,tsq,tc,0,0), list);
				}
			}
		}
	}
	tsq=i+7;
	tc = pieceSq(board, tsq);
	if(f != 0 && r != 7){
		if(tc != EMPTY){
			if(board->side == 1){
				if(tc > 6){
					AddCaptureMove(board, BITMOVE(i,tsq,tc,0,0), list);
				}
			}else{
				if(tc < 7){
					AddCaptureMove(board, BITMOVE(i,tsq,tc,0,0), list);
				}
			}
		}
	}
	tsq=i+9;
	tc = pieceSq(board, tsq);
	if(f != 7 && r != 7){
		if(tc != EMPTY){
			if(board->side == 1){
				if(tc > 6){
					AddCaptureMove(board, BITMOVE(i,tsq,tc,0,0), list);
				}
			}else{
				if(tc < 7){
					AddCaptureMove(board, BITMOVE(i,tsq,tc,0,0), list);
				}
			}
		}
	}
	tsq=i-7;
	tc = pieceSq(board, tsq);
	if(f != 7 && r != 0){
		if(tc != EMPTY){
			if(board->side == 1){
				if(tc > 6){
					AddCaptureMove(board, BITMOVE(i,tsq,tc,0,0), list);
				}
			}else{
				if(tc < 7){
					AddCaptureMove(board, BITMOVE(i,tsq,tc,0,0), list);
				}
			}
		}
	}
	tsq=i-9;
	tc = pieceSq(board, tsq);
	if(f != 0 && r != 0){
		if(tc != EMPTY){
			if(board->side == 1){
				if(tc > 6){
					AddCaptureMove(board, BITMOVE(i,tsq,tc,0,0), list);
				}
			}else{
				if(tc < 7){
					AddCaptureMove(board, BITMOVE(i,tsq,tc,0,0), list);
				}
			}
		}
	}
}

static void genBishopCapMoves(BOARD *board, MOVELIST *list, int i, int f, int r){
	int tsq,mv=1,tc;
	while((7-f) >= mv && (7-r) >= mv){
		tsq = i+mv*9;
		tc = pieceSq(board, tsq);
		if(tc != EMPTY){
			if(board->side == 1){
				if(tc > 6){
					AddCaptureMove(board, BITMOVE(i,tsq,tc,0,0), list);
				}
			}else{
				if(tc < 7){
					AddCaptureMove(board, BITMOVE(i,tsq,tc,0,0), list);
				}
			}
			break;
		}
		mv++;
	}
	mv=1;
	while(f >= mv && (7-r) >= mv){
		tsq = i+mv*7;
		tc = pieceSq(board, tsq);
		if(tc != EMPTY){
			if(board->side == 1){
				if(tc > 6){
					AddCaptureMove(board, BITMOVE(i,tsq,tc,0,0), list);
				}
			}else{
				if(tc < 7){
					AddCaptureMove(board, BITMOVE(i,tsq,tc,0,0), list);
				}
			}
			break;
		}
		mv++;
	}
	mv=1;
	while(f >= mv && r >= mv){
		tsq = i-mv*9;
		tc = pieceSq(board, tsq);
		if(tc != EMPTY){
			if(board->side == 1){
				if(tc > 6){
					AddCaptureMove(board, BITMOVE(i,tsq,tc,0,0), list);
				}
			}else{
				if(tc < 7){
					AddCaptureMove(board, BITMOVE(i,tsq,tc,0,0), list);
				}
			}
			break;
		}
		mv++;
	}
	mv=1;
	while((7-f) >= mv && r >= mv){
		tsq = i-mv*7;
		tc = pieceSq(board, tsq);
		if(tc != EMPTY){
			if(board->side == 1){
				if(tc > 6){
					AddCaptureMove(board, BITMOVE(i,tsq,tc,0,0), list);
				}
			}else{
				if(tc < 7){
					AddCaptureMove(board, BITMOVE(i,tsq,tc,0,0), list);
				}
			}
			break;
		}
		mv++;
	}
}

void genCapMoves(BOARD *board, MOVELIST *list){
	list->count = 0;
	int res,f,r,i,tsq,tc;
	for(i=0;i<64;i++){
		res = board->BOTH>>i & 1;
		f = i & 7;
		r = (i-f) / 8;
		if(res != 0){
			if(board->side == 1){
				if(1ULL<<i & board->WHITE_PAWN){
					tsq = i + 8;
					if(r != 6){

						tc = pieceSq(board, tsq-1);
						if(tc > 6 && f != 0){
							AddCaptureMove(board, BITMOVE(i,tsq-1,tc,0,0), list);
						}
						
						tc = pieceSq(board, tsq+1);
						if(tc > 6 && f != 7){
							AddCaptureMove(board, BITMOVE(i,tsq+1,tc,0,0), list);
							
						}
						
						if(board->enPas != 0){
							if(f != 0 && pieceSq(board, tsq-9) == bP && tsq-1==board->enPas){
									AddCaptureMove(board, BITMOVE(i,tsq-1,0,0,MFLAGEP), list);
							}else if(f != 7 && pieceSq(board, tsq-7) == bP && tsq+1==board->enPas){
									AddCaptureMove(board, BITMOVE(i,tsq+1,0,0,MFLAGEP), list);
							}
						}
						
					}else{
						tc = pieceSq(board, tsq-1);
						if(tc > 6 && f != 0){
							AddCaptureMove(board, BITMOVE(i,tsq-1,tc,wN,0), list);
							AddCaptureMove(board, BITMOVE(i,tsq-1,tc,wB,0), list);
							AddCaptureMove(board, BITMOVE(i,tsq-1,tc,wQ,0), list);
							AddCaptureMove(board, BITMOVE(i,tsq-1,tc,wR,0), list);
						}
						tc = pieceSq(board, tsq+1);
						if(tc > 6 && f != 7){
							AddCaptureMove(board, BITMOVE(i,tsq+1,tc,wN,0), list);
							AddCaptureMove(board, BITMOVE(i,tsq+1,tc,wB,0), list);
							AddCaptureMove(board, BITMOVE(i,tsq+1,tc,wQ,0), list);
							AddCaptureMove(board, BITMOVE(i,tsq+1,tc,wR,0), list);
						}
					}

				}else if(1ULL<<i & board->WHITE_KNIGHT){
					genKnightCapMoves(board, list, i, f, r);
				}else if(1ULL<<i & board->WHITE_BISHOP){
					genBishopCapMoves(board, list, i, f, r);
				}else if(1ULL<<i & board->WHITE_ROOK){
					genRookCapMoves(board, list, i, f, r);
				}else if(1ULL<<i & board->WHITE_QUEEN){
					genRookCapMoves(board, list, i, f, r);
					genBishopCapMoves(board, list, i, f, r);
				}else if(1ULL<<i & board->WHITE_KING){
					genKingCapMoves(board, list, i, f, r);
				}
			}else{
				if(1ULL<<i & board->BLACK_PAWN){
					tsq = i - 8;
					if(r != 1){
						tc = pieceSq(board, tsq-1);
						if(tc != EMPTY && tc < 7 && f != 0){
							AddCaptureMove(board, BITMOVE(i,tsq-1,tc,0,0), list);
						}

						tc = pieceSq(board, tsq+1);
						if(tc != EMPTY && tc < 7 && f != 7){
							AddCaptureMove(board, BITMOVE(i,tsq+1,tc,0,0), list);
						}
						if(board->enPas != 0){
							if(f != 0 && pieceSq(board, tsq+7) == wP && tsq-1==board->enPas){
								AddCaptureMove(board, BITMOVE(i,tsq-1,0,0,MFLAGEP), list);
							}else if(f != 7 && pieceSq(board, tsq+9) == wP && tsq+1==board->enPas){
								AddCaptureMove(board, BITMOVE(i,tsq+1,0,0,MFLAGEP), list);
							}
						}
					}else{
						tc = pieceSq(board, tsq-1);
						if(tc != EMPTY && tc < 7 && f != 0){
							AddCaptureMove(board, BITMOVE(i,tsq-1,tc,bN,0), list);
							AddCaptureMove(board, BITMOVE(i,tsq-1,tc,bB,0), list);
							AddCaptureMove(board, BITMOVE(i,tsq-1,tc,bQ,0), list);
							AddCaptureMove(board, BITMOVE(i,tsq-1,tc,bR,0), list);
						}
						tc = pieceSq(board, tsq+1);
						if(tc != EMPTY && tc < 7 && f != 7){
							AddCaptureMove(board, BITMOVE(i,tsq+1,tc,bN,0), list);
							AddCaptureMove(board, BITMOVE(i,tsq+1,tc,bB,0), list);
							AddCaptureMove(board, BITMOVE(i,tsq+1,tc,bQ,0), list);
							AddCaptureMove(board, BITMOVE(i,tsq+1,tc,bR,0), list);
						}
					}

				}else if(1ULL<<i & board->BLACK_KNIGHT){
					genKnightCapMoves(board, list, i, f, r);
				}else if(1ULL<<i & board->BLACK_BISHOP){
					genBishopCapMoves(board, list, i, f, r);
				}else if(1ULL<<i & board->BLACK_ROOK){
					genRookCapMoves(board, list, i, f, r);
				}else if(1ULL<<i & board->BLACK_QUEEN){
					genRookCapMoves(board, list, i, f, r);
					genBishopCapMoves(board, list, i, f, r);
				}else if(1ULL<<i & board->BLACK_KING){
					genKingCapMoves(board, list, i, f, r);	
				}
			}
		}
	}
}

void printMoveList(const MOVELIST *list) {
	int i = 0;
	int score = 0;
	int move = 0;
	printf("MoveList:\n");
	
	for(i = 0; i < list->count; ++i) {
	
		move = list->moves[i].move;
		score = list->moves[i].score;
		
		printf("Move:%d > %s (score:%d)\n",i+1,printMove(move),score);
	}
	printf("MoveList Total %d Moves:\n\n",list->count);
}

char *printMove(const int move) {

	static char MvStr[6];
	
	int fr = FROMSQ(move);
	int to = TOSQ(move);
	int ff = fr & 7;
	int rf = (fr-ff) / 8;
	int ft = to & 7;
	int rt = (to-ft) / 8;;
	
	int promoted = PROMOTED(move);
	
	if(promoted) {
		char pchar = 'q';
		if(promoted == 2 || promoted == 8) {
			pchar = 'n';
		} else if(promoted == 4 || promoted == 10)  {
			pchar = 'r';
		} else if(promoted == 3 || promoted == 9)  {
			pchar = 'b';
		}
		sprintf(MvStr, "%c%c%c%c%c", ('a'+(7-ff)), ('1'+rf), ('a'+(7-ft)), ('1'+rt), pchar);
	} else {
		sprintf(MvStr, "%c%c%c%c", ('a'+(7-ff)), ('1'+rf), ('a'+(7-ft)), ('1'+rt));
	}
	
	return MvStr;
}

//X-Ray mod?
int sqAttacked(const char sq, const char side, const BOARD *board) {

	char tsq,mv=1,f=f(sq),r=r(sq);
	
	if(side == 1) {
		if((1ULL<<(sq-9) & board->WHITE_PAWN && f != 0) || (1ULL<<(sq-7) & board->WHITE_PAWN && f != 7)) {
			return 1;
		}
	} else {
		if((1ULL<<(sq+9) & board->BLACK_PAWN && f != 7) || (1ULL<<(sq+7) & board->BLACK_PAWN && f != 0)) {
			return 1;
		}	
	}
	
	// knights
	tsq = sq+6;
	if(f > 1 && r != 7){
		if(side == 1){
			if(1ULL<<tsq & board->WHITE_KNIGHT){
				return 1;
			}
		}else{
			if(1ULL<<tsq & board->BLACK_KNIGHT){
				return 1;
			}
		}
	}
	tsq = sq+10;
	if(f < 6 && r != 7){
		if(side == 1){
			if(1ULL<<tsq & board->WHITE_KNIGHT){
				return 1;
			}
		}else{
			if(1ULL<<tsq & board->BLACK_KNIGHT){
				return 1;
			}
		}
	}
	tsq = sq+15;
	if(f != 0 && r < 6){
		if(side == 1){
			if(1ULL<<tsq & board->WHITE_KNIGHT){
				return 1;
			}
		}else{
			if(1ULL<<tsq & board->BLACK_KNIGHT){
				return 1;
			}
		}
	}										
	tsq = sq+17;
	if(f != 7 && r < 6){
		if(side == 1){
			if(1ULL<<tsq & board->WHITE_KNIGHT){
				return 1;
			}
		}else{
			if(1ULL<<tsq & board->BLACK_KNIGHT){
				return 1;
			}
		}
	}	
	tsq = sq-6;
	if(f < 6 && r != 0){
		if(side == 1){
			if(1ULL<<tsq & board->WHITE_KNIGHT){
				return 1;
			}
		}else{
			if(1ULL<<tsq & board->BLACK_KNIGHT){
				return 1;
			}
		}
	}
	tsq = sq-10;
	if(f > 1 && r != 0){
		if(side == 1){
			if(1ULL<<tsq & board->WHITE_KNIGHT){
				return 1;
			}
		}else{
			if(1ULL<<tsq & board->BLACK_KNIGHT){
				return 1;
			}
		}
	}
	tsq = sq-15;
	if(f != 7 && r > 1){
		if(side == 1){
			if(1ULL<<tsq & board->WHITE_KNIGHT){
				return 1;
			}
		}else{
			if(1ULL<<tsq & board->BLACK_KNIGHT){
				return 1;
			}
		}
	}		
	tsq = sq-17;
	if(f != 0 && r > 1){
		if(side == 1){
			if(1ULL<<tsq & board->WHITE_KNIGHT){
				return 1;
			}
		}else{
			if(1ULL<<tsq & board->BLACK_KNIGHT){
				return 1;
			}
		}
	}
	//Bishops & Queens
	mv=1;
	while((7-f) >= mv && (7-r) >= mv){
		tsq = sq+mv*9;
		if(1ULL<<tsq & board->BOTH){
			if(side == 1){
				if(1ULL<<tsq & board->WHITE_BISHOP || 1ULL<<tsq & board->WHITE_QUEEN){
					return 1;
				}
			}else{
				if(1ULL<<tsq & board->BLACK_BISHOP || 1ULL<<tsq & board->BLACK_QUEEN){
					return 1;
				}
			}
			break;
		}
		mv++;
	}
	mv=1;
	while(f >= mv && (7-r) >= mv){
		tsq = sq+mv*7;
		if(1ULL<<tsq & board->BOTH){
			if(side == 1){
				if(1ULL<<tsq & board->WHITE_BISHOP || 1ULL<<tsq & board->WHITE_QUEEN){
					return 1;
				}
			}else{
				if(1ULL<<tsq & board->BLACK_BISHOP || 1ULL<<tsq & board->BLACK_QUEEN){
					return 1;
				}
			}
			break;
		}
		mv++;
	}
	mv=1;
	while(f >= mv && r >= mv){
		tsq = sq-mv*9;
		if(1ULL<<tsq & board->BOTH){
			if(side == 1){
				if(1ULL<<tsq & board->WHITE_BISHOP || 1ULL<<tsq & board->WHITE_QUEEN){
					return 1;
				}
			}else{
				if(1ULL<<tsq & board->BLACK_BISHOP || 1ULL<<tsq & board->BLACK_QUEEN){
					return 1;
				}
			}
			break;
		}
		mv++;
	}
	mv=1;
	while((7-f) >= mv && r >= mv){
		tsq = sq-mv*7;
		if(1ULL<<tsq & board->BOTH){
			if(side == 1){
				if(1ULL<<tsq & board->WHITE_BISHOP || 1ULL<<tsq & board->WHITE_QUEEN){
					return 1;
				}
			}else{
				if(1ULL<<tsq & board->BLACK_BISHOP || 1ULL<<tsq & board->BLACK_QUEEN){
					return 1;
				}
			}
			break;
		}
		mv++;
	}
	//Rooks & Queens
	mv=1;
	while((7-r) >= mv){
		tsq = sq+mv*8;
		if(sq == 38){
			//printf("TSQ %d, F: %d, R: %d, MV: %d\n", tsq, f, r, mv);
		}
		if(1ULL<<tsq & board->BOTH){
			//printf("TRUE!\n");
			if(side == 1){
				if(1ULL<<tsq & board->WHITE_ROOK || 1ULL<<tsq & board->WHITE_QUEEN){
					return 1;
				}
			}else{
				if(1ULL<<tsq & board->BLACK_ROOK || 1ULL<<tsq & board->BLACK_QUEEN){
					return 1;
				}
			}
			break;
		}
		mv++;
	}
	mv=1;
	while((7-f) >= mv){
		tsq = sq+mv;
		if(1ULL<<tsq & board->BOTH){
			if(side == 1){
				if(1ULL<<tsq & board->WHITE_ROOK || 1ULL<<tsq & board->WHITE_QUEEN){
					return 1;
				}
			}else{
				if(1ULL<<tsq & board->BLACK_ROOK || 1ULL<<tsq & board->BLACK_QUEEN){
					return 1;
				}
			}
			break;
		}
		mv++;
	}
	mv=1;
	while(r >= mv){
		tsq = sq-mv*8;
		if(1ULL<<tsq & board->BOTH){
			//printf("TRUE!\n");
			if(side == 1){
				if(1ULL<<tsq & board->WHITE_ROOK || 1ULL<<tsq & board->WHITE_QUEEN){
					return 1;
				}
			}else{
				if(1ULL<<tsq & board->BLACK_ROOK || 1ULL<<tsq & board->BLACK_QUEEN){
					return 1;
				}
			}
			break;
		}
		mv++;
	}
	mv=1;
	while(f >= mv){
		tsq = sq-mv;
		if(1ULL<<tsq & board->BOTH){
			if(side == 1){
				if(1ULL<<tsq & board->WHITE_ROOK || 1ULL<<tsq & board->WHITE_QUEEN){
					return 1;
				}
			}else{
				if(1ULL<<tsq & board->BLACK_ROOK || 1ULL<<tsq & board->BLACK_QUEEN){
					return 1;
				}
			}
			break;
		}
		mv++;
	}
	//King
	tsq=sq+8;
	if(r != 7){
		if(side == 1){
			if(1ULL<<tsq & board->WHITE_KING){
				return 1;
			}
		}else{
			if(1ULL<<tsq & board->BLACK_KING){
				return 1;
			}
		}
	}
	tsq=sq-8;
	if(r != 0){
		if(side == 1){
			if(1ULL<<tsq & board->WHITE_KING){
				return 1;
			}
		}else{
			if(1ULL<<tsq & board->BLACK_KING){
				return 1;
			}
		}
	}
	tsq=sq+1;
	if(f != 7){
		if(side == 1){
			if(1ULL<<tsq & board->WHITE_KING){
				return 1;
			}
		}else{
			if(1ULL<<tsq & board->BLACK_KING){
				return 1;
			}
		}
	}
	tsq=sq-1;
	if(f != 0){
		if(side == 1){
			if(1ULL<<tsq & board->WHITE_KING){
				return 1;
			}
		}else{
			if(1ULL<<tsq & board->BLACK_KING){
				return 1;
			}
		}
	}
	tsq=sq+7;
	if(f != 0 && r != 7){
		if(side == 1){
			if(1ULL<<tsq & board->WHITE_KING){
				return 1;
			}
		}else{
			if(1ULL<<tsq & board->BLACK_KING){
				return 1;
			}
		}
	}
	tsq=sq+9;
	if(f != 7 && r != 7){
		if(side == 1){
			if(1ULL<<tsq & board->WHITE_KING){
				return 1;
			}
		}else{
			if(1ULL<<tsq & board->BLACK_KING){
				return 1;
			}
		}
	}
	tsq=sq-7;
	if(f != 7 && r != 0){
		if(side == 1){
			if(1ULL<<tsq & board->WHITE_KING){
				return 1;
			}
		}else{
			if(1ULL<<tsq & board->BLACK_KING){
				return 1;
			}
		}
	}
	tsq=sq-9;
	if(f != 0 && r != 0){
		if(side == 1){
			if(1ULL<<tsq & board->WHITE_KING){
				return 1;
			}
		}else{
			if(1ULL<<tsq & board->BLACK_KING){
				return 1;
			}
		}
	}
	return 0;
}

int moveExists(BOARD *board, const int move){
	MOVELIST list[1];
	genMoves(board,list);
	int nMove = 0;
	for(nMove=0;nMove<list->count;++nMove){
		if(!makeMove(board,list->moves[nMove].move)){
			continue;
		}
		takeMove(board);
		if(list->moves[nMove].move == move){
			return 1;
		}
	}
	return 0;
}

