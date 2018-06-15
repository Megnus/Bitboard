/*
 * pawn.h
 *
 *  Created on: 8 maj 2018
 *      Author: msundstr
 */
#include "piece.h"
#ifndef PAWN_H_
#define PAWN_H_

class Pawn: public Piece {
private:
	//TODO Create lookup arrays for attacks and singlepush.
	uint64_t whitePawnAttacks(CBoard *cboard, int sq) {
		uint64_t occ = cboard->getPieceSet(CBoard::occ);
		uint64_t empty = ~occ;
		//uint64_t blackPieces = cboard->getPieceSet(CBoard::occ, CBoard::white);
		uint64_t sq64 = bitMaskEx[sq];
		uint64_t singlePushs = Tool::nortOne(sq64) & empty;
		uint64_t attacks = Tool::noEaOne(sq64) & occ/*blackPieces*/;
		attacks |= Tool::noWeOne(sq64) & occ/*blackPieces*/;
		attacks |= singlePushs;
		attacks |= Tool::nortOne(singlePushs) & empty & Tool::rank4;
		return attacks;
	}

	uint64_t blackPawnAttacks(CBoard *cboard, int sq) {
		uint64_t occ = cboard->getPieceSet(CBoard::occ);
		uint64_t empty = ~occ;
		//uint64_t whitePieces = cboard->getPieceSet(CBoard::occ, CBoard::white);
		uint64_t sq64 = bitMaskEx[sq];
		uint64_t singlePushs = Tool::soutOne(sq64) & empty;
		uint64_t attacks = Tool::soEaOne(sq64) & occ/*whitePieces*/;
		attacks |= Tool::soWeOne(sq64) & occ/*whitePieces*/;
		attacks |= singlePushs;
		attacks |= Tool::soutOne(singlePushs) & empty & Tool::rank5;
		return attacks;
	}

public:
	Pawn();

	virtual ~Pawn();

	uint64_t attacks(CBoard *cboard, int sq) {
		return cboard->color == CBoard::white ? whitePawnAttacks(cboard, sq) : blackPawnAttacks(cboard, sq);
	};

	CBoard::EnumPiece type() {
		cout << "nPawn"  << endl;
		return CBoard::nPawn;
	}

	string toString() {
		return "pawn";
	}
};

#endif /* PAWN_H_ */
