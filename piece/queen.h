/*
 * queen.h
 *
 *  Created on: 1 juni 2018
 *      Author: msundstr
 */

#include "rook.h"
#include "bishop.h"
#ifndef PIECE_QUEEN_H_
#define PIECE_QUEEN_H_

class Queen : public Bishop, public Rook {
public:
	Queen();

	virtual ~Queen();

	uint64_t attacks(CBoard *cboard, int sq) {
		return Rook::attacks(cboard, sq) | Bishop::attacks(cboard, sq);
	}

	CBoard::EnumPiece type() {
		cout << "nQueen"  << endl;
		return CBoard::nQueen;
	}
};

#endif /* PIECE_QUEEN_H_ */
