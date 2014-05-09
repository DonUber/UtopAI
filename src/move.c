#include "inc.h"
#include "stdio.h"

#define HASH_CA (board->POS_KEY ^= (CastleKeys[(board->castlePerm)]))
#define HASH_SIDE (board->POS_KEY ^= (SideKey))
#define HASH_EP (board->POS_KEY ^= (board->enPas))

const int CastlePerm[64] = {
    14, 15, 15, 12, 15, 15, 15, 13,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    11, 15, 15, 3,  15, 15, 15, 7
};

static void clearPiece(const int sq, BOARD *board){
	int tc;
	tc = pieceSq(board,sq);
	switch(tc){
		case 1: board->POS_KEY ^= board->WHITE_PAWN;CLRBIT(board->WHITE_PAWN, sq);board->POS_KEY ^= board->WHITE_PAWN;break;
		case 2: board->POS_KEY ^= board->WHITE_KNIGHT;CLRBIT(board->WHITE_KNIGHT, sq);board->POS_KEY ^= board->WHITE_KNIGHT;break;
		case 3: board->POS_KEY ^= board->WHITE_BISHOP;CLRBIT(board->WHITE_BISHOP, sq);board->POS_KEY ^= board->WHITE_BISHOP;break; 
		case 4: board->POS_KEY ^= board->WHITE_ROOK;CLRBIT(board->WHITE_ROOK, sq);board->POS_KEY ^= board->WHITE_ROOK;break; 
		case 5: board->POS_KEY ^= board->WHITE_QUEEN;CLRBIT(board->WHITE_QUEEN, sq);board->POS_KEY ^= board->WHITE_QUEEN;break; 
		case 6: board->POS_KEY ^= board->WHITE_KING;CLRBIT(board->WHITE_KING, sq);board->POS_KEY ^= board->WHITE_KING;break; 
		case 7: board->POS_KEY ^= board->BLACK_PAWN;CLRBIT(board->BLACK_PAWN, sq);board->POS_KEY ^= board->BLACK_PAWN;break; 
		case 8: board->POS_KEY ^= board->BLACK_KNIGHT;CLRBIT(board->BLACK_KNIGHT, sq);board->POS_KEY ^= board->BLACK_KNIGHT;break; 
		case 9: board->POS_KEY ^= board->BLACK_BISHOP;CLRBIT(board->BLACK_BISHOP, sq);board->POS_KEY ^= board->BLACK_BISHOP;break; 
		case 10: board->POS_KEY ^= board->BLACK_ROOK;CLRBIT(board->BLACK_ROOK, sq);board->POS_KEY ^= board->BLACK_ROOK;break; 
		case 11: board->POS_KEY ^= board->BLACK_QUEEN;CLRBIT(board->BLACK_QUEEN, sq);board->POS_KEY ^= board->BLACK_QUEEN;break; 
		case 12: board->POS_KEY ^= board->BLACK_KING;CLRBIT(board->BLACK_KING, sq);board->POS_KEY ^= board->BLACK_KING;break; 
	}
	if(tc < 7){
		CLRBIT(board->WHITE, sq);
	}else{
		CLRBIT(board->BLACK, sq);		
	}
	CLRBIT(board->BOTH, sq);
}

static void addPiece(const int sq, BOARD *board, const int tc){
	switch(tc){
		case 1: board->POS_KEY ^= board->WHITE_PAWN;SETBIT(board->WHITE_PAWN, sq);board->POS_KEY ^= board->WHITE_PAWN;break;
		case 2: board->POS_KEY ^= board->WHITE_KNIGHT;SETBIT(board->WHITE_KNIGHT, sq);board->POS_KEY ^= board->WHITE_KNIGHT;break;
		case 3: board->POS_KEY ^= board->WHITE_BISHOP;SETBIT(board->WHITE_BISHOP, sq);board->POS_KEY ^= board->WHITE_BISHOP;break; 
		case 4: board->POS_KEY ^= board->WHITE_ROOK;SETBIT(board->WHITE_ROOK, sq);board->POS_KEY ^= board->WHITE_ROOK;break; 
		case 5: board->POS_KEY ^= board->WHITE_QUEEN;SETBIT(board->WHITE_QUEEN, sq);board->POS_KEY ^= board->WHITE_QUEEN;break; 
		case 6: board->POS_KEY ^= board->WHITE_KING;SETBIT(board->WHITE_KING, sq);board->POS_KEY ^= board->WHITE_KING;break; 
		case 7: board->POS_KEY ^= board->BLACK_PAWN;SETBIT(board->BLACK_PAWN, sq);board->POS_KEY ^= board->BLACK_PAWN;break; 
		case 8: board->POS_KEY ^= board->BLACK_KNIGHT;SETBIT(board->BLACK_KNIGHT, sq);board->POS_KEY ^= board->BLACK_KNIGHT;break; 
		case 9: board->POS_KEY ^= board->BLACK_BISHOP;SETBIT(board->BLACK_BISHOP, sq);board->POS_KEY ^= board->BLACK_BISHOP;break; 
		case 10: board->POS_KEY ^= board->BLACK_ROOK;SETBIT(board->BLACK_ROOK, sq);board->POS_KEY ^= board->BLACK_ROOK;break; 
		case 11: board->POS_KEY ^= board->BLACK_QUEEN;SETBIT(board->BLACK_QUEEN, sq);board->POS_KEY ^= board->BLACK_QUEEN;break; 
		case 12: board->POS_KEY ^= board->BLACK_KING;SETBIT(board->BLACK_KING, sq);board->POS_KEY ^= board->BLACK_KING;break; 
	}
	if(tc < 7){
		SETBIT(board->WHITE, sq); 
	}else{
		SETBIT(board->BLACK, sq); 		
	}
	SETBIT(board->BOTH, sq);
}

static void movePiece(const int from, const int to, BOARD *board){
	int tc;
	tc = pieceSq(board,from);
	switch(tc){
		case 1: board->POS_KEY ^= board->WHITE_PAWN;CLRBIT(board->WHITE_PAWN, from); SETBIT(board->WHITE_PAWN, to);board->POS_KEY ^= board->WHITE_PAWN;break;
		case 2: board->POS_KEY ^= board->WHITE_KNIGHT;CLRBIT(board->WHITE_KNIGHT, from); SETBIT(board->WHITE_KNIGHT, to);board->POS_KEY ^= board->WHITE_KNIGHT;break;
		case 3: board->POS_KEY ^= board->WHITE_BISHOP;CLRBIT(board->WHITE_BISHOP, from); SETBIT(board->WHITE_BISHOP, to);board->POS_KEY ^= board->WHITE_BISHOP;break; 
		case 4: board->POS_KEY ^= board->WHITE_ROOK;CLRBIT(board->WHITE_ROOK, from); SETBIT(board->WHITE_ROOK, to);board->POS_KEY ^= board->WHITE_ROOK;break; 
		case 5: board->POS_KEY ^= board->WHITE_QUEEN;CLRBIT(board->WHITE_QUEEN, from); SETBIT(board->WHITE_QUEEN, to);board->POS_KEY ^= board->WHITE_QUEEN;break; 
		case 6: board->POS_KEY ^= board->WHITE_KING;CLRBIT(board->WHITE_KING, from); SETBIT(board->WHITE_KING, to);board->POS_KEY ^= board->WHITE_KING;break; 
		case 7: board->POS_KEY ^= board->BLACK_PAWN;CLRBIT(board->BLACK_PAWN, from); SETBIT(board->BLACK_PAWN, to);board->POS_KEY ^= board->BLACK_PAWN;break; 
		case 8: board->POS_KEY ^= board->BLACK_KNIGHT;CLRBIT(board->BLACK_KNIGHT, from); SETBIT(board->BLACK_KNIGHT, to);board->POS_KEY ^= board->BLACK_KNIGHT;break; 
		case 9: board->POS_KEY ^= board->BLACK_BISHOP;CLRBIT(board->BLACK_BISHOP, from); SETBIT(board->BLACK_BISHOP, to);board->POS_KEY ^= board->BLACK_BISHOP;break; 
		case 10: board->POS_KEY ^= board->BLACK_ROOK;CLRBIT(board->BLACK_ROOK, from); SETBIT(board->BLACK_ROOK, to);board->POS_KEY ^= board->BLACK_ROOK;break; 
		case 11: board->POS_KEY ^= board->BLACK_QUEEN;CLRBIT(board->BLACK_QUEEN, from); SETBIT(board->BLACK_QUEEN, to);board->POS_KEY ^= board->BLACK_QUEEN;break; 
		case 12: board->POS_KEY ^= board->BLACK_KING;CLRBIT(board->BLACK_KING, from); SETBIT(board->BLACK_KING, to);board->POS_KEY ^= board->BLACK_KING;break; 
	}
	if(tc < 7){
		CLRBIT(board->WHITE, from);
		SETBIT(board->WHITE, to); 
	}else{
		CLRBIT(board->BLACK, from);
		SETBIT(board->BLACK, to); 		
	}
	CLRBIT(board->BOTH, from);
	SETBIT(board->BOTH, to);
}

int makeMove(BOARD *board, const int move){
	char tmp;
	char from = FROMSQ(move);
	char to = TOSQ(move);
	char side = board->side;

	board->history[board->hisPly].POS_KEY = board->POS_KEY;
	if(move & MFLAGEP){
		if(side == 1){
			clearPiece(to-8,board);
		}else{
			clearPiece(to+8,board);	
		}
	}else if(move & MFLAGC){
		switch(to){
			case 5:
				movePiece(7,4,board);
			break;
			case 61:
				movePiece(63,60,board);
			break;
			case 1:
				movePiece(0,2,board);
			break;
			case 57:
				movePiece(56,58,board);
			break;
			default: break;
		}
	}
	if(board->enPas != 0) HASH_EP;
	HASH_CA;
	board->history[board->hisPly].move = move;
	board->history[board->hisPly].fifty = board->fifty;
	board->history[board->hisPly].enPas = board->enPas;
	board->history[board->hisPly].castlePerm = board->castlePerm;
	board->castlePerm &= CastlePerm[from];
	board->castlePerm &= CastlePerm[to];
	board->enPas = 0;
	HASH_CA;
	int cap = CAPTURED(move);
	board->fifty++;
	if(cap != EMPTY){
		clearPiece(to, board);
		board->fifty = 0;
	}
	board->ply++;
	board->hisPly++;

	if(move & MFLAGPS){
		board->fifty = 0;
		if(side == 0){
			board->enPas=from-8;
		}else{
			board->enPas=from+8;
		}
		HASH_EP;
	}

	movePiece(from, to, board);
	char promo = PROMOTED(move);
	if(promo != EMPTY){
		//printf("FROM %d TO %d\n", from, to);
		clearPiece(to,board);
		addPiece(to,board,promo);
	}else if(1ULL << to & board->WHITE_KING){
		board->kings[1] = to;
	}else if(1ULL << to & board->BLACK_KING){
		board->kings[0] = to;
	}
	board->side ^= 1;
	HASH_SIDE;
	if(side == 0){
		if(sqAttacked(board->kings[0],board->side,board)){
			takeMove(board);
			return 0;
		}
	}else{
		if(sqAttacked(board->kings[1],board->side,board)){
			takeMove(board);
			return 0;
		}	
	}
	return 1;
}

void takeMove(BOARD *board) {
	
	board->hisPly--;
    board->ply--;
	
    int move = board->history[board->hisPly].move;
    int from = FROMSQ(move);
    int to = TOSQ(move);	
	
	if(board->enPas != 0) HASH_EP;
    HASH_CA;

    board->castlePerm = board->history[board->hisPly].castlePerm;
    board->fifty = board->history[board->hisPly].fifty;
    board->enPas = board->history[board->hisPly].enPas;

    if(board->enPas != 0) HASH_EP;
    HASH_CA;

    board->side ^= 1;
    HASH_SIDE;
	
	if(MFLAGEP & move) {
        if(board->side == 1) {
            addPiece(to-8, board, bP);
        } else {
            addPiece(to+8, board, wP);
        }
    } else if(MFLAGC & move) {
        switch(to) {
            case 5: movePiece(4, 7, board); break;
            case 61: movePiece(60, 63, board); break;
            case 1: movePiece(2, 0, board); break;
            case 57: movePiece(58, 56, board); break;
            default: break;
        }
    }
	
	movePiece(to, from, board);
	
	if(1ULL << from & board->WHITE_KING){
		board->kings[1] = from;
	}else if(1ULL << from & board->BLACK_KING){
		board->kings[0] = from;
	}
	
	int captured = CAPTURED(move);
    if(captured != EMPTY) {
        addPiece(to, board, captured);
    }
	
	if(PROMOTED(move) != EMPTY)   {
        clearPiece(from, board);
        addPiece(from, board, (PROMOTED(move) < 7 ? wP : bP));
    }
}
