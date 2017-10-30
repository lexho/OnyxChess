/*
 * MoveComperator.cpp
 *
 *  Created on: Aug 30, 2017
 *      Author: alex
 */

#include "MoveComparator.h"
#include <algorithm>

MoveComparator::MoveComparator(std::array<short, 64> board_raw, vector<Move> movelist) {
	// TODO Auto-generated constructor stub
	MoveComparator::board_raw = board_raw;
	MoveComparator::movelist = movelist;
}

MoveComparator::~MoveComparator() {
	// TODO Auto-generated destructor stub
}

bool MoveComparator::operator () (const Move& a, const Move& b) {
	//cout << "compare: " + a.toString() + " with " + b.toString() << endl;
	/*std::vector<Move>::iterator it = std::find(movelist.begin(), movelist.end(), a);
	if(it != movelist.end()) {
		auto pos = it - movelist.begin();
		cout << "movelist contains: " << a << " at "<< pos << endl;
	}
	else {
	    // v does not contain x
		cerr << "movelist does not contain move " << a << endl;
	}
	it = std::find(movelist.begin(), movelist.end(), b);
	if(it != movelist.end()) {
		auto pos = it - movelist.begin();
		cout << "movelist contains: " << b << " at "<< pos << endl;
	}
	else {
	    // v does not contain x
		cerr << "movelist does not contain move " << b << endl;
	}
	//TODO test moves for debugging
	if(a.getSource8x8Index() > 63 || a.getSource8x8Index() < 0) cerr << "Illegal Move " << a << " in MoveComparator::operator ()!" << endl;
	if(a.getTarget8x8Index() > 63 || a.getTarget8x8Index() < 0) cerr << "Illegal Move " << a << " in MoveComparator::operator ()!" << endl;
	if(b.getSource8x8Index() > 63 || b.getSource8x8Index() < 0) cerr << "Illegal Move " << b << " in MoveComparator::operator ()!" << endl;
	if(b.getTarget8x8Index() > 63 || b.getTarget8x8Index() < 0) cerr << "Illegal Move " << b << " in MoveComparator::operator ()!" << endl;*/

	int a_vval = getVictimValue(a);
	int b_vval = getVictimValue(b);
	if(a_vval > b_vval)
		return false;
	else if(a_vval == b_vval) {
		int a_agrval = getAggressorValue(a);
		int b_agrval = getAggressorValue(b);
		if(a_agrval < b_agrval)
			return false;
		else if(a_agrval == b_agrval)
			return false;
		else
			return true;
	} else {
		return true;
	}
}

int MoveComparator::getVictimValue(const Move& m) {
	int trg = m.getTarget8x8Index();
	int piece = board_raw[trg];

	if(piece != BitBoard::EMPTY)
		return BitBoard::getPieceValue(piece);
	else return 0;

}

int MoveComparator::getAggressorValue(const Move& m) {
	int src = m.getSource8x8Index();
	int piece = board_raw[src];

	if(piece != BitBoard::EMPTY)
		return BitBoard::getPieceValue(piece);
	else return 0;
}
