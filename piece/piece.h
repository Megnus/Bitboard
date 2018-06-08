/*
 * piece.h
 *
 *  Created on: 8 maj 2018
 *      Author: msundstr
 */
#include "..\byteswap.h"
#include "..\cboard.h"
#ifndef PIECE_H_
#define PIECE_H_

class Piece {
protected:
	uint64_t bitMaskEx[64];

	uint64_t bitMask(int sq) {
		return (uint64_t) 1 << sq;
	}

public:
	Piece() {
		for (int sq = 0; sq < 64; sq++) {
			bitMaskEx[sq] = bitMask(sq);
		}
	}

	virtual ~Piece();

	//virtual uint64_t attacks(CBoard *cboard, uint64_t *origin) = 0;
	virtual uint64_t attacks(CBoard *cboard, int sq) = 0;

	virtual CBoard::EnumPiece type() = 0;
};

#endif /* PIECE_H_ */
