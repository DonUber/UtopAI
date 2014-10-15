#include "stdio.h"
#include "inc.h"
#include "string.h"
#ifdef WIN32
#include "windows.h"
#else
#include "time.h"
#include "sys/select.h"
#endif
#define RAND_64 (	(U64)rand() | \
					(U64)rand() << 15 | \
					(U64)rand() << 30 | \
					(U64)rand() << 45 | \
					((U64)rand() & 0xf) << 60)

U64 SideKey;
U64 CastleKeys[16];
U64 fileBitBoards[8]; 
U64 rankBitBoards[8]; 
 
long getTimeMs(){ 
#ifdef WIN32
  return GetTickCount();
#else
  struct timeval t;
  gettimeofday(&t, NULL);
  return t.tv_sec*1000 + t.tv_usec/1000;
#endif
}

int inputWaiting()
{
#ifndef WIN32
  fd_set readfds;
  struct timeval tv;
  FD_ZERO (&readfds);
  FD_SET (fileno(stdin), &readfds);
  tv.tv_sec=0; tv.tv_usec=0;
  select(16, &readfds, 0, 0, &tv);

  return (FD_ISSET(fileno(stdin), &readfds));
#else
   static int init = 0, pipe;
   static HANDLE inh;
   DWORD dw;

   if (!init) {
     init = 1;
     inh = GetStdHandle(STD_INPUT_HANDLE);
     pipe = !GetConsoleMode(inh, &dw);
     if (!pipe) {
        SetConsoleMode(inh, dw & ~(ENABLE_MOUSE_INPUT|ENABLE_WINDOW_INPUT));
        FlushConsoleInputBuffer(inh);
      }
    }
    if (pipe) {
      if (!PeekNamedPipe(inh, NULL, 0, NULL, &dw, NULL)) return 1;
      return dw;
    } else {
      GetNumberOfConsoleInputEvents(inh, &dw);
      return dw <= 1 ? 0 : dw;
	}
#endif
}

void readInput(SEARCHINFO *info) {
  int             bytes;
  char            input[256] = "", *endc;

    if (inputWaiting()) {    
		info->stopped = 1;
		do {
		  bytes=read(fileno(stdin),input,256);
		} while (bytes<0);
		endc = strchr(input,'\n');
		if (endc) *endc=0;

		if (strlen(input) > 0) {
			if (!strncmp(input, "quit", 4))    {
			  info->quit = 1;
			}
		}
		return;
    }
}

#ifdef USE_POPCNT
#include "smmintrin.h"
int popCount(U64 bb){
 	return _mm_popcnt_u64(bb);
}
#elif USE_LOOKUP
unsigned char popCountOfByte256[];
 
void initpopCountOfByte256()
{
   popCountOfByte256[0] = 0;
   int i = 1;
   for (i=1; i < 256; i++)
      popCountOfByte256[i] = popCountOfByte256[i / 2] + (i & 1);
}
 
int popCount (U64 bb) {
   return popCountOfByte256[ bb        & 0xff] +
          popCountOfByte256[(bb >>  8) & 0xff] +
          popCountOfByte256[(bb >> 16) & 0xff] +
          popCountOfByte256[(bb >> 24) & 0xff] +
          popCountOfByte256[(bb >> 32) & 0xff] +
          popCountOfByte256[(bb >> 40) & 0xff] +
          popCountOfByte256[(bb >> 48) & 0xff] +
          popCountOfByte256[ bb >> 56];
}
#elif USE_SWAR
const U64 k1 = 0x5555555555555555; /*  -1/3   */
const U64 k2 = 0x3333333333333333; /*  -1/5   */
const U64 k4 = 0x0f0f0f0f0f0f0f0f; /*  -1/17  */
const U64 kf = 0x0101010101010101; /*  -1/255 */
int popCount (U64 bb) {
    bb =  bb       - ((bb >> 1)  & k1);
    bb = (bb & k2) + ((bb >> 2)  & k2);
    bb = (bb       +  (bb >> 4)) & k4 ;
    bb = (bb * kf) >> 56;
    return (int) bb;
}
#else
int popCount(U64 bb){
	int count = 0;
	while (bb){
		count++;
		bb &= bb-1;
	}
	return count;
}
#endif

static void initHashKeys() {
	int i = 0;
	SideKey = RAND_64;
	for(i = 0; i < 16; ++i) {
		CastleKeys[i] = RAND_64;
	}

}

static void initFileRankBitBoards(){
	int i,j;
	for(i=0;i<8;i++){
		for(j=0;j<8;j++){
			SETBIT(fileBitBoards[i],i+j*8);
			SETBIT(rankBitBoards[i],i*8+j);
		}	
	}
}

void init(){
	initHashKeys();
	initMvvLva();
	initFileRankBitBoards();
	#ifdef USE_LOOKUP
	initpopCountOfByte256();
	#endif
}
