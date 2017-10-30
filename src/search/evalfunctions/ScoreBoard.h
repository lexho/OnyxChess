/*
 * ScoreBoard.h
 *
 *  Created on: Sep 10, 2017
 *      Author: alex
 */

#ifndef SEARCH_EVALFUNCTIONS_SCOREBOARD_H_
#define SEARCH_EVALFUNCTIONS_SCOREBOARD_H_

#include <array>
#include "../../board/bitboard.h"

class ScoreBoard {
	bool player_id;
	bool opponent_id;
	BitBoard start;
	BitBoard position;
	static const bool WHITE = true;
	static const bool BLACK = false;
	static std::array<short, 64> PawnTable;
	static std::array<short, 64> KnightTable;
	static std::array<short, 64> BishopTable;
	static std::array<short, 64> KingTable;
	static std::array<short, 64> KingTableEndGame;
public:
	ScoreBoard();
	ScoreBoard(const BitBoard& board);
	virtual ~ScoreBoard();
	int scoreCounter;
	static int scoreMaterial(BitBoard board);
	static int scoreMobility(BitBoard board); 	/* side to move relative score */
	static int scorePosition(BitBoard board);
	int apply(BitBoard board);
};

#endif /* SEARCH_EVALFUNCTIONS_SCOREBOARD_H_ */
