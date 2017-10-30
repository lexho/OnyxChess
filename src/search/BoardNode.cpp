/*
 * BoardNode.cpp
 *
 *  Created on: Aug 30, 2017
 *      Author: alex
 */

#include <algorithm>
#include <functional>
#include <memory>
#include "BoardNode.h"
#include "../board/MoveComparator.h"
#include "../board/exceptions/BitBoardError.cpp"

using namespace std;

BoardNode::BoardNode() {
	parent = nullptr;
}

BoardNode::~BoardNode() {
	// TODO Auto-generated destructor stub
}

BoardNode::BoardNode(const BitBoard board) {
	//TODO implement BoardNode constructor
	BoardNode::board = make_shared<BitBoard>(board);
}

BoardNode::BoardNode(const BoardNode* node) : BoardNode(*node) {}

BoardNode::BoardNode(const BoardNode& node) : parent(node.parent), move(node.move), board(node.board){

}

BoardNode::BoardNode(const BoardNode& parent, Move move, BitBoard board) {
	/*BoardNode par = parent; // TODO is this efficient? (just pass the pointer)
	BoardNode::parent = shared_ptr<BoardNode>(&par);
	BoardNode::move = shared_ptr<Move>(&move);
	BoardNode::board = shared_ptr<BitBoard>(&board);*/

	BoardNode::parent = make_shared<BoardNode>(parent);
	BoardNode::move = make_shared<Move>(move);
	BoardNode::board = make_shared<BitBoard>(board);
	//BitBoard b = BitBoard(*board); //TODO copy board!!!
	for(short sq : BoardNode::board->getSquares()) {
	   if(sq > 12) throw BitBoardError(*BoardNode::board, "ínvalid piece " + to_string(sq) + " on squares, at BoardNode::BoardNode");
	}
}

vector<shared_ptr<Node>> BoardNode::adjacent() {
	//cout << "BoardNode::adjacent()" << endl;

	vector<shared_ptr<Node>> successors;
	vector<Move> possible = board->getPossibleMoves();

	/* Move Ordering */
	//int[] board_raw = board.getPosition12x10().get12x10Board();
	array<short, 64> board_raw = board->getSquares();
	//cout << "squares: " << endl;
	for(short sq : board->getSquares()) {
		//cout << to_string(sq) << " ";
		if(sq > 12) throw BitBoardError(*board, "invalid piece " + to_string(sq) + " on squares");
	}
	//cout << endl;

	// sorts moves by Most Valuable Victim - Least Valuable Aggressor
	using namespace std::placeholders;

	std::sort (possible.begin(), possible.end(), MoveComparator(board_raw, possible));

	//cout << "sorted moves" << endl;

	for (Move move : possible)
	{
		//System.out.println(move);
		//System.out.println(move.getSource8x8Index() + " " + move.getTarget8x8Index());
		BitBoard next = BitBoard(*board); //TODO copy board!!!
		for(short sq : next.getSquares()) {
		   if(sq > 12) throw BitBoardError(next, "invalid piece " + to_string(sq) + " on squares, at BoardNode::adjacent");
		}
		//cout << "created bitboard by bitboard" << endl;
		//cout << "make move " << move << "..." << endl;
		try {
			next.makeMove(move);
		} catch (BitBoardError error) {
			std::cerr << "Fehler: " << error.what() << endl;
			std::cerr << error.getDebugInfo() << endl;
		}
		//cout << "made move" << endl;
		//System.out.println(next);
		//BoardNode n = BoardNode(*this, move, next);
		successors.push_back(shared_ptr<Node>(new BoardNode(*this, move, next)));
	}

	return successors;
}

BitBoard BoardNode::getState() {
	return *board;
}

shared_ptr<Node> BoardNode::getParent() {
	return parent;
}

Move BoardNode::getAction() {
	return *move;
}

int BoardNode::getDepth() {
	return board->getMoveNr();
}

bool BoardNode::isRoot() {
	return parent == nullptr;
}

bool BoardNode::isLeaf() {
	if(!board->isRunning())
		return true;
	else
		return false;
}

int BoardNode::hashCode() {
	return 0; // TODO implement
}

U64 BoardNode::hashCode64() {
	return 0LL; // TODO implement
}

bool BoardNode::equals(shared_ptr<Node> n) {
	return false;
	//TODO implement equals
	/*if (this == n)
		return true;
	if (n == nullptr)
		return false;*/
}
