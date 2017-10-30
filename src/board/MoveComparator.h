/*
 * MoveComperator.h
 *
 *  Created on: Aug 30, 2017
 *      Author: alex
 */

#ifndef BOARD_MOVECOMPARATOR_H_
#define BOARD_MOVECOMPARATOR_H_

#include<array>
#include "move.h"
#include "bitboard.h"

class MoveComparator {
	std::array<short, 64> board_raw;
	vector<Move> movelist;
	int getVictimValue(const Move& m);
	int getAggressorValue(const Move& m);
public:
	MoveComparator(std::array<short, 64> board_raw, vector<Move> movelist);
	virtual ~MoveComparator();

	bool operator () (const Move& a, const Move& b);
};

#endif /* BOARD_MOVECOMPARATOR_H_ */
