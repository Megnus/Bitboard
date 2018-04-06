//============================================================================
// Name        : bitboard.cpp
// Author      : Magnus
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================


#include "chessboardio.h"
//using namespace std;
using namespace chessio;
/*
using ChessboardIO::print;
using ChessboardIO::printSmallBoard;
using ChessboardIO::printBigBoard;*/

void LS1B(uint64_t u64, uint64_t *p) {
	while (u64 > 0) {
		*(p++) = u64 & -u64;  //LS1B
		u64 &= (u64 - 1);	  //LS1B_reset
	}
	*(p++) = 0;
}

void print_all(uint64_t u64) {
	uint64_t l[100];
	uint64_t *p = l;
	LS1B(u64, p);
	while (*p > 0) {
		ChessboardIO::print(*(p++));
	};
}

uint64_t knightAttacks(uint64_t knights) {
	uint64_t l1 = (knights >> 1) & uint64_t(0x7f7f7f7f7f7f7f7f);
	uint64_t l2 = (knights >> 2) & uint64_t(0x3f3f3f3f3f3f3f3f);
	uint64_t r1 = (knights << 1) & uint64_t(0xfefefefefefefefe);
	uint64_t r2 = (knights << 2) & uint64_t(0xfcfcfcfcfcfcfcfc);
	uint64_t h1 = l1 | r1;
	uint64_t h2 = l2 | r2;
   return (h1<<16) | (h1>>16) | (h2<<8) | (h2>>8);
}

uint64_t flipVertical(uint64_t x) {
    return  ( (x << 56)                           ) |
            ( (x << 40) & uint64_t(0x00ff000000000000) ) |
            ( (x << 24) & uint64_t(0x0000ff0000000000) ) |
            ( (x <<  8) & uint64_t(0x000000ff00000000) ) |
            ( (x >>  8) & uint64_t(0x00000000ff000000) ) |
            ( (x >> 24) & uint64_t(0x0000000000ff0000) ) |
            ( (x >> 40) & uint64_t(0x000000000000ff00) ) |
            ( (x >> 56) );
}
/*
uint64_t fileAttacks(uint64_t occ, enumSquare sq) {
	uint64_t forward, reverse;
   forward  = occ & smsk[sq].fileMaskEx;
   reverse  = __bswap_constant_64(forward);
   forward -= smsk[sq].bitMask;
   reverse -= __bswap_constant_64(smsk[sq].bitMask);
   forward ^= __bswap_constant_64(reverse);
   forward &= smsk[sq].fileMaskEx;
   return forward;
}*/

uint64_t mirrorHorizontal(uint64_t x) {
   const uint64_t k1 = uint64_t(0x5555555555555555);
   const uint64_t k2 = uint64_t(0x3333333333333333);
   const uint64_t k4 = uint64_t(0x0f0f0f0f0f0f0f0f);
   x = ((x >> 1) & k1) | ((x & k1) << 1);
   x = ((x >> 2) & k2) | ((x & k2) << 2);
   x = ((x >> 4) & k4) | ((x & k4) << 4);
   return x;
}

uint64_t rotate180(uint64_t x) {
   return mirrorHorizontal(flipVertical(x));
}

const int lsb_64_table[64] = {
   63, 30,  3, 32, 59, 14, 11, 33,
   60, 24, 50,  9, 55, 19, 21, 34,
   61, 29,  2, 53, 51, 23, 41, 18,
   56, 28,  1, 43, 46, 27,  0, 35,
   62, 31, 58,  4,  5, 49, 54,  6,
   15, 52, 12, 40,  7, 42, 45, 16,
   25, 57, 48, 13, 10, 39,  8, 44,
   20, 47, 38, 22, 17, 37, 36, 26
};

int bitScanForward(uint64_t bb) {
   unsigned int folded;
   assert (bb != 0);
   bb ^= bb - 1;
   folded = (int) bb ^ (bb >> 32);
   return lsb_64_table[folded * 0x78291ACF >> 26];
}

int main() {
	uint64_t u64 = 0b0000000000000000000000000000000000000000000000000001000000000000;
                 //00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000
	uint64_t r = u64;
	uint64_t o = 0b0001000000010000000000000000000000000000000000000001000000010000;
	//__bswap_constant_64(u64);
	ChessboardIO::print(u64);
	ChessboardIO::print(__bswap_constant_64(u64));
	ChessboardIO::print(rotate180(u64));

	uint64_t lineAttacks = (o - 2 * r);// ^ rotate180(rotate180(o) - 2 * rotate180(r));
	uint64_t lineAttacks2 = lineAttacks ^ o;
	ChessboardIO::print(lineAttacks);
	ChessboardIO::print(lineAttacks2);
	ChessboardIO::printSmallBoard();
	ChessboardIO::print(r);
	ChessboardIO::print(o);
	ChessboardIO::printBigBoard(o);
	cout << ChessboardIO::getSquare(11) << endl;
	/*print_all(u64);
	uint64_t ka = knightAttacks(u64);
	print(ka);
	print_all(ka);*/

	uint64_t u64x = 0x0102030405060708;
	unsigned long ul = 0x01020304;
	printf("byteswap of %I64x = %I64x\n", u64x, __bswap_constant_64(u64x));
	printf("byteswap of %Ix = %Ix", ul, __bswap_constant_32(ul));
	return 0;
}
