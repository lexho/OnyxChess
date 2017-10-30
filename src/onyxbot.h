/*
 * onyxbot.h
 *
 *  Created on: Aug 30, 2017
 *      Author: alex
 */

#ifndef ONYXBOT_H_
#define ONYXBOT_H_

#include "board/bitboard.h"
#include "search/algorithms/AlphaBetaSearch.h"
#include "board/move.h"
#include <memory>

using namespace std;

class OnyxBot {
	BitBoard board;
	void init();
	AlphaBetaSearch alphabeta;
	int depthLimit;
	int currentScore;
public:
	OnyxBot();
	virtual ~OnyxBot();
	Move getNextMove();
	int getNextMoveScore();
};

#endif /* ONYXBOT_H_ */
