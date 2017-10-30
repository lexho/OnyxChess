/*
 * FEN.h
 *
 *  Created on: Oct 17, 2017
 *      Author: alex
 */

#ifndef FEN_H_
#define FEN_H_

#include <string>
#include <vector>

using namespace std;

class FEN {
	vector<string> fenVect;
	string fenStr;
public:
	FEN(string  fenstr);
	virtual ~FEN();

	string getPiecePlacement();
	char getActiveColor();
	string getCastling();
	string getEnPassant();
	string getHalfMove();
	string getFullMove();
	string toString();
};

#endif /* FEN_H_ */
