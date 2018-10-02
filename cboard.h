/*
 * CBoard.h
 *
 *  Created on: 26 apr. 2018
 *      Author: 	msundstr
 *      Url: 		https://chessprogramming.wikispaces.com/Bitboard%20Board-Definition
 */
//#include <iostream>
////#include <iostream>
//#include <stdio.h>#include <stdio.h>
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
		nWhite,
		nBlack,
		occ,
		nPawn,
		nKnight,
		nBishop,
		nRook,
		nQueen,
		nKing,
		nCastle
	};

	enum CastleType {
		none,
		shortCastle,
		longCastle
	};

	ColorType friendlyColor;
	ColorType enemyColor;

//	virtual ColorType getOppositeColor()
//
//	virtual ColorType getColor();
//https://flir.app.box.com
//	virtual void switchColor();

		//uint64_t shortCastle = 0b01;
		//uint64_t longCastle = 0b01;
		uint64_t shortCastle64[2];
		uint64_t longCastle64[2];
	// SpecialMove.cpp

	// Valid Passant
	// Extract valid passant

	// -- Valid short castle
	// Short castle is still valid
	// Rook on place
	// King on place
	// Empty squares correct
	// King has not been moved
	// Rook has not been moved

	// -- Valid long castle
	// Long castle is still valid
	// Rook on place
	// King on place
	// Empty squares correct
	// King has not been moved
	// Rook has not been moved

	ColorType getFriendlyColor() {
		return this->friendlyColor;
	}

	ColorType getEnemyColor() {
		return this->enemyColor;
	}

	void toggleColor() {
		if (this->friendlyColor == white) {
			this->friendlyColor = black;
			this->enemyColor = white;
		} else {
			this->friendlyColor = white;
			this->enemyColor = black;
		}
	}

	uint64_t getShortCastleSet(ColorType color) {
		return shortCastle64[(int)color];
	}

	void setPieceSet(uint64_t u64, EnumPiece piece, ColorType color)  {
		pieceBB[color] |= u64;
		pieceBB[piece] |= u64;
		pieceBB[occ] |= u64;
	}

	void setPieceSet(ChessboardIO::enumSquare squares[], EnumPiece piece, ColorType color) {
		uint64_t u64 = ChessboardIO::setBoard(squares);
		setPieceSet(u64, piece, color);
	}

	uint64_t getEmpty(ColorType color) const {
		return ~pieceBB[color];
	}

	uint64_t getEmpty() const {
		return ~pieceBB[occ];
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

	virtual ~CBoard() {
		cout << "CBoard deleted" << endl;
	}
};
// Ny pjäs vi slutet 1 och 8 ranken.
// Rockad
// Passant

// FEN notation
/* lookup för rookad
 * 0-0 		: Kung ej rörd. Torn på h1 ej rörd. e1, f1, g1 måste vara tomma. e1, f1, g1 kan ej behärskans av motsåndarens pjäser.
 * 0-0-0 	: Kung ej rörd. Torn på a1 ej rörd. e1, d1, c1, b1 måste vara tomma. e1, d1, c1 kan ej behärskans av motsåndarens pjäser.
- kungen får inte stå i schack; man kan alltså inte undkomma en schack genom att rockera
- varken kungen eller det torn som används för rockaden får ha flyttats tidigare under partiet
- inget fält mellan kungen och tornet får vara besatt av en annan pjäs; det får alltså inte stå någon annan pjäs emellan dem, oavsett färg
- inget av de fält som kungen rör sig över, eller hamnar på, får vara hotat av någon av motståndarens pjäser; man kan alltså inte flytta in i schack
(Rockaden räknas som ett kungsdrag)
 */

#endif /* CBOARD_H_ */
