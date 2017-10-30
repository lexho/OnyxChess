/*
 * Node.h
 *
 *  Created on: Aug 31, 2017
 *      Author: alex
 */

#ifndef SEARCH_NODE_H_
#define SEARCH_NODE_H_

#include <memory>
#include <vector>
#include "../board/bitboard.h"
#include "../board/move.h"
#include "../datatypes.h"

using namespace std;

class Node {
public:
	virtual ~Node()=0;
	virtual BitBoard getState()=0;
	virtual Move getAction()=0;

	virtual shared_ptr<Node> getParent()=0;
	virtual int getDepth()=0;
	virtual bool isRoot()=0;
	virtual bool isLeaf()=0;
	virtual std::vector<shared_ptr<Node>> adjacent()=0;

	virtual int hashCode()=0;
	virtual U64 hashCode64()=0;
	virtual bool equals(shared_ptr<Node> n)=0;
};

inline Node::~Node() { }



#endif /* SEARCH_NODE_H_ */
