/*
 * move.h
 *
 *  Created on: Jul 25, 2017
 *      Author: alex
 */

#include <iostream>
#include <string>
#include <array>

#ifndef MOVE_H_
#define MOVE_H_

class Move {
	int src8x8;
	int trg8x8;
	int promPiece = 0;
	friend std::ostream& operator<<(std::ostream&, const Move &m);
	void createMove(int src, int trg, int type);
	friend bool operator==(const Move& a, const Move& b);
public:
	Move();
	//Move(const Move& m);
	Move(int src8x8, int trg8x8);
	Move(int src8x8, int trg8x8, int promPiece);
	int getSource8x8Index() const;
	int getTarget8x8Index() const;
	int getPromoteTo();
	static bool compare(std::array<short, 64> board_raw, Move a, Move b);
	std::string toString();
};



#endif /* MOVE_H_ */
