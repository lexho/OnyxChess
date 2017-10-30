/*
 * BoardNode.h
 *
 *  Created on: Aug 30, 2017
 *      Author: alex
 */

#ifndef SEARCH_BOARDNODE_H_
#define SEARCH_BOARDNODE_H_

#include "../board/move.h"
#include "../board/bitboard.h"
#include "Node.h"
#include <memory>

class BoardNode : public Node {
protected:
	shared_ptr<BoardNode> parent;
	shared_ptr<BitBoard> board;
	shared_ptr<Move> move;

	bool moveComparator(Move a, Move b);

	BitBoard getState();
	Move getAction();

	//Node getParent();

public:
	BoardNode();
	BoardNode(const BoardNode& node);
	BoardNode(const BoardNode* node);
	BoardNode(const BitBoard board);
	BoardNode(const BoardNode&  parent, Move move, BitBoard board);
	virtual ~BoardNode();

	virtual shared_ptr<Node> getParent();
	virtual int getDepth();
	virtual bool isRoot();
	virtual bool isLeaf();
	virtual std::vector<shared_ptr<Node>> adjacent();

	virtual int hashCode();
	virtual U64 hashCode64();
	virtual bool equals(shared_ptr<Node> n);
};

#endif /* SEARCH_BOARDNODE_H_ */
