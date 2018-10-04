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
	enum CastleType {
		Long,
		Short,
		Both
	};

	enum PositionType {
		Start,
		End
	};

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

	bool castleEnable[2][3];
	uint64_t castleKingPosition[2][2][2];
	uint64_t castleRookPosition[2][2][2];
	uint64_t castleEmptySquares[2][2];
	uint64_t castleSquares[2][2];

	void castelConstants() {
		castleEnable[CBoard::white][Both] = true;
		castleEnable[CBoard::black][Both] = true;
		castleEnable[CBoard::white][Short] = true;
		castleEnable[CBoard::black][Short] = true;
		castleEnable[CBoard::white][Long] = true;
		castleEnable[CBoard::black][Long] = true;

		castleKingPosition[CBoard::white][Both][Start] = (uint64_t) 1 << 4;
		castleKingPosition[CBoard::black][Both][Start]  = (uint64_t) 1 << 60;

		castleKingPosition[CBoard::white][Short][End] = (uint64_t) 1 << 6;
		castleKingPosition[CBoard::black][Short][End] = (uint64_t) 1 << 62;
		castleKingPosition[CBoard::white][Long][End] = (uint64_t) 1 << 2;
		castleKingPosition[CBoard::black][Long][End] = (uint64_t) 1 << 58;

		castleRookPosition[CBoard::white][Short][Start] = (uint64_t) 1 << 7;
		castleRookPosition[CBoard::black][Short][Start] = (uint64_t) 1 << 63;
		castleRookPosition[CBoard::white][Long][Start] = (uint64_t) 1 << 0;
		castleRookPosition[CBoard::black][Long][Start] = (uint64_t) 1 << 56;

		castleRookPosition[CBoard::white][Short][End] = (uint64_t) 1 << 5;
		castleRookPosition[CBoard::black][Short][End] = (uint64_t) 1 << 61;
		castleRookPosition[CBoard::white][Long][End] = (uint64_t) 1 << 3;
		castleRookPosition[CBoard::black][Long][End] = (uint64_t) 1 << 59;

		castleEmptySquares[CBoard::white][Short] = (uint64_t) 0b111 << 1;
		castleEmptySquares[CBoard::black][Short] = (uint64_t) 0b111 << 57;
		castleEmptySquares[CBoard::white][Long] = (uint64_t) 0b11 << 5;
		castleEmptySquares[CBoard::black][Long] = (uint64_t) 0b11 << 61;
	}

	uint64_t arrKingPosition[64][2];

	void castleAttacks() {
		for (int sq = 0; sq < 64; sq++) {
			for (int color = 0; color < 2; color++) {
				if (bitMaskEx[sq] == castleKingPosition[color][Both][Start]) {
					arrKingPosition[sq][color] = castleKingPosition[color][Both][Start];
				}
			}
		}
	}

public:
	King() {
		castelConstants();
		castleAttacks();
		kingAttacks();
	}

	virtual ~King();

	uint64_t attacks(CBoard *cboard, int sq) {
		uint64_t attack64 = arrKingAttacks[sq] | castleAttacks(cboard, sq);
		ChessboardIO::printBigBoard(attack64);
		return attack64; //arrKingAttacks[sq] | castleAttacks(cboard, sq);
	}

	CBoard::EnumPiece type() {
		cout << "nKing"  << endl;
		return CBoard::nKing;
	}

	string toString() {
		return "king";
	}

	uint64_t castleAttacks(CBoard::ColorType color, CastleType type, uint64_t occ, uint64_t rooks) {
		if (castleEnable[color][type]) {
			if (rooks & castleRookPosition[color][type][Start]) {
				if (!(occ & castleEmptySquares[color][type])) {
					return castleKingPosition[color][type][End];
				}
			} else {
				castleEnable[color][type] = false;
			}
		}
		return 0;
	}

	uint64_t castleAttacks(CBoard *cboard, int sq) {
		CBoard::ColorType color = cboard->friendlyColor;
		uint64_t occ = cboard->getPieceSet(CBoard::occ);
		uint64_t rooks = cboard->getPieceSet(CBoard::nRook);

		if (!(castleEnable[color][Both] && arrKingPosition[sq][color])) {
			castleEnable[color][Both] = false;
			return 0;
		}

		return castleAttacks(color, Short, occ, rooks) | castleAttacks(color, Long, occ, rooks);
	}
};

#endif /* KING_H_ */
