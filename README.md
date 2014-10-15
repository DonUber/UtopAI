UtopAI
======

UtopAI is a multi-platform chess-engine using the UCI-protocol. 

Specifications:

- Protocol: UCI
- Board Representation: BitBoards
- Passed perft tests

TODO:

- Null Move Pruning
- Extend evaluation:
	- Endgame pre-generated position bitboards
	- King protection pre-generated bitboards
- Extend usage of transposition table
- Include book support
- Code optimisation

Compiling
-----------

```
make
```
will compile with the default flags

```
make -sse
```
will compile with the sse and optimisation flags: -O3 -msse4.2 -mfpmath=sse -DUSE_SWAR

Three POPCOUNT methods are implemented:

```
DUSE_POPCNT
```
Will use "_mm_popcnt_u64" from smmintrin.h

```
DUSE_SWAR
```
 Uses [SWAR](https://chessprogramming.wikispaces.com/Population+Count#SWAR-Popcount) popcount method
 
```
DUSE_LOOKUP
```
Uses the [lookup table](https://chessprogramming.wikispaces.com/Population+Count#Lookup) method

Contact
-----------

Bug reports or suggestions? Don't hesitate to mail me donuber36@gmail.com

