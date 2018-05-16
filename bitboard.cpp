//============================================================================
// Name			: bitboard.cpp
// Author		: Magnus Sundström
// Version		: 0.1
// Copyright	: Your copyright notice
// Description	: Bitboard for chess. C++, Ansi-style
//============================================================================


#include "chessboardio.h"
#include "tool.h"
#include "cboard.h"
#include "piece.h"
#include "pawn.h"
#include <map>
//using namespace std;
//using namespace chessio;

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

int nextBitScanForward(uint64_t *bb) {
	/*unsigned int folded;
	assert (*bb != 0);
	*bb ^= *bb - 1;
	folded = (int) *bb ^ (*bb >> 32);
	*bb &= (*bb - 1);
	return lsb_64_table[folded * 0x78291ACF >> 26];*/
	int result = bitScanForward(*bb);
	*bb &= (*bb - 1);
	return result;
}

void nextBitScanForward(uint64_t *u64, uint64_t *p) {
	*p = (*u64) & -(*u64);	//LS1B
	*u64 &= (*u64 - 1);	//LS1B_reset
}

void bitScanForward(uint64_t u64, int *p) {
	while (u64 > 0) {
		*(p++) = bitScanForward(u64);  //LS1B
		u64 &= (u64 - 1);	  //LS1B_reset
	}
	*(p++) = -1;
}

void LS1B(uint64_t u64, uint64_t *p) {
	while (u64 > 0) {
		*(p++) = u64 & -u64;  //LS1B
		u64 &= (u64 - 1);	  //LS1B_reset
	}
	*(p++) = 0;
}


void printMovesByMatrix(uint64_t x) {
	uint64_t moveArray[64];
	uint64_t* movePointer = moveArray;
	LS1B(x, movePointer);
	while (*movePointer != 0) {
		ChessboardIO::print(*(movePointer++));
	}
}

void printPicesBySquare(uint64_t x) {
	int pieceIndexArray[64];
	int* pieceIndexPointer = pieceIndexArray;
	bitScanForward(x, pieceIndexPointer);
	while (*pieceIndexPointer != -1) {
		cout << ChessboardIO::getSquare(*(pieceIndexPointer++)) << endl;
	}
}

void print_all(uint64_t u64) {
	uint64_t l[100];
	uint64_t *p = l;
	LS1B(u64, p);
	while (*p > 0) {
		ChessboardIO::print(*(p++));
	};
}

const uint64_t notAFile = 0xfefefefefefefefe; // ~0x0101010101010101
const uint64_t notHFile = 0x7f7f7f7f7f7f7f7f; // ~0x8080808080808080
const uint64_t rank4 = uint64_t(0x00000000FF000000);
const uint64_t rank5 = uint64_t(0x000000FF00000000);

uint64_t nortOne(uint64_t b) { return  b << 8; }
uint64_t soutOne(uint64_t b) { return  b >> 8; }
uint64_t eastOne(uint64_t b) { return (b << 1) & notAFile; }
uint64_t noEaOne(uint64_t b) { return (b << 9) & notAFile; }
uint64_t soEaOne(uint64_t b) { return (b >> 7) & notAFile; }
uint64_t westOne(uint64_t b) { return (b >> 1) & notHFile; }
uint64_t soWeOne(uint64_t b) { return (b >> 9) & notHFile; }
uint64_t noWeOne(uint64_t b) { return (b << 7) & notHFile; }



//--- Pawn attacks ----------------------------
uint64_t wPawnAttacks(uint64_t wb, uint64_t bb, uint64_t wPawns) {
	uint64_t empty = ~ (bb | wb);
	uint64_t singlePushs = nortOne(wPawns) & empty;
	uint64_t attacks = noEaOne(wPawns) & bb;
	attacks |= noWeOne(wPawns) & bb;
	attacks |= singlePushs;
	attacks |= nortOne(singlePushs) & empty & rank4;
	return attacks;
}

uint64_t bPawnAttacks(uint64_t wb, uint64_t bb, uint64_t bPawns) {
	uint64_t empty = ~ (bb | wb);
	uint64_t singlePushs = soutOne(bPawns) & empty;
	uint64_t attacks = soEaOne(bPawns) & wb;
	attacks |= soWeOne(bPawns) & wb;
	attacks |= singlePushs;
	attacks |= soutOne(singlePushs) & empty & rank5;
	return attacks;
}

uint64_t wPawnAttacks(uint64_t wb, uint64_t bb, ChessboardIO::enumSquare sq) {
	return wPawnAttacks(wb, bb, (uint64_t)1 << sq);
}

uint64_t bPawnAttacks(uint64_t wb, uint64_t bb, ChessboardIO::enumSquare sq) {
	return bPawnAttacks(wb, bb, (uint64_t)1 << sq);
}
//---------------------------------------------

//--- King attacks ----------------------------
uint64_t arrKingAttacks[64];

uint64_t kingAttacks(uint64_t kingSet) {
	uint64_t attack = nortOne(kingSet);
	attack |= nortOne(kingSet);
	attack |= soutOne(kingSet);
	attack |= eastOne(kingSet);
	attack |= noEaOne(kingSet);
	attack |= soEaOne(kingSet);
	attack |= westOne(kingSet);
	attack |= soWeOne(kingSet);
	attack |= noWeOne(kingSet);
	return attack;
}

void kingAttacks() {
	for (int sq = 0; sq < 64; sq++) {
		arrKingAttacks[sq] = kingAttacks((uint64_t) 1 << sq);
	}
}

uint64_t kingAttacks(ChessboardIO::enumSquare sq) {
	return arrKingAttacks[sq];
}
//--------------------------------------------

//--- Knight attacks -------------------------
uint64_t arrKnightAttacks[64];

uint64_t knightAttacks(uint64_t knightSet) {
	uint64_t l1 = (knightSet >> 1) & uint64_t(0x7f7f7f7f7f7f7f7f);
	uint64_t l2 = (knightSet >> 2) & uint64_t(0x3f3f3f3f3f3f3f3f);
	uint64_t r1 = (knightSet << 1) & uint64_t(0xfefefefefefefefe);
	uint64_t r2 = (knightSet << 2) & uint64_t(0xfcfcfcfcfcfcfcfc);
	uint64_t h1 = l1 | r1;
	uint64_t h2 = l2 | r2;
	return (h1<<16) | (h1>>16) | (h2<<8) | (h2>>8);
}

void knightAttacks() {
	for (int sq = 0; sq < 64; sq++) {
		arrKnightAttacks[sq] = knightAttacks((uint64_t) 1 << sq);
	}
}

uint64_t knightAttacks(ChessboardIO::enumSquare sq) {
	return arrKnightAttacks[sq];
}
//--------------------------------------------


static struct {
	uint64_t bitMask;			// 1 << sq for convenience
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

void swapTest() {
	uint64_t u64 = 0b0000000000000000000000000000000000000000000000000001000000000000;
	uint64_t bswap = __bswap_constant_64(u64);
	ChessboardIO::print(u64);
	ChessboardIO::print(bswap);
	ChessboardIO::print(Tool::rotate180(u64));

	uint64_t u64x = 0x0102030405060708;
	unsigned long ul = 0x01020304;
	printf("byteswap of %I64x = %I64x\n", u64x, __bswap_constant_64(u64x));
	printf("byteswap of %Ix = %Ix\n", ul, __bswap_constant_32(ul));
}

void printPawnAttacks() {
	ChessboardIO::enumSquare wEnumSquares[] = {
			ChessboardIO::b2,
			ChessboardIO::d2,
			ChessboardIO::g3,
			ChessboardIO::end };

	ChessboardIO::enumSquare bEnumSquares[] = {
			ChessboardIO::f4,
			ChessboardIO::d7,
			ChessboardIO::g7,
			ChessboardIO::end };

	uint64_t wpb = ChessboardIO::setBoard(wEnumSquares);
	uint64_t bpb = ChessboardIO::setBoard(bEnumSquares);
	ChessboardIO::printBigBoard(wpb);
	ChessboardIO::printBigBoard(bpb);
	//uint64_t board = ChessboardIO::setBoard(enumSquares);
	uint64_t x = wPawnAttacks(wpb, bpb, wpb);
	ChessboardIO::printBigBoard(x);

	x = bPawnAttacks(wpb, bpb, bpb);
	ChessboardIO::printBigBoard(x);
}

void printQueenAttacks(uint64_t o) {
	uint64_t x = fileAttacks(o, ChessboardIO::g3);
	x |= diagonalAttacks(o, ChessboardIO::g3);
	x |= antiDiagAttacks(o, ChessboardIO::g3);
	x |= rankAttacks(o, ChessboardIO::g3);
	x |= knightAttacks(ChessboardIO::b3);
	ChessboardIO::printBigBoard(o);
	ChessboardIO::printBigBoard(x);
}

void printMask(uint64_t occ) {
	uint64_t x = antiDiagMask((int) (ChessboardIO::f3));
	x |= diagonalMask((int) (ChessboardIO::f3));
	x |= rankMask((int) (ChessboardIO::f3));
	x |= fileMask((int) (ChessboardIO::f3));
	x |= rankAttacks(occ, ChessboardIO::f3);
	ChessboardIO::printBigBoard(x);
}

//Test by print all moves
void generateAllMoves(CBoard *cboard, CBoard::ColorType color) {
/*	uint64_t occ = u64.getPieceSet(CBoard::occ);
	uint64_t empty = u64.getPieceSet(CBoard::empty);
	uint64_t friends = u64.getPieceSet(CBoard::occ, color);
	uint64_t enymies = u64.getPieceSet(CBoard::occ, CBoard::black);

	uint64_t pawns = u64.getPieceSet(CBoard::nPawn, color);
	uint64_t knights = u64.getPieceSet(CBoard::nKnight, color);
	uint64_t bishops = u64.getPieceSet(CBoard::nBishop, color);
	uint64_t queens = u64.getPieceSet(CBoard::nQueen, color);
	uint64_t king = u64.getPieceSet(CBoard::nKing, color);*/

	std::map<CBoard::EnumPiece, Piece*> pieceMap;
	pieceMap[CBoard::nPawn] = new Pawn();
	/*pieceMap[CBoard::nKnight] = new Knight();
	pieceMap[CBoard::nBishop] = new Bishop();
	pieceMap[CBoard::nRook] = new Rook();
	pieceMap[CBoard::nQueen] = new Queen();
	pieceMap[CBoard::nKing] = new King();*/
	uint64_t origin = 0;
	uint64_t attack = 0;

	for (int piece = CBoard::nPawn; piece != CBoard::nKnight; piece++) {
		CBoard::EnumPiece type = static_cast<CBoard::EnumPiece>(piece);
		uint64_t pieces = cboard->getPieceSet(type, color);
		while (pieces > 0) {
			nextBitScanForward(&pieces, &origin);
			uint64_t piceAttack = pieceMap[CBoard::nPawn]->attacks(cboard, &origin);
			while (piceAttack > 0) {
				nextBitScanForward(&piceAttack, &attack);
				cout << ChessboardIO::getSquare64(origin) << "-" << ChessboardIO::getSquare64(attack) << endl;
			}
		}
	}
}

int main() {
	createLookUpAttacks();

	uint64_t occ = 0b01000100;
	ChessboardIO::printBigBoard(occ);
	uint64_t attack = rankAttacks(occ, ChessboardIO::e1);
	ChessboardIO::printBigBoard(attack);

	ChessboardIO::enumSquare enumSquares[] = {
			ChessboardIO::b2,
			ChessboardIO::d2,
			ChessboardIO::g3,
			ChessboardIO::end };

	uint64_t board = ChessboardIO::setBoard(enumSquares);
	ChessboardIO::printBigBoard(board);

	uint64_t o   = 0b0001000000010000000000000000000000000000000100000001000000010000;

	printMask(occ);

	printQueenAttacks(o);

	printPawnAttacks();

	printMovesByMatrix(board);
	
	printPicesBySquare(board);
	
	swapTest();

	CBoard *cboard = new CBoard();
	cboard->setPieceSet(ChessboardIO::setBoard(enumSquares), CBoard::nPawn, CBoard::white);
	ChessboardIO::printBigBoard(ChessboardIO::setBoard(enumSquares));
	generateAllMoves(cboard, CBoard::white);

	return 0;
}
