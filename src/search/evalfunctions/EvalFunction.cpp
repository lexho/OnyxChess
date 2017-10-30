/*
 * EvalFunction.cpp
 *
 *  Created on: Sep 11, 2017
 *      Author: alex
 */

#include "EvalFunction.h"

int EvalFunction::apply(Node& n) {
	BitBoard board = static_cast<BitBoard>(n.getState());
	return scoreboard.apply(board);
}
