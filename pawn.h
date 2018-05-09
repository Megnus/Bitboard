/*
 * pawn.h
 *
 *  Created on: 8 maj 2018
 *      Author: msundstr
 */
#include "piece.h"
#ifndef PAWN_H_
#define PAWN_H_

class Pawn : public Piece {
public:
	Pawn();
	virtual ~Pawn();
	uint64_t attacks(CBoard *cboard, int origin) {
		return 1;
	}

};

#endif /* PAWN_H_ */
