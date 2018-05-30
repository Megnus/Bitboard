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
	/*private:
		static const int lsb_64_table[64];*/
	public:
//	const int lsb_64_table[64] = {
//		63, 30,  3, 32, 59, 14, 11, 33,
//		60, 24, 50,  9, 55, 19, 21, 34,
//		61, 29,  2, 53, 51, 23, 41, 18,
//		56, 28,  1, 43, 46, 27,  0, 35,
//		62, 31, 58,  4,  5, 49, 54,  6,
//		15, 52, 12, 40,  7, 42, 45, 16,
//		25, 57, 48, 13, 10, 39,  8, 44,
//		20, 47, 38, 22, 17, 37, 36, 26
//	};
//
//	// There suppose to be faster once: https://chessprogramming.wikispaces.com/BitScan
//	int bitScanForward(uint64_t bb) {
//		unsigned int folded;
//		assert (bb != 0);
//		bb ^= bb - 1;
//		folded = (int) bb ^ (bb >> 32);
//		return lsb_64_table[folded * 0x78291ACF >> 26];
//	}
//
//	int nextBitScanForward(uint64_t *bb) {
//
//		/*unsigned int folded;
//		assert (*bb != 0);
//		*bb ^= *bb - 1;
//		folded = (int) *bb ^ (*bb >> 32);
//		*bb &= (*bb - 1);
//		return lsb_64_table[folded * 0x78291ACF >> 26];*/
//		int result = bitScanForward(*bb);
//		*bb &= (*bb - 1);
//		return result;
//	}
//
//	int nextBitScanForward(uint64_t *bb, uint64_t *p) {
//		*p = (*bb) & -(*bb);	//LS1B
//		*bb &= (*bb - 1);	//LS1B_reset
//		return 0;
//	}
//
//	void nextBitScanForwards(uint64_t *bb, uint64_t *p) {
//		*p = (*bb) & -(*bb);	//LS1B
//		*bb &= (*bb - 1);	//LS1B_reset
//	}
//
//	void bitScanForward(uint64_t bb, int *p) {
//		while (bb > 0) {
//			*(p++) = bitScanForward(bb);  //LS1B
//			bb &= (bb - 1);	  //LS1B_reset
//		}
//		*(p++) = -1;
//	}
//
//	void LS1B(uint64_t bb, uint64_t *p) {
//		while (bb > 0) {
//			*(p++) = bb & -bb;  //LS1B
//			bb &= (bb - 1);	  //LS1B_reset
//		}
//		*(p++) = 0;
//	}
//
//	void findFirstSet(uint64_t num) {
//		// https://en.wikipedia.org/wiki/Find_first_set
//		// https://www.go4expert.com/articles/builtin-gcc-functions-builtinclz-t29238/
//		int clz = 0;
//		int ctz = 0;
//		int ones = 0;
//
//		clz = __builtin_clz(num);
//
//		printf("Number of leading zero's in %llu is %d\n", num, clz);
//
//		clz = __builtin_clz(-num);
//		printf("Number of leading zero's in %llu is %d\n", -num, clz);
//
//		ctz = __builtin_ctz(num);
//		printf("Number of trailing zero's in %llu is %d\n", num, ctz);
//
//		ones = __builtin_popcount(num);
//		printf("Number of one's in %llu is %d\n", num, ones);
//	}

		static const uint64_t notAFile = 0xfefefefefefefefe; // ~0x0101010101010101
		static const uint64_t notHFile = 0x7f7f7f7f7f7f7f7f; // ~0x8080808080808080
		static const uint64_t rank4 = uint64_t(0x00000000FF000000);
		static const uint64_t rank5 = uint64_t(0x000000FF00000000);

		static uint64_t nortOne(uint64_t b) { return  b << 8; }
		static uint64_t soutOne(uint64_t b) { return  b >> 8; }
		static uint64_t eastOne(uint64_t b) { return (b << 1) & notAFile; }
		static uint64_t noEaOne(uint64_t b) { return (b << 9) & notAFile; }
		static uint64_t soEaOne(uint64_t b) { return (b >> 7) & notAFile; }
		static uint64_t westOne(uint64_t b) { return (b >> 1) & notHFile; }
		static uint64_t soWeOne(uint64_t b) { return (b >> 9) & notHFile; }
		static uint64_t noWeOne(uint64_t b) { return (b << 7) & notHFile; }

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

		static int LS1B(uint64_t u64) {
			static const int lsb_64_table[64] = {
				63, 30,  3, 32, 59, 14, 11, 33,
				60, 24, 50,  9, 55, 19, 21, 34,
				61, 29,  2, 53, 51, 23, 41, 18,
				56, 28,  1, 43, 46, 27,  0, 35,
				62, 31, 58,  4,  5, 49, 54,  6,
				15, 52, 12, 40,  7, 42, 45, 16,
				25, 57, 48, 13, 10, 39,  8, 44,
				20, 47, 38, 22, 17, 37, 36, 26
			};
			unsigned int folded;
			assert (u64 != 0);
			u64 ^= u64 - 1;
			folded = (int) u64 ^ (u64 >> 32);
			return lsb_64_table[folded * 0x78291ACF >> 26];
		}
};
#endif /* TOOL_H_ */
