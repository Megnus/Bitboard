/*
 * tool.h
 *
 *  Created on: 17 apr. 2018
 *      Author: msundstr
 */
#include <iostream>
#ifndef TOOL_H_
#define TOOL_H_

class Tool {
public:
	static uint64_t flipVertical(uint64_t x) {
		return    ((x << 56))
				| ((x << 40) & uint64_t(0x00ff000000000000))
				| ((x << 24) & uint64_t(0x0000ff0000000000))
				| ((x << 8)  & uint64_t(0x000000ff00000000))
				| ((x >> 8)  & uint64_t(0x00000000ff000000))
				| ((x >> 24) & uint64_t(0x0000000000ff0000))
				| ((x >> 40) & uint64_t(0x000000000000ff00))
				| ((x >> 56));
	}

	static uint64_t mirrorHorizontal(uint64_t x) {
		const uint64_t k1 = uint64_t(0x5555555555555555);
		const uint64_t k2 = uint64_t(0x3333333333333333);
		const uint64_t k4 = uint64_t(0x0f0f0f0f0f0f0f0f);
		x = ((x >> 1) & k1) | ((x & k1) << 1);
		x = ((x >> 2) & k2) | ((x & k2) << 2);
		x = ((x >> 4) & k4) | ((x & k4) << 4);
		return x;
	}

	static int swapSqByflipDiagA1H8(int sq) {
		return ((sq >> 3) | (sq << 3)) & 63;
	}

	static uint64_t flipDiagA1H8(uint64_t x) {
		uint64_t t;
		const uint64_t k1 = uint64_t(0x5500550055005500);
		const uint64_t k2 = uint64_t(0x3333000033330000);
		const uint64_t k4 = uint64_t(0x0f0f0f0f00000000);
		t = k4 & (x ^ (x << 28));
		x ^= t ^ (t >> 28);
		t = k2 & (x ^ (x << 14));
		x ^= t ^ (t >> 14);
		t = k1 & (x ^ (x << 7));
		x ^= t ^ (t >> 7);
		return x;
	}

	static uint64_t rotate180(uint64_t x) {
		return mirrorHorizontal(flipVertical(x));
	}
};

#endif /* TOOL_H_ */
