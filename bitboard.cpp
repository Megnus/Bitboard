//============================================================================
// Name        : bitboard.cpp
// Author      : Magnus
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================


#include "chessboardio.h"
#include "tool.h"
//using namespace std;
using namespace chessio;

void LS1B(uint64_t u64, uint64_t *p) {
	while (u64 > 0) {
		*(p++) = u64 & -u64;  //LS1B
		u64 &= (u64 - 1);	  //LS1B_reset
	}
	*(p++) = 0;
}

const uint64_t notAFile = 0xfefefefefefefefe; // ~0x0101010101010101
const uint64_t notHFile = 0x7f7f7f7f7f7f7f7f; // ~0x8080808080808080
const uint64_t rank4 = uint64_t(0x00000000FF000000);
const uint64_t rank5 = uint64_t(0x000000FF00000000);

uint64_t nortOne (uint64_t b) {return  b << 8;}
uint64_t soutOne (uint64_t b) {return  b >> 8;}
uint64_t eastOne (uint64_t b) {return (b << 1) & notAFile;}
uint64_t noEaOne (uint64_t b) {return (b << 9) & notAFile;}
uint64_t soEaOne (uint64_t b) {return (b >> 7) & notAFile;}
uint64_t westOne (uint64_t b) {return (b >> 1) & notHFile;}
uint64_t soWeOne (uint64_t b) {return (b >> 9) & notHFile;}
uint64_t noWeOne (uint64_t b) {return (b << 7) & notHFile;}

void print_all(uint64_t u64) {
	uint64_t l[100];
	uint64_t *p = l;
	LS1B(u64, p);
	while (*p > 0) {
		ChessboardIO::print(*(p++));
	};
}

uint64_t wPawnAttacks(uint64_t wb, uint64_t bb, uint64_t wPawns) {
	uint64_t empty = not (bb | wb);
	uint64_t singlePushs = nortOne(wPawns) ^ empty;
	uint64_t attacks = noEaOne(wPawns) & bb;
	attacks |= noWeOne(wPawns) & bb;
	attacks |= singlePushs;
	attacks |= nortOne(singlePushs) & empty & rank4;
	return attacks;
}

uint64_t bPawnAttacks(uint64_t wb, uint64_t bb, uint64_t bPawns) {
	uint64_t empty = not (bb | wb);
	uint64_t singlePushs = soutOne(bPawns) ^ empty;
	uint64_t attacks = soEaOne(bPawns) & wb;
	attacks |= soWeOne(bPawns) & wb;
	attacks |= singlePushs;
	attacks |= soutOne(singlePushs) & empty & rank5;
	return attacks;
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

static struct {
	uint64_t bitMask;         // 1 << sq for convenience
	uint64_t diagonalMaskEx;
	uint64_t antidiagMaskEx;
	uint64_t fileMaskEx;
	//uint64_t rankMaskEx;
} smsk[64]; // 2 KByte

uint8_t arrFirstRankAttacks64x8[64 * 8]; // 512 Bytes = 1/2KByte

uint64_t fileAttacks(uint64_t occ, ChessboardIO::enumSquare sq) {
	uint64_t forward, reverse;
   forward  = occ & smsk[sq].fileMaskEx;
   reverse  = __bswap_constant_64(forward);
   forward -= smsk[sq].bitMask;
   reverse -= __bswap_constant_64(smsk[sq].bitMask);
   forward ^= __bswap_constant_64(reverse);
   forward &= smsk[sq].fileMaskEx;
   return forward;
}

uint64_t antiDiagAttacks(uint64_t occ, ChessboardIO::enumSquare sq) {
	uint64_t forward, reverse;
   forward  = occ & smsk[sq].antidiagMaskEx;
   reverse  = __bswap_constant_64(forward);
   forward -= smsk[sq].bitMask;
   reverse -= __bswap_constant_64(smsk[sq].bitMask);
   forward ^= __bswap_constant_64(reverse);
   forward &= smsk[sq].antidiagMaskEx;
   return forward;
}

uint64_t diagonalAttacks(uint64_t occ, ChessboardIO::enumSquare sq) {
	uint64_t forward, reverse;
   forward = occ & smsk[sq].diagonalMaskEx;
   reverse  = __bswap_constant_64(forward);
   forward -= smsk[sq].bitMask;
   reverse -= __bswap_constant_64(smsk[sq].bitMask);
   forward ^= __bswap_constant_64(reverse);
   forward &= smsk[sq].diagonalMaskEx;
   return forward;
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

uint64_t eastMaskEx(int sq) {
   const uint64_t one = 1;
   return 2*( (one << (sq | 7)) - (one << sq) );
}

uint64_t diagonalMask(int sq) {
   const uint64_t maindia = uint64_t(0x8040201008040201);
   int diag = 8 * (sq & 7) - (sq & 56);
   int nort = -diag & ( diag >> 31);
   int sout =  diag & (-diag >> 31);
   return (maindia >> sout) << nort;
}

uint64_t antiDiagMask(int sq) {
   const uint64_t maindia = uint64_t(0x0102040810204080);
   int diag = 56 - 8 * (sq & 7) - (sq & 56);
   int nort = -diag & ( diag >> 31);
   int sout =  diag & (-diag >> 31);
   return (maindia >> sout) << nort;
}

uint64_t rankMask(int sq) {
	return  uint64_t(0xff) << (sq & 56);
}

uint64_t fileMask(int sq) {
	return uint64_t(0x0101010101010101) << (sq & 7);
}

uint64_t bitMask(int sq) {
	return (uint64_t)1 << sq;
}

uint64_t rankAttacks(uint64_t occ, ChessboardIO::enumSquare sq) {
   unsigned int file = sq & 7;
   unsigned int rkx8 = sq & 56; // rank * 8
   occ = (occ >> rkx8) & 2 * 63;
   uint64_t attacks = arrFirstRankAttacks64x8[4 * occ + file];
   return attacks << rkx8;
}

void fillArrFirstRankAttacks64x8() {
	for (uint64_t occ64 = 0; occ64 < 255; occ64++) {
		for (int sq = 0; sq < 8; sq++) {
			if ((occ64 & (1 << sq)) == 0 ) {
				int swapSq = Tool::swapSqByflipDiagA1H8((int)sq);
				uint64_t attacks = Tool::flipDiagA1H8(occ64);
				attacks = fileAttacks(attacks, (ChessboardIO::enumSquare)swapSq);
				attacks = Tool::flipDiagA1H8(attacks);
				unsigned int file = sq & 7;
				unsigned int rkx8 = sq & 56;
				uint8_t occ8 = (occ64 >> rkx8) & 2 * 63;
				arrFirstRankAttacks64x8[4 * occ8 + file] = (uint8_t)attacks;
			}
		}
	}
}

void fillSmsk() {
	for (int sq = 0; sq < 64; sq++) {
		smsk[sq].bitMask = bitMask(sq);
		smsk[sq].fileMaskEx = fileMask(sq);
		smsk[sq].diagonalMaskEx = diagonalMask(sq);
		smsk[sq].antidiagMaskEx = antiDiagMask(sq);
	}
}

void createLookUpAttacks() {
	fillSmsk();
	fillArrFirstRankAttacks64x8();
}

int main() {
	createLookUpAttacks();
	cout << 0b11111111 << endl;
	uint64_t occ = 0b01000100;
	ChessboardIO::printBigBoard(occ);
	uint64_t attack = rankAttacks(occ, ChessboardIO::e1);
	ChessboardIO::printBigBoard(attack);
	//if (true) return 0;

	uint64_t u64 = 0b0000000000000000000000000000000000000000000000000001000000000000;
                 //00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000
	uint64_t r = u64;
	uint64_t o = 0b0001000000010000000000000000000000000000000100000001000000010000;
	//__bswap_constant_64(u64);
	ChessboardIO::print(u64);
	ChessboardIO::print(__bswap_constant_64(u64));
	ChessboardIO::print(Tool::rotate180(u64));

	uint64_t lineAttacks = (o - 2 * r);// ^ rotate180(rotate180(o) - 2 * rotate180(r));
	uint64_t lineAttacks2 = lineAttacks ^ o;
	ChessboardIO::print(lineAttacks);
	ChessboardIO::print(lineAttacks2);
	ChessboardIO::printSmallBoard();
	ChessboardIO::print(r);
	ChessboardIO::print(o);
	ChessboardIO::printBigBoard(o);
	cout << ChessboardIO::getSquare(11) << endl;

	uint64_t x = antiDiagMask((int)ChessboardIO::f3);
	x |= diagonalMask((int)ChessboardIO::f3);
	x |= rankMask((int)ChessboardIO::f3);
	x |= fileMask((int)ChessboardIO::f3);
	x |= rankAttacks(occ, ChessboardIO::f3);
	ChessboardIO::printBigBoard(x);


	x = fileAttacks(o, ChessboardIO::g3);
	x |= diagonalAttacks(o, ChessboardIO::g3);
	x |= antiDiagAttacks(o, ChessboardIO::g3);
	x |= rankAttacks(o, ChessboardIO::g3);
	x |= knightAttacks(uint64_t(1) << ChessboardIO::b3);
	ChessboardIO::printBigBoard(o);
	ChessboardIO::printBigBoard(x);

	cout << "---------------" << endl;
	uint64_t u64x = 0x0102030405060708;
	unsigned long ul = 0x01020304;
	printf("byteswap of %I64x = %I64x\n", u64x, __bswap_constant_64(u64x));
	printf("byteswap of %Ix = %Ix", ul, __bswap_constant_32(ul));
	return 0;
}
