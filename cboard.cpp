/*
 * CBoard.cpp
 *
 *  Created on: 26 apr. 2018
 *      Author: msundstr
 */

#include "cboard.h"

CBoard::CBoard() {
	this->color = white;
	for (int index = 0; index < 12; index++) {
		pieceBB[index] = 0;
	}
}

CBoard::~CBoard() {
	// TODO Auto-generated destructor stub
}

