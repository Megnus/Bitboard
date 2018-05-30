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
	uint64_t whitePawnAttacks(uint64_t empty, uint64_t blackPieces, uint64_t whitePawns) {
		uint64_t singlePushs = Tool::nortOne(whitePawns) & empty;
		uint64_t attacks = Tool::noEaOne(whitePawns) & blackPieces;
		attacks |= Tool::noWeOne(whitePawns) & blackPieces;
		attacks |= singlePushs;
		attacks |= Tool::nortOne(singlePushs) & empty & Tool::rank4;
		return attacks;
	}

	uint64_t blackPawnAttacks(uint64_t empty, uint64_t whitePieces, uint64_t blackPawns) {
		uint64_t singlePushs = Tool::soutOne(blackPawns) & empty;
		uint64_t attacks = Tool::soEaOne(blackPawns) & whitePieces;
		attacks |= Tool::soWeOne(blackPawns) & whitePieces;
		attacks |= singlePushs;
		attacks |= Tool::soutOne(singlePushs) & empty & Tool::rank5;
		return attacks;
	}

public:
	Pawn();

	virtual ~Pawn();

	uint64_t attacks(CBoard *cboard, int sq) {
		if (cboard->color == CBoard::white) {
			return whitePawnAttacks(~cboard->getPieceSet(CBoard::occ),
					cboard->getPieceSet(CBoard::occ, CBoard::black),
					(uint64_t) 1 << sq);
		}

		return blackPawnAttacks(~cboard->getPieceSet(CBoard::occ),
				cboard->getPieceSet(CBoard::occ, CBoard::white),
				(uint64_t) 1 << sq);
	}

	CBoard::EnumPiece type() {
		return CBoard::nPawn;
	}

	/*
	 uint64_t attacks(CBoard *cboard, uint64_t *origin) {
	 if (cboard->color == CBoard::white) {
	 return whitePawnAttacks(
	 cboard->getPieceSet(CBoard::occ, CBoard::white),
	 cboard->getPieceSet(CBoard::occ, CBoard::black),
	 *origin);
	 }

	 return blackPawnAttacks(
	 cboard->getPieceSet(CBoard::occ, CBoard::white),
	 cboard->getPieceSet(CBoard::occ, CBoard::black),
	 *origin);
	 }*/
};

#endif /* PAWN_H_ */
