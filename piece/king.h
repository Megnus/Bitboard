/*
 * king.h
 *
 *  Created on: 21 maj 2018
 *      Author: msundstr
 */

#include "piece.h"
#ifndef KING_H_
#define KING_H_

class King: public Piece {
private:
	bool castleEnable[2];
	bool castleShortEnable[2];
	bool castleLongEnable[2];

	uint64_t castleKingPosition[2];
	uint64_t castleShortRookPosition[2];
	uint64_t castleLongRookPosition[2];
	uint64_t castleShortEmptySquares[2];
	uint64_t castleLongEmptySquares[2];
	uint64_t castleShortOccSquares[2];
	uint64_t castleLongOccSquares[2];

	void castleConstants() {
		castleEnable[CBoard::white] = true;
		castleEnable[CBoard::black] = true;

		castleShortEnable[CBoard::white] = true;
		castleShortEnable[CBoard::black] = true;

		castleLongEnable[CBoard::white] = true;
		castleLongEnable[CBoard::black] = true;

		castleKingPosition[CBoard::white] = (uint64_t) 1 << 4;
		castleKingPosition[CBoard::black] = (uint64_t) 1 << 4;

		castleShortRookPosition[CBoard::white] = (uint64_t) 1;
		castleShortRookPosition[CBoard::black] = (uint64_t) 1;

		castleLongRookPosition[CBoard::white] = (uint64_t) 1;
		castleLongRookPosition[CBoard::black] = (uint64_t) 1;

		castleShortRookPosition[CBoard::white] = (uint64_t) 1;
		castleShortRookPosition[CBoard::black] = (uint64_t) 1;

		castleLongRookPosition[CBoard::white] = (uint64_t) 1;
		castleLongRookPosition[CBoard::black] = (uint64_t) 1;

		castleShortEmptySquares[CBoard::white] = (uint64_t) 0b111 << 1;
		castleShortEmptySquares[CBoard::black] = (uint64_t) 0b111 << 1;

		castleLongOccSquares[CBoard::white] = (uint64_t) 0b1111 << 5;
		castleLongOccSquares[CBoard::black] = (uint64_t) 0b1111 << 5;

		castleShortOccSquares[CBoard::white] = castleShortEmptySquares[CBoard::white];
		castleShortOccSquares[CBoard::black] = castleShortEmptySquares[CBoard::black];

		castleLongOccSquares[CBoard::white] = (uint64_t) 0b111 << 5;
		castleLongOccSquares[CBoard::black] = (uint64_t) 0b111 << 5;
	}

	uint64_t arrKingAttacks[64];

	uint64_t kingAttacks(uint64_t kingSet) {
		uint64_t attack = Tool::nortOne(kingSet);
		attack |= Tool::nortOne(kingSet);
		attack |= Tool::soutOne(kingSet);
		attack |= Tool::eastOne(kingSet);
		attack |= Tool::noEaOne(kingSet);
		attack |= Tool::soEaOne(kingSet);
		attack |= Tool::westOne(kingSet);
		attack |= Tool::soWeOne(kingSet);
		attack |= Tool::noWeOne(kingSet);
		return attack;
	}

	void kingAttacks() {
		for (int sq = 0; sq < 64; sq++) {
			arrKingAttacks[sq] = kingAttacks((uint64_t) 1 << sq);
		}
	}

public:
	King() {
		castleConstants();
		kingAttacks();
	}

	virtual ~King();

	uint64_t attacks(CBoard *cboard, int sq) {
		return arrKingAttacks[sq];
	}

	CBoard::EnumPiece type() {
		cout << "nKing"  << endl;
		return CBoard::nKing;
	}

	string toString() {
		return "king";
	}
};

#endif /* KING_H_ */
