#include "../../board/bitboard.h"
#include "../Node.h"
#include "ScoreBoard.h"

#ifndef SEARCH_EVALFUNCTION_
#define SEARCH_EVALFUNCTION_

/*template <typename N, typename E>
class EvalFunction {
	ScoreBoard scoreboard;
public:
	EvalFunction() : scoreboard() {}
	EvalFunction(const ScoreBoard& scoreboard) : scoreboard(scoreboard){}
	E apply(Node& n);
};

template <typename N, typename E>
E EvalFunction<N, E>::apply(Node& n) {
	BitBoard board = static_cast<BitBoard>(n.getState());
	return scoreboard.apply(board);
}*/


class EvalFunction {
	ScoreBoard scoreboard;
public:
	EvalFunction() {}
	EvalFunction(const ScoreBoard& scoreboard) : scoreboard(scoreboard){}
	int apply(Node& n);
};

#endif /* SEARCH_EVALFUNCTION_ */
