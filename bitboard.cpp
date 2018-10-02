//============================================================================
// Name			: bitboard.cpp
// Author		: Magnus Sundström
// Version		: 0.1
// Copyright	: Your copyright notice
// Description	: Bitboard for chess. C++, Ansi-style
//============================================================================

#include "chessboardio.h"
//#include "tool.h"
#include "cboard.h"
#include "piece\piece.h"
#include "piece\pawn.h"
#include "piece\knight.h"
#include "piece\king.h"
#include "piece\rook.h"
#include "piece\bishop.h"
#include "piece\queen.h"
#include "piece\castle.h"
#include <map>
#include <string>
using namespace std;
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

// There suppose to be faster once: https://chessprogramming.wikispaces.com/BitScan
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

int nextBitScanForward(uint64_t *bb, uint64_t *p) {
	*p = (*bb) & -(*bb);	//LS1B
	*bb &= (*bb - 1);	//LS1B_reset
	return 0;
}

void nextBitScanForwards(uint64_t *bb, uint64_t *p) {
	*p = (*bb) & -(*bb);	//LS1B
	*bb &= (*bb - 1);	//LS1B_reset
}

void bitScanForward(uint64_t bb, int *p) {
	while (bb > 0) {
		*(p++) = bitScanForward(bb);  //LS1B
		bb &= (bb - 1);	  //LS1B_reset
	}
	*(p++) = -1;
}

void LS1B(uint64_t bb, uint64_t *p) {
	while (bb > 0) {
		*(p++) = bb & -bb;  //LS1B
		bb &= (bb - 1);	  //LS1B_reset
	}
	*(p++) = 0;
}

void findFirstSet(uint64_t num) {
	// https://en.wikipedia.org/wiki/Find_first_set
	// https://www.go4expert.com/articles/builtin-gcc-functions-builtinclz-t29238/
	int clz = 0;
	int ctz = 0;
	int ones = 0;

	clz = __builtin_clz(num);

	printf("Number of leading zero's in %I64u is %d\n", num, clz);

	clz = __builtin_clz(-num);
	printf("Number of leading zero's in %I64u is %d\n", -num, clz);

	ctz = __builtin_ctz(num);
	printf("Number of trailing zero's in %I64u is %d\n", num, ctz);

	ones = __builtin_popcount(num);
	printf("Number of one's in %I64u is %d\n", num, ones);
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
//uint64_t arrKingAttacks[64];

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
}/*

void kingAttacks() {
	for (int sq = 0; sq < 64; sq++) {
		arrKingAttacks[sq] = kingAttacks((uint64_t) 1 << sq);
	}
}*/
/*
uint64_t kingAttacks(ChessboardIO::enumSquare sq) {
	return arrKingAttacks[sq];
}*/
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

struct Move {
	uint8_t from;
	uint8_t to;
	CBoard::EnumPiece type;
	bool castleShort;
	bool castleLong;
	bool passant; //Takes a unccupied square;

/*
	uint64_t bitMask(int sq) {
		return (uint64_t) 1 << sq;
	}

	// For every move???
	Move() {
		for (int sq = 0; sq < 64; sq++) {
			bitMaskEx[sq] = bitMask(sq);
		}
	}
*/
	void add(uint8_t from, uint8_t to, CBoard::EnumPiece type) {
		this->from = from;
		this->to = to;
		this->type = type;

	}

	void print() {
		cout << ChessboardIO::getSquare(from) << "-" << ChessboardIO::getSquare(to) << " piece: " << type << endl;
	}
};

/*
U64 CBoard::attacksToKing(enumSquare squareOfKing, enumColor colorOfKing) {
   U64 opPawns, opKnights, opRQ, opBQ;
   opPawns     = pieceBB[nBlackPawn   - colorOfKing];
   opKnights   = pieceBB[nBlackKnight - colorOfKing];
   opRQ = opBQ = pieceBB[nBlackQueen  - colorOfKing];
   opRQ       |= pieceBB[nBlackRook   - colorOfKing];
   opBQ       |= pieceBB[nBlackBishop - colorOfKing];
   return (arrPawnAttacks[colorOfKing][squareOfKing] & opPawns)
        | (arrKnightAttacks[squareOfKing]            & opKnights)
        | (bishopAttacks (occupiedBB, squareOfKing)  & opBQ)
        | (rookAttacks   (occupiedBB, squareOfKing)  & opRQ);
}
*/

bool validateSq(CBoard* cboard, Piece** pieceArray, int sq) {
	uint64_t knightAttacks = pieceArray[1]->attacks(cboard, sq);
	uint64_t bishopAttacks = pieceArray[4]->attacks(cboard, sq);
	uint64_t rookAttacks = pieceArray[3]->attacks(cboard, sq);
	uint64_t queenAttacks = bishopAttacks | rookAttacks;
	uint64_t enemyOcc = cboard->getPieceSet(CBoard::occ, cboard->getEnemyColor());

	if ((knightAttacks | queenAttacks) & enemyOcc) {
		cout << "potentiallyCheck" << endl;

		uint64_t blackKnights = cboard->getPieceSet(CBoard::nKnight, CBoard::black);
		uint64_t blackBishops = cboard->getPieceSet(CBoard::nBishop, CBoard::black);
		uint64_t blackRooks = cboard->getPieceSet(CBoard::nRook, CBoard::black);
		uint64_t blackQueen = cboard->getPieceSet(CBoard::nQueen, CBoard::black);

		cout << "blackKnights " << (bool) ((knightAttacks & blackKnights)) << endl;
		cout << "blackBishops " << (bool) ((bishopAttacks & blackBishops)) << endl;
		cout << "blackRooks " << (bool) ((rookAttacks & blackRooks)) << endl;
		cout << "blackQueens " << (bool) ((queenAttacks & blackQueen)) << endl;

		return (knightAttacks & blackKnights)
				| (bishopAttacks & blackBishops)
				| (rookAttacks & blackRooks)
				| (queenAttacks & blackQueen);

	}
	return false;
}


bool check(CBoard* cboard, Piece** pieceArray) {
	uint64_t king64 = cboard->getPieceSet(CBoard::nKing, CBoard::white);
	int kingSq = __builtin_ctzll(king64);
	return validateSq(cboard, pieceArray, kingSq);
}

bool castleDisabled = false;
bool shortCastleDisabled = false;
bool longCastelDisabled = false;
// White castle constants
uint64_t whiteCastleKing = (uint64_t) 1 << 4;

// Black castle constants
uint64_t blackCastleKing = (uint64_t) 1 << 4;

// Short white castle constants.
uint64_t whiteShortCastle = 0b11 << 1;
uint64_t whiteShortCastleRook = (uint64_t) 1;

// Long white castle constants.
uint64_t whiteLongCastle = 0b11 << 1;
uint64_t whiteLongCastleRook = (uint64_t) 1;

// Short black castle constants.
uint64_t blackShortCastle = 0b11 << 1;
uint64_t blackShortCastleRook = (uint64_t) 1;

// Long black castle constants.
uint64_t blackLongCastle = 0b11 << 1;
uint64_t blackLongCastleRook = (uint64_t) 1;

//boolm castle()

bool checkOccSquares(uint64_t *sqrs,  Piece** pieceArray, CBoard* cboard) {
	while (*sqrs) {
		int sq = __builtin_ctzll(*sqrs);
		if (validateSq(cboard, pieceArray, sq)) {
			return true;
		}
		*sqrs &= (*sqrs - 1);
	}
	return false;
}

bool castleWhite(CBoard* cboard, Piece** pieceArray) {
	// castleDisabled --> return

	uint64_t whiteKingPosition = cboard->getPieceSet(CBoard::nKing, CBoard::white);
	bool whiteKingCastleHeuristics = whiteKingPosition & whiteCastleKing; // castleDisabled = true;
		//whiteKingCastleHeuristics == false --> No castle allowed.
		//castleDisabled = true;

	uint64_t whiteRook = cboard->getPieceSet(CBoard::nRook, CBoard::white);
	bool whiteShortCastleHeuristics = whiteRook & whiteShortCastleRook; // shortCastleDisabled = true;
		//whiteShortCastleHeuristics == false --> No short castle allowed
		//shortCastleDisabled = true;

	bool whiteLongCastleHeuristics = whiteRook & whiteShortCastleRook; // shortCastleDisabled = true;
		//whiteLongCastleHeuristics == false --> No short castle allowed
		//longCastleDisabled = true;

	uint64_t occ = cboard->getPieceSet(CBoard::occ);

	if (!(occ & whiteShortCastle)) {
		uint64_t criticalSquaresShort = whiteCastleKing | whiteShortCastle;
		checkOccSquares(&criticalSquaresShort, pieceArray, cboard);
	}
	// else --> No short castle allowed

	if (!(occ & whiteLongCastle)) {
		uint64_t criticalSquaresLong = whiteCastleKing | whiteLongCastle;
		checkOccSquares(&criticalSquaresLong, pieceArray, cboard);
	}
	// else --> No long castle allowed
}



//Test by print all moves
void generateMoves(CBoard *cboard/*, Piece *pieceArray[]*/) {

	//Piece *pieceArray[] = {new Pawn(), new Knight()/*, new King(), new Rook(), new Bishop(), new Queen()*/ };
	/*pieceMap[CBoard::nKnight] = new Knight();
	pieceMap[CBoard::nBishop] = new Bishop();
	pieceMap[CBoard::nRook] = new Rook();
	pieceMap[CBoard::nQueen] = new Queen();
	pieceMap[CBoard::nKing] = new King();*/
	Piece** pieceArray = new Piece*[6];
	pieceArray[0] = new Pawn();
	pieceArray[1] = new Knight();
	pieceArray[2] = new King();
	pieceArray[3] = new Rook();
	pieceArray[4] = new Bishop();
	pieceArray[5] = new Queen();
	pieceArray[6] = new Castle();
	//pieceArray[7] = new Passant();

	// --- CRITICAL SQUARES ---
	// Check!
	bool isCheck = check(cboard, pieceArray);
	cout << "Is check: " << isCheck << endl;

	Move moves[256];
	uint8_t index = 0;
	for (int i = 0; i < 7; i++) {
		CBoard::EnumPiece type = pieceArray[i]->type();
		uint64_t pieces = cboard->getPieceSet(type, cboard->friendlyColor);

		while (pieces) {
	//		if (i == 6)
				ChessboardIO::printBigBoard(pieces);

			int sq = __builtin_ctzll(pieces);
			pieces &= (pieces - 1);
			uint64_t attacks = pieceArray[i]->attacks(cboard, sq);
			// if type = rook, sq = rooksq, attacks = 0x111;
			// if type = king, sq = ok, attacks = 0x0111;
			attacks &= cboard->getEmpty(cboard->friendlyColor);
			if (i == 6)
				ChessboardIO::printBigBoard(pieces);
			while (attacks > 0) {
				if (i == 6)
					ChessboardIO::printBigBoard(attacks);
				int attack = __builtin_ctzll(attacks);
				attacks &= (attacks - 1);
				moves[index++].add(sq, attack, type);
			}
		}

	}

	cout << "Moves available:" << endl;
	for (int i = 0; i < index; i++) {
		moves[i].print();
	}

	for (int i = 0; i < 7; i++) {
		delete pieceArray[i];
	}

	delete[] pieceArray;
/*
	std::map<CBoard::EnumPiece, Piece*> pieceMap;
	pieceMap[CBoard::nPawn] = new Pawn();
	pieceMap[CBoard::nKnight] = new Knight();
	for (int piece = CBoard::nPawn; piece != CBoard::nBishop; piece++) {
		CBoard::EnumPiece type = static_cast<CBoard::EnumPiece>(piece);
		uint64_t pieces = cboard->getPieceSet(type, color);
		while (pieces > 0) {
			int sq = __builtin_ctzll(pieces);
			pieces &= (pieces - 1);
			uint64_t attacks = pieceMap[(CBoard::EnumPiece)piece]->attacks(cboard, sq);
			while (attacks > 0) {
				int attack = __builtin_ctzll(attacks);
				attacks &= (attacks - 1);
				cout << ChessboardIO::getSquare(sq) << "-" << ChessboardIO::getSquare(attack) << endl;
			}
		}
	}*/
}

void functionTests() {
	createLookUpAttacks();

	ChessboardIO::enumSquare enumSquares[] = {
			ChessboardIO::b2,
			ChessboardIO::d2,
			ChessboardIO::f3,
			ChessboardIO::end };

	uint64_t board = ChessboardIO::setBoard(enumSquares);
	ChessboardIO::printBigBoard(board);
	uint64_t occ = 0b01000100;
	ChessboardIO::printBigBoard(occ);
	uint64_t attack = rankAttacks(occ, ChessboardIO::e1);
	ChessboardIO::printBigBoard(attack);
	uint64_t o = 0b0001000000010000000000000000000000000000000100000001000000010000;

	printMask(occ);
	printQueenAttacks(o);
	printPawnAttacks();
	printMovesByMatrix(board);
	printPicesBySquare(board);
	swapTest();
	findFirstSet(o);
}

/*
ostream & operator<<(ostream & out, const Pawn & point) {
   out << "(" << "point.x" << "," << "point.y" << ")";  // access private data
   return out;
}*/

// http://wtharvey.com/
int main() {
	//functionTests();

	ChessboardIO::enumSquare enumPawnSquares[] = {
			ChessboardIO::b2,
			ChessboardIO::d2,
			ChessboardIO::f3,
			ChessboardIO::end };

	ChessboardIO::enumSquare enumKnightSquares[] = {
			ChessboardIO::c4,
			ChessboardIO::end };

	ChessboardIO::enumSquare enumKingSquares[] = {
			ChessboardIO::e1,
			ChessboardIO::end };

	ChessboardIO::enumSquare enumRookSquares[] = {
			ChessboardIO::h1,
			ChessboardIO::a1,
			ChessboardIO::end };

	ChessboardIO::enumSquare enumBishopSquares[] = {
			ChessboardIO::e6,
			ChessboardIO::end };

	ChessboardIO::enumSquare enumQueenSquares[] = {
			ChessboardIO::c7,
			ChessboardIO::end };

	CBoard *cboard = new CBoard();

	cboard->setPieceSet(ChessboardIO::setBoard(enumPawnSquares), CBoard::nPawn, CBoard::white);
	cboard->setPieceSet(ChessboardIO::setBoard(enumKnightSquares), CBoard::nKnight, CBoard::white);
	cboard->setPieceSet(ChessboardIO::setBoard(enumKingSquares), CBoard::nKing, CBoard::white);
	cboard->setPieceSet(ChessboardIO::setBoard(enumQueenSquares), CBoard::nCastle, CBoard::white);
	cboard->setPieceSet(ChessboardIO::setBoard(enumRookSquares), CBoard::nRook, CBoard::white);
	cboard->setPieceSet(ChessboardIO::setBoard(enumBishopSquares), CBoard::nBishop, CBoard::white);
	cboard->setPieceSet(ChessboardIO::setBoard(enumQueenSquares), CBoard::nQueen, CBoard::white);
	cboard->friendlyColor = CBoard::white;
	generateMoves(cboard);
	ChessboardIO::printBigBoard(cboard->getPieceSet(CBoard::occ));

	delete cboard;

	return 0;
}
