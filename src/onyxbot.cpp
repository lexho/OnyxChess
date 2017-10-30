/*
 * onyxbot.cpp
 *
 *  Created on: Aug 30, 2017
 *      Author: alex
 */

#include "onyxbot.h"
#include "board/bitboard.h"
#include "search/BoardNode.h"
#include <iostream>
#include <memory>
#include "search/evalfunctions/EvalFunction.h"

using namespace std;

OnyxBot::OnyxBot() : depthLimit(8)
{
	cout << "OnyxBot by Alexander Hoertenhuber | August 2017" << endl;
	board = BitBoard();
	init();
}

OnyxBot::~OnyxBot() {
	// TODO Auto-generated destructor stub
}

void OnyxBot::init() {

}

Move OnyxBot::getNextMove() {
	Move nextMove;
	//long searchtime = System.currentTimeMillis(); // current time in milliseconds

	/* AlphaBeta Search */
	//ScoreBoard scoreboard = new ScoreBoard(board.copy());
	ScoreBoard scoreboard(board);
	EvalFunction evalFunction(scoreboard);

	long starttime = 0; // TODO implement starttime

	// iterative deepening
	for(int depth = 8; depth <= depthLimit; depth++) {
		alphabeta = AlphaBetaSearch(depth, starttime);

		pair<shared_ptr<Node>, int> result;
		//double[] aspReset = {new Double(aspwindow[0]), new Double(aspwindow[1])};
		bool failed = false;
		//alphabeta.setBounds(Integer.MIN_VALUE, Integer.MAX_VALUE);
		result = alphabeta.search(
				make_shared<BoardNode>(new BoardNode(board)),
				evalFunction);
		currentScore = result.second;

		if(result.first != nullptr) {
			//System.out.println(result.first.getAction() + " " + result.second + " depth " + depth);
			currentScore = result.second;
			nextMove = result.first->getAction();
		}
	}

	//searchtime = System.currentTimeMillis() - searchtime;

	//printSearchStats(searchtime, scoreboard);

	return nextMove;
}

int OnyxBot::getNextMoveScore() {
	return currentScore;
}
