#include "inc.h"
#include "stdio.h"

int setPos(char *fen, BOARD *board){
	resetBoard(board);
	char i=63,c,j;
	while((i >= 0) && *fen){
		c=0;
		switch (*fen) {
            case 'P': SETBIT(board->WHITE_PAWN,i);SETBIT(board->BOTH,i);SETBIT(board->WHITE,i);i--; break;
            case 'R': SETBIT(board->WHITE_ROOK,i);SETBIT(board->BOTH,i);SETBIT(board->WHITE,i);i--; break;
            case 'N': SETBIT(board->WHITE_KNIGHT,i);SETBIT(board->BOTH,i);SETBIT(board->WHITE,i);i--; break;
            case 'B': SETBIT(board->WHITE_BISHOP,i);SETBIT(board->BOTH,i);SETBIT(board->WHITE,i);i--; break;
            case 'K': SETBIT(board->WHITE_KING,i);SETBIT(board->BOTH,i);SETBIT(board->WHITE,i);board->kings[1]=i;i--;break;
            case 'Q': SETBIT(board->WHITE_QUEEN,i);SETBIT(board->BOTH,i);SETBIT(board->WHITE,i);i--; break;
            case 'p': SETBIT(board->BLACK_PAWN,i);SETBIT(board->BOTH,i);SETBIT(board->BLACK,i);i--; break;
            case 'r': SETBIT(board->BLACK_ROOK,i);SETBIT(board->BOTH,i);SETBIT(board->BLACK,i);i--; break;
            case 'n': SETBIT(board->BLACK_KNIGHT,i);SETBIT(board->BOTH,i);SETBIT(board->BLACK,i);i--; break;
            case 'b': SETBIT(board->BLACK_BISHOP,i);SETBIT(board->BOTH,i);SETBIT(board->BLACK,i);i--; break;
            case 'k': SETBIT(board->BLACK_KING,i);SETBIT(board->BOTH,i);SETBIT(board->BLACK,i);board->kings[0]=i;i--;break;
            case 'q': SETBIT(board->BLACK_QUEEN,i);SETBIT(board->BOTH,i);SETBIT(board->BLACK,i);i--; break;

            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
                c = *fen - '0';
                i=i-c;
                break;

            case '/':
            case ' ':
                fen++;
                continue;              

            default:
                printf("Incorrect FEN\n");
                return -1;
        }
        fen++;		
	}
	fen++;
	board->side = (*fen == 'w') ? 1 : 0;
	fen += 2;
	
	for (j = 0; j < 4; j++) {
        if (*fen == ' ') {
            break;
        }		
		switch(*fen) {
			case 'K': board->castlePerm |= 1; break;
			case 'Q': board->castlePerm |= 2; break;
			case 'k': board->castlePerm |= 4; break;
			case 'q': board->castlePerm |= 8; break;
			default: break;
        }
		fen++;
	}
	fen++;
	
	if (*fen != '-') {        
		char f = fen[0] - 'a';
		char r = fen[1] - '1';		
		board->enPas = SQ2BIT(f,r);		
    }

	board->POS_KEY = genPosKey(board);

	return 0;
}

void resetBoard(BOARD *board){
	board->WHITE=0;
	board->BLACK=0;
	board->BOTH=0;
	board->BLACK_PAWN=0;
	board->WHITE_PAWN=0;
	board->BLACK_KNIGHT=0;
	board->WHITE_KNIGHT=0;
	board->BLACK_BISHOP=0;
	board->WHITE_BISHOP=0;
	board->BLACK_ROOK=0;
	board->WHITE_ROOK=0;
	board->BLACK_QUEEN=0;
	board->WHITE_QUEEN=0;
	board->BLACK_KING=0;
	board->WHITE_KING=0;
	board->POS_KEY=0;
	board->enPas=0;
	board->castlePerm=0;
	board->side=0;
	board->ply=0;
	board->hisPly=0;
}

U64 genPosKey(const BOARD *board) {

	U64 finalKey = 0;

	finalKey ^= board->BLACK_PAWN;
	finalKey ^= board->WHITE_PAWN;
	finalKey ^= board->BLACK_KNIGHT;
	finalKey ^= board->WHITE_KNIGHT;
	finalKey ^= board->BLACK_BISHOP;
	finalKey ^= board->WHITE_BISHOP;
	finalKey ^= board->BLACK_ROOK;
	finalKey ^= board->WHITE_ROOK;
	finalKey ^= board->BLACK_QUEEN;
	finalKey ^= board->WHITE_QUEEN;
	finalKey ^= board->BLACK_KING;
	finalKey ^= board->WHITE_KING;
	finalKey ^= board->enPas;
	if(board->side == 1) {
		finalKey ^= SideKey;
	}
	finalKey ^= CastleKeys[board->castlePerm];
	
	return finalKey;
}
