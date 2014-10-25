#include "stdlib.h"

#define NAME "UtopAI"
#define VERSION "001"
#define MAX_GAME_MOVES 2048
#define MAX_POSITION_MOVES 256
#define MAX_DEPTH 64
#define START_FEN "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"

#define SQ2BIT(f,r)((r*8 + 7 - f))
#define BITMOVE(f,t,ca,pro,fl) ( (f) | ((t) << 7) | ( (ca) << 14 ) | ( (pro) << 20 ) | (fl))
#define FROMSQ(m) ((m) & 0x7F)
#define TOSQ(m) (((m)>>7) & 0x7F)
#define CAPTURED(m) (((m)>>14) & 0xF)
#define PROMOTED(m) (((m)>>20) & 0xF)
#define f(i) ((i) & 7)
#define r(i) ((i-f(i)) / 8)
#define SETBIT(bb,i) ((bb)|= 1ULL << (i))
#define CLRBIT(bb,i) ((bb) &= ~(1ULL << (i)))

#define MFLAGEP 0x40000
#define MFLAGPS 0x80000
#define MFLAGC 0x1000000

#define MFLAGCAP 0x7C000
#define MFLAGPROM 0xF00000

enum { EMPTY, wP, wN, wB, wR, wQ, wK, bP, bN, bB, bR, bQ, bK  };
typedef unsigned long long int U64;

typedef struct{
	U64 POS_KEY;
	int move;
} PVENTRY;

typedef struct{
	PVENTRY *pTable;
	int nEntries;
} PVTABLE;

typedef struct{
	int move;
	char enPas;
	char fifty;
	char castlePerm;
	U64 POS_KEY;
} UNDO;

typedef struct{
	int move;
	int score;
} MOVE;

typedef struct{
	MOVE moves[MAX_POSITION_MOVES];
	int count;
} MOVELIST;

typedef struct {
	UNDO history[MAX_GAME_MOVES];
	PVTABLE pvTable[1];
	
	U64 WHITE;
	U64 BLACK;
	U64 BOTH;
	U64 BLACK_PAWN;
	U64 WHITE_PAWN;
	U64 BLACK_KNIGHT;
	U64 WHITE_KNIGHT;
	U64 BLACK_BISHOP;
	U64 WHITE_BISHOP;
	U64 BLACK_ROOK;
	U64 WHITE_ROOK;
	U64 BLACK_QUEEN;
	U64 WHITE_QUEEN;
	U64 BLACK_KING;
	U64 WHITE_KING;
	U64 POS_KEY;
	int searchHistory[13][64];
	int searchKillers[2][MAX_DEPTH];
	int pvArray[MAX_DEPTH];
	int ply;
	int hisPly;
	int kings[2];
	char enPas;
	char side;
	char castlePerm;
	char fifty;
} BOARD;

typedef struct{
	long starttime;
	long stoptime;
	long timeset;
	long nodes;
	int depth;
	int depthset;
	int inf;
	int movestogo;
	char quit;
	char stopped;
	//float fh;
	//float fhf;
} SEARCHINFO;

extern U64 SideKey;
extern U64 CastleKeys[16];
extern long getTimeMs();
extern U64 fileBitBoards[8];
extern U64 rankBitBoards[8];

extern void init();
extern int parseMove(char *moveChar, BOARD *pos);
extern void genMoves(BOARD *board, MOVELIST *list);
extern void genCapMoves(BOARD *board, MOVELIST *list);
extern int setPos(char *fen, BOARD *board);
extern void resetBoard(BOARD *board);
extern U64 genPosKey(const BOARD *board);
extern int pieceSq(const BOARD *board, const int i);
extern char *printMove(const int move);
extern void printMoveList(const MOVELIST *list);
extern int sqAttacked(const char sq, const char side, const BOARD *board);
extern int makeMove(BOARD *board, const int move);
extern void takeMove(BOARD *board);
extern int evalPos(BOARD *board);
extern void searchPos(BOARD *board, SEARCHINFO *info);
extern int popCount(U64 bb);
extern void perftTest(int depth, BOARD *pos);
extern void uciLoop();
extern int evalPos(BOARD *board);
extern void initMvvLva();
extern void storePvMove(const BOARD *board, const int move);
extern int getPvLine(const int depth, BOARD *board);
extern void clearPvTable(PVTABLE *table);
extern void initPvTable(PVTABLE *table);
extern int probePvTable(const BOARD *board);
