/*
 * AlphaBetaSearch.cpp
 *
 *  Created on: Aug 31, 2017
 *      Author: alex
 */

#include "AlphaBetaSearch.h"
#include "../BoardNode.h"
#include "../evalfunctions/ScoreBoard.h"
#include "../evalfunctions/EvalFunction.h"
#include "../../board/exceptions/BitBoardError.cpp"
#include <limits>
#include <memory>
#include <ostream>

using namespace std;

AlphaBetaSearch::AlphaBetaSearch() {}

AlphaBetaSearch::AlphaBetaSearch(int depthLimit, long starttime) : depthLimit(depthLimit), starttime(starttime), evalFunction(ScoreBoard()) {
	//lastinfo = System.currentTimeMillis();
	//interrupted = false; // reset interrupt
	alphaBound = std::numeric_limits<int>::min();
	betaBound = std::numeric_limits<int>::max();
	//evalFunction = EvalFunction<Node, int>(ScoreBoard());
}

AlphaBetaSearch::~AlphaBetaSearch() {
	// TODO Auto-generated destructor stub
}

class SearchFailException {
public:
	SearchFailException(bool alphaFail) {
		cerr << "Search Fail Exception" << endl;
	}
};

std::pair<shared_ptr<Node>, int> AlphaBetaSearch::search(shared_ptr<Node> start, EvalFunction evalFunction) {
	cout << "AlphaBetaSearch::search" << endl;
	//nodecount = 0;
	//lastinfo = System.currentTimeMillis();

	shared_ptr<Node> nullNode = nullptr; //(new BoardNode());
	pair<shared_ptr<Node>, int> alpha(nullNode, alphaBound); //Double.POSITIVE_INFINITY;
	pair<shared_ptr<Node>, int> beta(nullNode, betaBound); // Double.NEGATIVE_INFINITY;
	//if(alpha.first == nullptr) throw invalid_argument("alpha is NullNode");
	depth = 0;
	shared_ptr<Node> current = start;
	//TODO uncomment evalFunction
	//AlphaBetaSearch::evalFunction = shared_ptr<EvalFunction<shared_ptr<Node>, int>>(&evalFunction);

	cout << "alpha: " << alpha.second << endl;
	cout << "beta: " << beta.second << endl;

	pair<shared_ptr<Node>, int> result = alphaBetaMax( start, alpha, beta, depthLimit);

	/* Alpha- and Beta-Fails*/
	// TODO implement alpha beta fails
	if(result.first == nullNode) {
		if(result.second == alphaBound) throw SearchFailException(true);
		else throw SearchFailException(false);
	}
	return result;
}

pair<shared_ptr<Node>, int> AlphaBetaSearch::alphaBetaMax( shared_ptr<Node> current,  pair<shared_ptr<Node>, int> alpha,  pair<shared_ptr<Node>, int> beta, int depthleft ) {
	//cout << "AlphaBetaSearch::alphaBetaMax" << endl;
	if(current == nullptr) throw invalid_argument("calling max on NullNode");
	BitBoard board = current->getState();
	for(short sq : board.getSquares()) {
		if(sq > 12) throw BitBoardError(board, "ínvalid piece " + to_string(sq) + " on squares, at AlphaBetaSearch::alphaBetaMax");
	}
	//nodecount++;
	//cout << "depthleft: " << depthleft << endl;
	if ( depthleft == 0 || current->isLeaf() ) {
		//cout << "isLeaf / depthlimit" << endl;
		//NrOfLeafNodes++;
		int eval = evalFunction.apply(*current);
		//cout << "evaluation score: " << eval << endl;
		return pair<shared_ptr<Node>, int>(current, eval);
	}
	   int current_depth = depth;
	   int i = 0;
	   for(auto &node : current->adjacent()) {
			if(node == nullptr) throw invalid_argument("adj is NullNode");
			depth = current_depth;
			depth++;
			BitBoard board = node->getState();
			for(short sq : board.getSquares()) {
			   if(sq > 12) {
				   cerr << "parent: " << endl;
				   shared_ptr<Node> parent = node->getParent();
				   if(parent->isRoot()) cerr << " is root";
				   /*for(short sq : parent->getState().getSquares()) {
					   cerr << sq << " ";
				   }*/
				   cerr << endl;
				   throw BitBoardError(board, "ínvalid piece " + to_string(sq) + " on squares, at AlphaBetaSearch::alphaBetaMax adj");
			   }
			}
			pair<shared_ptr<Node>, int> min = alphaBetaMin( shared_ptr<Node>(node), alpha, beta, depthleft - 1 );
			int score = min.second;

			//printInfoMessage(node, score);
			if( score >= beta.second ) {
			  if(current == nullptr) throw invalid_argument("beta is NullNode");
			  //cout << "score: " << score << " >= beta " << beta.second << endl;
			 return beta;   // fail hard beta-cutoff
			}
			if( score > alpha.second ) {
			 alpha = pair<shared_ptr<Node>, int>(shared_ptr<Node>(node), score); // alpha acts like max in MiniMax
			}
			i++;
	   }
	   return alpha;
}

pair<shared_ptr<Node>, int> AlphaBetaSearch::alphaBetaMin( shared_ptr<Node> current, pair<shared_ptr<Node>, int> alpha, pair<shared_ptr<Node>, int> beta, int depthleft ) {
	//cout << "AlphaBetaSearch::alphaBetaMin" << endl;
	if(current == nullptr) throw invalid_argument("calling min on NullNode");
	BitBoard board = current->getState();
	for(short sq : board.getSquares()) {
		if(sq > 12) throw BitBoardError(board, "ínvalid piece " + to_string(sq) + " on squares, at AlphaBetaSearch::alphaBetaMin");
	}

	//nodecount++;
	//cout << "depthleft: " << depthleft << endl;
	if ( depthleft == 0 || current->isLeaf() ) {
		//cout << "isLeaf / depthlimit" << endl;
		//NrOfLeafNodes++;
		int eval = -evalFunction.apply(*current);
		//cout << "evaluation score: " << eval << endl;
		return pair<shared_ptr<Node>, int>(current, eval);
	}
	   int current_depth = depth;
	   int i = 0;
	   vector<shared_ptr<Node>> adj;
	   try {
		   adj = current->adjacent();
	   } catch (BitBoardError error){
		   std::cerr << "Fehler: " << error.what() << endl;
		   std::cerr << error.getDebugInfo() << endl;
	   }
	   for ( auto &node : current->adjacent() ) {
		   //cout << "adj " << i << endl;
		   if(node == nullptr) throw invalid_argument("adj is NullNode");
		   depth = current_depth;
		   depth++;
		  int score = alphaBetaMax( node, alpha, beta, depthleft - 1 ).second;
		  //printInfoMessage(node, score);
	      if( score <= alpha.second ) {
			  //cout << "score: " << score << " <= alpha: " << alpha.second << endl;
	    	  //alphaCount += current.adjacent().size() - i;
	    	  if(current == nullptr) throw invalid_argument("alpha is NullNode");
	         return alpha; // fail hard alpha-cutoff
	      }
	      if( score < beta.second ) {
	    	  //cout << "set new beta with score: " << score << endl;
	    	  beta = pair<shared_ptr<Node>, int>(shared_ptr<Node>(node), score);  // beta acts like min in MiniMax
	      }
	      i++;
	   }
	   return beta;
}
