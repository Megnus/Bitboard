/*
 * piece.h
 *
 *  Created on: 8 maj 2018
 *      Author: msundstr
 */

#include "cboard.h"
#ifndef PIECE_H_
#define PIECE_H_

class Piece {
	public:
		Piece();

		virtual ~Piece();

		virtual uint64_t attacks(CBoard *cboard, uint64_t *origin) = 0;
};

#endif /* PIECE_H_ */
