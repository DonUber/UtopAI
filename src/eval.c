#include "stdio.h"
#include "inc.h"

//Position Eval Bitboards

/*BPAWN +5:
00000000
00000000
10011001
00000000
11100111
00000000
00000000
00000000
*/

/*BPAWN +10:
00000000
11000011
00000000
00100100
00011000
11100111
11111111
00000000
*/

/*BPAWN +20:
00000000
00000000
00000000
00011000
00000000
00011000
11111111
00000000
*/

/*WPAWN +5:
00000000
00000000
00000000
11100111
00000000
10011001
00000000
00000000
*/

/*WPAWN +10:
00000000
11111111
11100111
00011000
00100100
00000000
11000011
00000000
*/

/*WPAWN +20:
00000000
11111111
00011000
00000000
00011000
00000000
00000000
00000000
*/

/*BKNIGHT +5:
00000000
00011000
00000000
00000000
10000001
10000001
00100100
00000000
*/

/*BKNIGHT +10:
00000000
00000000
00111100
01100110
01100110
01100110
00011000
00000000
*/

/*BKNIGHT +20:
00000000
00000000
00000000
00011000
00011000
00011000
00000000
00000000
*/

/*WKNIGHT +5:
00000000
00100100
10000001
10000001
00000000
00000000
00011000
00000000
*/

/*WKNIGHT +10:
00000000
00011000
01100110
01100110
01100110
00111100
00000000
00000000
*/

/*WKNIGHT +20:
00000000
00000000
00011000
00011000
00011000
00000000
00000000
00000000
*/

/* BWBISHOP + 10
00000000
00011000
00100100
01000010
01000010
00100100
00011000
00000000
*/

/* BWBISHOP + 20
00000000
00000000
00011000
00111100
00111100
00011000
00000000
00000000
*/

/*BROOK +10
00100100
00100100
00100100
00100100
00100100
00100100
11111111
00100100
*/

/*BROOK +15
00011000
00011000
00011000
00011000
00011000
00011000
11111111
00011000
*/

/*WROOK +10
00100100
11111111
00100100
00100100
00100100
00100100
00100100
00100100
*/

/*WROOK +15
00011000
11111111
00011000
00011000
00011000
00011000
00011000
00011000
*/

/*WKINGK
00000000
00000000
00000000
00000000
00000000
00000000
00000111
00000000
*/

/*WKINGQ
00000000
00000000
00000000
00000000
00000000
00000000
11100000
00000000
*/

/*BKINGK
00000000
00000111
00000000
00000000
00000000
00000000
00000000
00000000
*/

/*BKINGQ
00000000
11100000
00000000
00000000
00000000
00000000
00000000
00000000
*/

/*WKINGKA
00000000
00000000
00000000
00000000
00000000
00000010
00000101
00000000
*/

/*WKINGQA
00000000
00000000
00000000
00000000
00000000
01000000
10100000
00000000
*/

/*BKINGKA
00000000
00000101
00000010
00000000
00000000
00000000
00000000
00000000
*/

/*BKINGQA
00000000
10100000
01000000
00000000
00000000
00000000
00000000
00000000
*/

// TODO:
// Alternative King & Pawn bitboards for endgame
// King protection (pre-generated bitboard)

// Position Eval Bitboards - Hex
static const U64 bpawn5 = 0x9900E7000000;
static const U64 bpawn10 = 0xC3002418E7FF00;
static const U64 bpawn20 = 0x180018FF00;
static const U64 wpawn5 = 0xE700990000;
static const U64 wpawn10 = 0xFFE7182400C300;
static const U64 wpawn20 = 0xFF180018000000;
static const U64 bknight5 = 0x18000081812400;
static const U64 bknight10 = 0x3C6666661800;
static const U64 bknight20 = 0x1818180000;
static const U64 wknight5 = 0x24818100001800;
static const U64 wknight10 = 0x186666663C0000;
static const U64 wknight20 = 0x181818000000;
static const U64 bwbishop10 = 0x18244242241800;
static const U64 bwbishop20 = 0x183C3C180000;
static const U64 brook10 = 0x242424242424FF24;
static const U64 brook15 = 0x181818181818FF18;
static const U64 wrook10 = 0x24FF242424242424;
static const U64 wrook15 = 0x18FF181818181818;
static const U64 wkingk = 0x700;
static const U64 wkingq = 0xE000;
static const U64 bkingk = 0x7000000000000;
static const U64 bkingq = 0xE0000000000000;
static const U64 wkingka = 0x20500;
static const U64 wkingqa = 0x40A000;
static const U64 bkingka = 0x5020000000000;
static const U64 bkingqa = 0xA0400000000000;

int evalPos(BOARD *board){
	int i,tmp,res = 0,score = 0;
	U64 bb;
	
	// Count all available pieces
	const int cWhitePawn = popCount(board->WHITE_PAWN);
	const int cWhiteKnight = popCount(board->WHITE_KNIGHT);
	const int cWhiteBishop = popCount(board->WHITE_BISHOP);
	const int cWhiteRook = popCount(board->WHITE_ROOK);
	const int cWhiteQueen = popCount(board->WHITE_QUEEN);
	const int cBlackPawn = popCount(board->BLACK_PAWN);
	const int cBlackKnight = popCount(board->BLACK_KNIGHT);
	const int cBlackBishop = popCount(board->BLACK_BISHOP);
	const int cBlackRook = popCount(board->BLACK_ROOK);
	const int cBlackQueen = popCount(board->BLACK_QUEEN);
	
	// Material Eval
	const int whiteMaterial = (cWhitePawn + 3 * (cWhiteKnight + cWhiteBishop) + 5 * cWhiteRook + 9 * cWhiteQueen);
	const int blackMaterial = (cBlackPawn + 3 * (cBlackKnight + cBlackBishop) + 5 * cBlackRook + 9 * cBlackQueen);
	
	if(blackMaterial != 0){
		score += (80+whiteMaterial/blackMaterial*20)*whiteMaterial;
	}
	
	if(whiteMaterial != 0){
		score -= (80+blackMaterial/whiteMaterial*20)*blackMaterial;
	}
	
	if(cBlackBishop > 1){
		score -= 30;	
	}
	if(cWhiteBishop > 1){
		score += 30;	
	}
	if(cBlackRook > 1){
		score -= 50;	
	}
	if(cWhiteRook > 1){
		score += 50;	
	}
		
	//Position Eval
	if(cWhitePawn != 0){
	score += 5 * popCount(wpawn5 & board->WHITE_PAWN);
	score += 10 * popCount(wpawn10 & board->WHITE_PAWN);
	score += 20 * popCount(wpawn20 & board->WHITE_PAWN);
	}
	if(cBlackPawn != 0){
	score -= 5 * popCount(bpawn5 & board->BLACK_PAWN);
	score -= 10 * popCount(bpawn10 & board->BLACK_PAWN);
	score -= 20 * popCount(bpawn20 & board->BLACK_PAWN);
	}
	
	if(cWhiteKnight != 0){
	score += 5 * popCount(wknight5 & board->WHITE_KNIGHT); // Optional
	score += 10 * popCount(wknight10 & board->WHITE_KNIGHT);
	score += 20 * popCount(wknight20 & board->WHITE_KNIGHT);
	}
	if(cBlackKnight != 0){
	score -= 5 * popCount(bknight5 & board->BLACK_KNIGHT); // Optional
	score -= 10 * popCount(bknight10 & board->BLACK_KNIGHT);
	score -= 20 * popCount(bknight20 & board->BLACK_KNIGHT);
	}
	if(cWhiteBishop != 0){
	score += 10 * popCount(bwbishop10 & board->WHITE_BISHOP);
	score += 20 * popCount(bwbishop20 & board->WHITE_BISHOP);
	}
	if(cBlackBishop != 0){
	score -= 10 * popCount(bwbishop10 & board->BLACK_BISHOP);
	score -= 20 * popCount(bwbishop20 & board->BLACK_BISHOP);
	}
	
	if(cWhiteRook != 0){
	score += 10 * popCount(wrook10 & board->WHITE_ROOK);
	score += 15 * popCount(wrook15 & board->WHITE_ROOK);
	}
	if(cBlackRook != 0){
	score -= 10 * popCount(brook10 & board->BLACK_ROOK);
	score -= 15 * popCount(brook15 & board->BLACK_ROOK);
	}
	
	// Open file bonus
	if(whiteMaterial + blackMaterial < 50){
		for(i=0;i<8;i++){
			if(popCount(fileBitBoards[i] & board->BOTH) == 1){
			      // Push for promotion
				res = 0;
				bb = fileBitBoards[i] & board->WHITE_PAWN;
				tmp = popCount(bb);
				score += popCount(bb) * 15;
				while (bb >>= 1) res++;
				score+=r(res)*5;
				bb = fileBitBoards[i] & board->BLACK_PAWN;
				score -= popCount(bb) * 15;
				while (bb >>= 1) res++;
				score+=r(res)*5;
			}else{
				//Double Pawns
				bb = fileBitBoards[i] & board->WHITE_PAWN;
				if(popCount(bb) > 1){
					score-=10;
				}
				bb = fileBitBoards[i] & board->BLACK_PAWN;
				if(popCount(bb) > 1){
					score+=10;
				}
			}
		}
	}else{
	      // King protection
	      if(board->kings[1] == 1){
		      if(popCount(board->WHITE_PAWN & wkingk) == 3){
		      	score+=40;	
		      }else if(popCount(board->WHITE_PAWN & wkingka) == 3){
		      	score+=25;	
		      }
		      if(1ULL << 2 & board->WHITE){
		      	score+=15;	
		      }	
	      }else if(board->kings[1] == 5){
		      if(popCount(board->WHITE_PAWN & wkingq) == 3){
			      score+=40;	
		      }else if(popCount(board->WHITE_PAWN & wkingqa) == 3){
			      score+=25;	
		      }
		      if(1ULL << 4 & board->WHITE){
			      score+=15;	
		      }		
	      }

	
	      if(board->kings[0] == 57){
		      if(popCount(board->BLACK_PAWN & bkingk) == 3){
		      	score-=40;	
		      }else if(popCount(board->BLACK_PAWN & bkingka) == 3){
		      	score-=25;	
		      }
		      if(1ULL << 58 & board->BLACK){
		      	score-=15;	
		      }			
	      }else if(board->kings[0] == 61){
	      	if(popCount(board->BLACK_PAWN & bkingq) == 3){
		      	score-=40;	
		      }else if(popCount(board->BLACK_PAWN & bkingqa) == 3){
		      	score-=25;	
		      }
		      if(1ULL << 60 & board->BLACK){
		      	score-=15;	
		      }			
	      }
	}
	
	if(board->side == 1) {
		return score;
	} else {
		return -score;
	}	
}
