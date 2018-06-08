/*
 * bishop.h
 *
 *  Created on: 7 juni 2018
 *      Author: msundstr
 */

#include "piece.h"
#ifndef PIECE_BISHOP_H_
#define PIECE_BISHOP_H_

class Bishop: virtual public Piece {
private:
	uint8_t arrFirstRankAttacks64x8[64 * 8]; // 512 Bytes = 1/2KByte
	uint64_t diagonalMaskEx[64];
	uint64_t antidiagMaskEx[64];
	uint64_t bitMaskEx[64];

	uint64_t bitMask(int sq) {
		return (uint64_t) 1 << sq;
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

	void fillArrMask64() {
		for (int sq = 0; sq < 64; sq++) {
			bitMaskEx[sq] = bitMask(sq);
			diagonalMaskEx[sq] = diagonalMask(sq);
			antidiagMaskEx[sq] = antiDiagMask(sq);
		}
	}

	uint64_t antiDiagAttacks(uint64_t occ, int sq) {
		uint64_t forward, reverse;
		forward  = occ & antidiagMaskEx[sq];
		reverse  = __bswap_constant_64(forward);
		forward -= bitMaskEx[sq];
		reverse -= __bswap_constant_64(bitMaskEx[sq]);
		forward ^= __bswap_constant_64(reverse);
		forward &= antidiagMaskEx[sq];
		return forward;
	}

	uint64_t diagonalAttacks(uint64_t occ, int sq) {
		uint64_t forward, reverse;
		forward = occ & diagonalMaskEx[sq];
		reverse  = __bswap_constant_64(forward);
		forward -= bitMaskEx[sq];
		reverse -= __bswap_constant_64(bitMaskEx[sq]);
		forward ^= __bswap_constant_64(reverse);
		forward &= diagonalMaskEx[sq];
		return forward;
	}

public:
	Bishop() {
		fillArrMask64();
	}

	virtual ~Bishop();

	uint64_t attacks(CBoard *cboard, int sq) {
		uint64_t occ = cboard->getPieceSet(CBoard::occ) ^ bitMaskEx[sq];
		uint64_t att = antiDiagAttacks(occ, sq) | diagonalAttacks(occ, sq);
		ChessboardIO::printBigBoard(att);
		return att;
	}

	CBoard::EnumPiece type() {
		cout << "nBishop"  << endl;
		return CBoard::nBishop;
	}
};

#endif /* PIECE_BISHOP_H_ */
