CC = gcc
CFLAGS  = -g -Wall
CSSEFLAGS = -O3 -msse4.2 -mfpmath=sse -DUSE_SWAR

default: def

def:
	$(CC) $(CFLAGS) utopai.c movegen.c board.c perft.c move.c init.c pvtable.c search.c eval.c uci.c -o utopai

sse:
	$(CC) $(CSSEFLAGS)  utopai.c movegen.c board.c perft.c move.c init.c pvtable.c search.c eval.c uci.c -o utopai

clean: 
	$(RM) utopai *.o *~