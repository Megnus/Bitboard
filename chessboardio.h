/*
 * Render.h
 *
 *  Created on: 6 apr. 2018
 *      Author: msundstr
 */

#include <iostream>
#include <bitset>
#include <stdio.h>      /* printf */
#include <assert.h>     /* assert */
#include "byteswap.h"
#include "tool.h"
#include <string>
using namespace std;

#ifndef RENDER_H_
#define RENDER_H_

//namespace chessio {


class ChessboardIO {
	public:
		enum enumSquare {
			a1, b1, c1, d1, e1, f1, g1, h1,
			a2, b2, c2, d2, e2, f2, g2, h2,
			a3, b3, c3, d3, e3, f3, g3, h3,
			a4, b4, c4, d4, e4, f4, g4, h4,
			a5, b5, c5, d5, e5, f5, g5, h5,
			a6, b6, c6, d6, e6, f6, g6, h6,
			a7, b7, c7, d7, e7, f7, g7, h7,
			a8, b8, c8, d8, e8, f8, g8, h8,
			end
		};

		static string getSquare(enumSquare sq) {
			static const string stringSquare[64] = {
				"a1", "b1", "c1", "d1", "e1", "f1", "g1", "h1",
				"a2", "b2", "c2", "d2", "e2", "f2", "g2", "h2",
				"a3", "b3", "c3", "d3", "e3", "f3", "g3", "h3",
				"a4", "b4", "c4", "d4", "e4", "f4", "g4", "h4",
				"a5", "b5", "c5", "d5", "e5", "f5", "g5", "h5",
				"a6", "b6", "c6", "d6", "e6", "f6", "g6", "h6",
				"a7", "b7", "c7", "d7", "e7", "f7", "g7", "h7",
				"a8", "b8", "c8", "d8", "e8", "f8", "g8", "h8" };
			return stringSquare[sq];
		}

		static uint64_t setBoard(enumSquare squares[]) {
			uint64_t u64 = 0;
			int i = 0;
			while (squares[i] != end) {
				u64 |= (uint64_t)1 << squares[i++];
			}
			return u64;
		}

		static string getSquare(int index) {
			return getSquare((enumSquare)index);
		}

		static string getSquare64(uint64_t u64) {
			return getSquare(Tool::LS1B(u64));
		}

		static void print(uint64_t u64) {
			std::bitset<32> a(u64 >> 32);
			std::bitset<32> b(u64);
			string c = a.to_string() + b.to_string();
			for (int i = 0; i < 64; i += 8) {
				std::cout << c.substr(i, 8) << std::endl;
			}
			std::cout << std::endl;
		}

		static void printBigBoard(uint64_t u64) {
			u64 = Tool::flipVertical(u64);
			string border = std::string(5, '_');
			string space = std::string(5, ' ');
			string set = "  X  ";
			for (int i = 0; i < 8; i++) {
				std::cout << " " << border;
			}
			cout << std::endl;
			uint64_t mask = (uint64_t)1;//0b1 << 63;
			for (int k = 0; k < 8; k++) {
				for (int j = 0; j < 3; j++) {
					for (int i = 0; i < 8; i++) {
						std::cout << "|" << (j == 1 && (u64 & mask) > 0 ? set : (j == 2 ? border : space));

						mask <<= j == 1 ? 1 : 0;
					}

					std::cout << "|" << std::endl;
				}
			}
		}

		static void printSmallBoard() {
			string square = "[ ]";
			for (int j = 0; j < 8; j++) {
				for (int i = 0; i < 8; i++) {
					std::cout << square;
				}
				std::cout << std::endl;
			}
		}
};
//}
#endif /* RENDER_H_ */
