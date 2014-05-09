#include "stdio.h"
#include "inc.h"
#include "string.h"
#define INPUTBUFFER 400*6 //stdin ipv dit?

static void go(char* line, SEARCHINFO *info, BOARD *board){
	int depth = -1, movestogo = 30,inc=0;
	long movetime=-1, time=-1;
	char *pChar = NULL;
	info->timeset = 0;

	
	if ((pChar = strstr(line,"binc")) && board->side == 0) {
		inc = atoi(pChar + 5);
	}
	
	if ((pChar = strstr(line,"winc")) && board->side == 1) {
		inc = atoi(pChar + 5);
	} 
	
	if ((pChar = strstr(line,"wtime")) && board->side == 1) {
		time = atoi(pChar + 6);
	} 
	  
	if ((pChar = strstr(line,"btime")) && board->side == 0) {
		time = atoi(pChar + 6);
	} 
	  
	if ((pChar = strstr(line,"movestogo"))) {
		movestogo = atoi(pChar + 10);
	} 
	  
	if ((pChar = strstr(line,"movetime"))) {
		movetime = atoi(pChar + 9);
	}
	  
	if ((pChar = strstr(line,"depth"))) {
		depth = atoi(pChar + 6);
	} 
	
	if(movetime != -1){
		time = movetime;
		movestogo = 1;
	}
	info->starttime = getTimeMs();
	if(depth == -1) {
		info->depth = MAX_DEPTH;
	}else{
		info->depth = depth;
	}
	if(time != -1) {
		info->timeset = 1;
		time /= movestogo;
		time -= 50;		
		info->stoptime = info->starttime + time + inc;
	} 
	printf("time:%ld start:%ld stop:%ld depth:%d timeset:%ld\n",
		time,info->starttime,info->stoptime,info->depth,info->timeset);
	searchPos(board, info);
}

static void parsePos(char* line, BOARD *board){
	//More efficient?
	line += 9;
	char *pChar = line;
	if(!strncmp(line, "startboard",8)){
		setPos(START_FEN, board);
	}else{
		pChar = strstr(line, "fen");
		if(pChar != NULL){
			pChar += 4;
			setPos(pChar, board);
		}else{
			setPos(START_FEN,board);
		}
	}
	
	pChar = strstr(line, "moves");
	int move;
	if(pChar != NULL){
		//printf("IEOR!\n");
		pChar += 6;
		while(*pChar){
			move = parseMove(pChar, board);
			if(move ==  0) break;
			makeMove(board,move);
			board->ply=0;
			while(*pChar && *pChar!=' ') pChar++;
			pChar++;
		}
	}
	//printBoard(board);	
}
void uciLoop(){
	setbuf(stdin, NULL);
	setbuf(stdout, NULL);
	char line[INPUTBUFFER];
	printf("id name %s\n", NAME);
	printf("id author DonUber\n"); // Ook def?
	printf("uciok\n");
	
	// Start new game commands
	BOARD board[1];
	SEARCHINFO info[1];
	//S_MOVELIST list[1];
	initPvTable(board->pvTable);
	setPos(START_FEN, board);
	while(1){
		memset(&line[0], 0, sizeof(line));
		fflush(stdout);
		if (!fgets(line, INPUTBUFFER, stdin)) continue;
		if(line[0] == '\n') continue;
		if(!strncmp(line, "isready", 7)){
			printf("readyok\n");
			continue;
		}else if(!strncmp(line, "ucinewgame", 10) || !strncmp(line, "new", 3)){
			parsePos(START_FEN, board);
		}else if(!strncmp(line, "position", 8)){
			parsePos(line, board);
		}else if(!strncmp(line, "go", 2)){
		      printf("%s", line);
			go(line,info,board);
		}else if(!strncmp(line, "quit", 4)){
			info->quit = 1;
			break;
		}else if(!strncmp(line, "uci", 3)){
			printf("id name %s\n", NAME);
			printf("id author DonUber\n"); // Ook def?
			printf("uciok\n");
		}else if(!strncmp(line, "eval", 4)){
			printf("score %d\n",evalPos(board));
		}else if(!strncmp(line, "perf", 4)){
			perftTest(6, board);
		}
		if(info->quit) break;
	}
	free(board->pvTable->pTable);
}

int parseMove(char *moveChar, BOARD *pos){
	//printf("%c%c%c%c\n",moveChar[0],moveChar[1],moveChar[2],moveChar[3]);
	if(moveChar[1] > '8' || moveChar[1] < '1') return 0;
	if(moveChar[3] > '8' || moveChar[3] < '1') return 0;
	if(moveChar[0] > 'h' || moveChar[0] < 'a') return 0;
	if(moveChar[2] > 'h' || moveChar[2] < 'a') return 0;
	int a = moveChar[0] - 'a';
	int b = moveChar[1] - '1';
	int c = moveChar[2] - 'a';
	int d = moveChar[3] - '1';
	int from = SQ2BIT(a, b);
	int to = SQ2BIT(c, d);
	
	MOVELIST list[1];
	genMoves(pos,list); //Store already calculated move in table?
	int nMove = 0;
	int move = 0;
	int promPiece = EMPTY;
	for(nMove=0;nMove<list->count;nMove++){
		move = list->moves[nMove].move;
		if(FROMSQ(move)==from&&TOSQ(move)==to){
			promPiece = PROMOTED(move);
			if(promPiece!=EMPTY){
				if((promPiece == 4 || promPiece == 9) && moveChar[4]=='r'){
					return move;
				}else if((promPiece == 3 || promPiece == 8) && moveChar[4]=='b'){
					return move;
				}else if((promPiece == 5 || promPiece == 10) && moveChar[4]=='q'){
					return move;
				}else if((promPiece == 2 || promPiece == 7)&& moveChar[4]=='n'){
					return move;
				}
				continue;
			}
			return move;
		}
	}
	return 0;
}

