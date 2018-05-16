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
	private:
		uint64_t whitePawnAttacks(uint64_t wb, uint64_t bb, uint64_t wPawns) {
			uint64_t empty = ~ (bb | wb);
			uint64_t singlePushs = Tool::nortOne(wPawns) & empty;
			uint64_t attacks = Tool::noEaOne(wPawns) & bb;
			attacks |= Tool::noWeOne(wPawns) & bb;
			attacks |= singlePushs;
			attacks |= Tool::nortOne(singlePushs) & empty & Tool::rank4;
			return attacks;
		}

		uint64_t blackPawnAttacks(uint64_t wb, uint64_t bb, uint64_t bPawns) {
			uint64_t empty = ~ (bb | wb);
			uint64_t singlePushs = Tool::soutOne(bPawns) & empty;
			uint64_t attacks = Tool::soEaOne(bPawns) & wb;
			attacks |= Tool::soWeOne(bPawns) & wb;
			attacks |= singlePushs;
			attacks |= Tool::soutOne(singlePushs) & empty & Tool::rank5;
			return attacks;
		}

	public:
		Pawn();

		virtual ~Pawn();

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
		}
};

#endif /* PAWN_H_ */
