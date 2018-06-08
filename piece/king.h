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
};

#endif /* KING_H_ */
