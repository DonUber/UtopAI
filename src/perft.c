#include "inc.h"
#include "stdio.h"
long leafNodes;
int perft (int depth, BOARD *pos){
	if(depth==0){
		leafNodes++;
		return 0;
	}
	MOVELIST list[1];
	genMoves(pos,list);
	int nMove;
	for(nMove=0;nMove<list->count;++nMove){
		if(!makeMove(pos,list->moves[nMove].move)){
			continue;
		}
		perft(depth - 1, pos);
		takeMove(pos);
	}
	return 1;
}
void perftTest(int depth, BOARD *pos) {
	printf("\nStarting Test To Depth:%d\n",depth);	
	leafNodes = 0;
	long start = getTimeMs();
    MOVELIST list[1];
    genMoves(pos,list);	
    
    int move;	    
    int nMove = 0;
	for(nMove = 0; nMove < list->count; ++nMove) {
        move = list->moves[nMove].move;
        if (!makeMove(pos,move))  {
            continue;
        }
        long cumnodes = leafNodes;
        perft(depth - 1, pos);
        takeMove(pos);

        long oldnodes = leafNodes - cumnodes;
        printf("move %d : %s : %ld\n",nMove+1,printMove(move),oldnodes);
    }
	long timeElap = getTimeMs()-start;
	long nodesSec = (leafNodes*1000)/timeElap;
	printf("\nTest Complete : %ld nodes visited in %ld ms at %ld nodes per second\n",leafNodes,timeElap, nodesSec);
//printf("\nTest Complete : %ld nodes visited\n",leafNodes);
}
