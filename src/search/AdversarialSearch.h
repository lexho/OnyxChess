/*
 * AdversarialSearch.h
 *
 *  Created on: Aug 30, 2017
 *      Author: alex
 */

#ifndef SEARCH_ADVERSARIALSEARCH_H_
#define SEARCH_ADVERSARIALSEARCH_H_
#include <utility>
#include <memory>

#include "evalfunctions/EvalFunction.h"
#include "Node.h"

using namespace std;

class AdversarialSearch {
public:
	virtual ~AdversarialSearch()=0;
	/**
	 * This method takes a node from the state space and an evalFunction. The
	 * evalFunction should get called when the search terminates. Each leaf of the search-tree
	 * should therefore have a value from this evalFunction associated with it.
	 *
	 * The function returns a pair, namely the most beneficial state we want to be in, given the
	 * current state and the evalFunction. Additionally we'll return the value of the evalFunction
	 * that has led to this decision.
	 *
	 * @param start the starting node in state space
	 * @param evalFunction the eval function that scores a leaf
	 * @return Pair<T, Double> a pair (bestMove, score)
	 */
	virtual pair<shared_ptr<Node>, int> search(shared_ptr<Node> start, EvalFunction evalFunction)=0;
};

inline AdversarialSearch::~AdversarialSearch() { }

#endif /* SEARCH_ADVERSARIALSEARCH_H_ */
