/*
 * CBoard.h
 *
 *  Created on: 26 apr. 2018
 *      Author: 	msundstr
 *      Url: 		https://chessprogramming.wikispaces.com/Bitboard%20Board-Definition
 */
#include <iostream>
#include <stdio.h>
#include "chessboardio.h"
#ifndef CBOARD_H_
#define CBOARD_H_

class CBoard {
private:
	uint64_t pieceBB[10];

public:
	enum ColorType {
		white,
		black
	};

	enum EnumPiece {
		occ,
		empty,
		nPawn,
		nKnight,
		nBishop,
		nRook,
		nQueen,
		nKing
	};

	//This is broken
	void setPieceSet(uint64_t u64, EnumPiece piece, ColorType color)  {
		pieceBB[piece] |= u64;
		pieceBB[occ + color] |= u64;
		pieceBB[empty] = ~ pieceBB[occ];
	}

	void setPieceSet(ChessboardIO::enumSquare squares[], EnumPiece piece, ColorType color) {
		uint64_t u64 = ChessboardIO::setBoard(squares);
		setPieceSet(u64, piece, color);
	}

	uint64_t getPieceSet(EnumPiece piece) const {
		return pieceBB[piece];
	}

	uint64_t getPieceSet(EnumPiece piece, ColorType color) const {
		return pieceBB[piece] & pieceBB[color];
	}

	uint64_t getWhitePawns() const {
		return pieceBB[nPawn] & pieceBB[white];
	}

	uint64_t getPawns(ColorType color) const {
		return pieceBB[nPawn] & pieceBB[color];
	}

	CBoard();
	virtual ~CBoard();


};

#endif /* CBOARD_H_ */
