/*
 * knight.h
 *
 *  Created on: 21 maj 2018
 *      Author: msundstr
 */
#include "piece.h"
#ifndef KNIGHT_H_
#define KNIGHT_H_

class Knight: public Piece {
private:
	uint64_t arrKnightAttacks[64];

	uint64_t knightAttacks(uint64_t knightSet) {
		uint64_t l1 = (knightSet >> 1) & uint64_t(0x7f7f7f7f7f7f7f7f);
		uint64_t l2 = (knightSet >> 2) & uint64_t(0x3f3f3f3f3f3f3f3f);
		uint64_t r1 = (knightSet << 1) & uint64_t(0xfefefefefefefefe);
		uint64_t r2 = (knightSet << 2) & uint64_t(0xfcfcfcfcfcfcfcfc);
		uint64_t h1 = l1 | r1;
		uint64_t h2 = l2 | r2;
		return (h1 << 16) | (h1 >> 16) | (h2 << 8) | (h2 >> 8);
	}

	void knightAttacks() {
		for (int sq = 0; sq < 64; sq++) {
			arrKnightAttacks[sq] = knightAttacks((uint64_t) 1 << sq);
		}
	}

public:
	Knight();

	virtual ~Knight();

	uint64_t attacks(CBoard *cboard, int sq) {
		return arrKnightAttacks[sq];
	}

	CBoard::EnumPiece type() {
		cout << "nKnight"  << endl;
		return CBoard::nKnight;
	}

	string toString() {
		return "knight";
	}
};

#endif /* KNIGHT_H_ */
