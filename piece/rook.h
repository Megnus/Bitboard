/*
 * rook.h
 *
 *  Created on: 4 juni 2018
 *      Author: msundstr
 */

#include "piece.h"
#ifndef PIECE_ROOK_H_
#define PIECE_ROOK_H_

class Rook: virtual public Piece {
private:
	uint8_t arrFirstRankAttacks64x8[64 * 8]; // 512 Bytes = 1/2KByte
	uint64_t fileMaskEx[64];
	//uint64_t bitMaskEx[64];

	uint64_t bitMask(int sq) {
		return (uint64_t) 1 << sq;
	}

	uint64_t fileMask(int sq) {
		return uint64_t(0x0101010101010101) << (sq & 7);
	}

	void fillArrFirstRankAttacks64x8() {
		for (uint64_t occ64 = 0; occ64 < 255; occ64++) {
			for (int sq = 0; sq < 8; sq++) {
				if ((occ64 & bitMaskEx[sq]) == 0) {
					int swapSq = Tool::swapSqByflipDiagA1H8((int) sq);
					uint64_t attacks = Tool::flipDiagA1H8(occ64);
					attacks = fileAttacks(attacks, swapSq);
					attacks = Tool::flipDiagA1H8(attacks);
					unsigned int file = sq & 7;
					unsigned int rkx8 = sq & 56;
					uint8_t occ8 = (occ64 >> rkx8) & 2 * 63;
					arrFirstRankAttacks64x8[4 * occ8 + file] = (uint8_t) attacks;
				}
			}
		}
	}

	void fillArrMask64() {
		for (int sq = 0; sq < 64; sq++) {
			//bitMaskEx[sq] = bitMask(sq);
			fileMaskEx[sq] = fileMask(sq);
		}
	}

	uint64_t fileAttacks(uint64_t occ, int sq) {
		uint64_t forward, reverse;
		forward = occ & fileMaskEx[sq];
		reverse = __bswap_constant_64(forward);
		forward -= bitMaskEx[sq];
		reverse -= __bswap_constant_64(bitMaskEx[sq]);
		forward ^= __bswap_constant_64(reverse);
		forward &= fileMaskEx[sq];
		return forward;
	}

	uint64_t rankAttacks(uint64_t occ, int sq) {
		unsigned int file = sq & 7;
		unsigned int rkx8 = sq & 56;
		occ = (occ >> rkx8) & 2 * 63;
		uint64_t attacks = arrFirstRankAttacks64x8[4 * occ + file];
		return attacks << rkx8;
	}

public:
	Rook() {
		fillArrMask64();
		fillArrFirstRankAttacks64x8();
	}

	virtual ~Rook();

	uint64_t attacks(CBoard *cboard, int sq) {
		uint64_t occ = cboard->getPieceSet(CBoard::occ) ^ bitMaskEx[sq];
		return fileAttacks(occ, sq) | rankAttacks(occ, sq);
	}

	CBoard::EnumPiece type() {
		cout << "nRook"  << endl;
		return CBoard::nRook;
	}
};

#endif /* PIECE_ROOK_H_ */
