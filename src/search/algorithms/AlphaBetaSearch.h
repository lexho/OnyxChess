/*
 * AlphaBetaSearch.h
 *
 *  Created on: Aug 31, 2017
 *      Author: alex
 */

#ifndef SEARCH_ALPHABETASEARCH_H_
#define SEARCH_ALPHABETASEARCH_H_

#include<utility>
#include <memory>
#include "../AdversarialSearch.h"
#include "../Node.h"
#include "../evalfunctions/EvalFunction.h"

using namespace std;

class AlphaBetaSearch : public AdversarialSearch {
	long starttime;
	int alphaBound;
	int betaBound;
	//shared_ptr<EvalFunction<Node, int>> evalFunction;
	EvalFunction evalFunction;
protected:
	int depthLimit;
	int depth;
	pair<shared_ptr<Node>, int> alphaBetaMax( shared_ptr<Node> current, pair<shared_ptr<Node>, int> alpha, pair<shared_ptr<Node>, int> beta, int depthleft );
	pair<shared_ptr<Node>, int> alphaBetaMin( shared_ptr<Node> current, pair<shared_ptr<Node>, int> alpha, pair<shared_ptr<Node>, int> beta, int depthleft );
public:
	AlphaBetaSearch();
	AlphaBetaSearch(int depthLimit, long starttime);
	virtual ~AlphaBetaSearch();
	pair<shared_ptr<Node>, int> search(shared_ptr<Node> start, EvalFunction evalFunction);// throws SearchFailException;
};

#endif /* SEARCH_ALPHABETASEARCH_H_ */
