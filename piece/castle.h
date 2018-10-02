/*
 * castle.h
 *
 *  Created on: 18 sep. 2018
 *      Author: msundstr
 */
#include "piece.h"
#ifndef PIECE_CASTLE_H_
#define PIECE_CASTLE_H_

class Castle: virtual public Piece {
private:
	bool castleEnable[2];
	bool castleShortEnable[2];
	bool castleLongEnable[2];

	uint64_t castleKingPosition[2];
	uint64_t castleShortRookPosition[2];
	uint64_t castleLongRookPosition[2];
	uint64_t castleShortEmptySquares[2];
	uint64_t castleLongEmptySquares[2];
	uint64_t castleSquares[2];

	uint64_t castleKingShortEndPosition[2];
	uint64_t castleKingLongEndPosition[2];

	uint64_t castleKingAttackPosition[2];

	void castleConstants() {
		castleEnable[CBoard::white] = true;
		castleEnable[CBoard::black] = true;
		castleShortEnable[CBoard::white] = true;
		castleShortEnable[CBoard::black] = true;
		castleLongEnable[CBoard::white] = true;
		castleLongEnable[CBoard::black] = true;

		castleKingPosition[CBoard::white] = (uint64_t) 1 << 4;
		castleKingPosition[CBoard::black] = (uint64_t) 1 << 59;

		castleKingShortEndPosition[CBoard::white] = (uint64_t) 1 << 4;
		castleKingShortEndPosition[CBoard::black] = (uint64_t) 1 << 59;
		castleKingLongEndPosition[CBoard::white] = (uint64_t) 1 << 4;
		castleKingLongEndPosition[CBoard::black] = (uint64_t) 1 << 59;

		castleKingAttackPosition[CBoard::white] = (uint64_t) 0b01001000;
		castleKingAttackPosition[CBoard::black] = (uint64_t) 0b01001000 << 55;

		castleShortRookPosition[CBoard::white] = (uint64_t) 1;
		castleShortRookPosition[CBoard::black] = (uint64_t) 56;
		castleLongRookPosition[CBoard::white] = (uint64_t) 1;
		castleLongRookPosition[CBoard::black] = (uint64_t) 64;

		castleShortEmptySquares[CBoard::white] = (uint64_t) 0b11 << 2;
		castleShortEmptySquares[CBoard::black] = (uint64_t) 0b11 << 57;
		castleLongEmptySquares[CBoard::white] = (uint64_t) 0b111 << 5;
		castleLongEmptySquares[CBoard::black] = (uint64_t) 0b111 << 60;
	}

	uint64_t arrCastleAttacks[64][2];
	uint64_t bitMaskEx[64];

	uint64_t bitMask(int sq) {
		return (uint64_t) 1 << sq;
	}

	uint64_t fileMask(int sq) {
		return uint64_t(0x0101010101010101) << (sq & 7);
	}

	void castleAttacks() {
		for (int sq = 0; sq < 64; sq++) {
			for (int color = 0; color < 2; color++) {
				if (bitMaskEx[sq] == castleKingPosition[color]) {
					arrCastleAttacks[sq][color] = castleKingAttackPosition[color];
				}
			}
		}
	}

	void fillArrMask64() {
		for (int sq = 0; sq < 64; sq++) {
			bitMaskEx[sq] = bitMask(sq);
		}
	}

public:
	Castle() {
		fillArrMask64();
	}

	virtual ~Castle();

	uint64_t attacks(CBoard *cboard, int sq) {
		uint64_t castle = 0;

		if (castleEnable[cboard->friendlyColor] && arrCastleAttacks[sq][cboard->friendlyColor] == 0) {
			if ((cboard->getPieceSet(CBoard::nKing) ^ bitMaskEx[sq]) == 0) {
				castleEnable[cboard->friendlyColor] = false;
				return castle;
			}
		} else {
			return castle;
		}

		if (castleShortEnable[cboard->friendlyColor]) {
			if ((cboard->getPieceSet(CBoard::nRook) ^ castleShortRookPosition[cboard->friendlyColor]) != 0) {
				if ((cboard->getPieceSet(CBoard::occ) ^ castleShortEmptySquares[cboard->friendlyColor]) == 0) {
					castle |= castleKingShortEndPosition[cboard->friendlyColor];
				}
			} else {
				castleShortEnable[cboard->friendlyColor] = false;
			}
		}

		if (castleLongEnable[cboard->friendlyColor]) {
			if ((cboard->getPieceSet(CBoard::nRook) ^ castleLongRookPosition[cboard->friendlyColor]) != 0) {
				if ((cboard->getPieceSet(CBoard::occ) ^ castleLongEmptySquares[cboard->friendlyColor]) == 0) {
					castle |= castleKingLongEndPosition[cboard->friendlyColor];
				}
			} else {
				castleLongEnable[cboard->friendlyColor] = false;
			}
		}

		return castle;
	}

	CBoard::EnumPiece type() {
		cout << "nCastle"  << endl;
		return CBoard::nCastle;
	}

	string toString() {
		return "castle";
	}
};


#endif /* PIECE_CASTLE_H_ */
