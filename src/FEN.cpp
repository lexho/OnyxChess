/*
 * FEN.cpp
 *
 *  Created on: Oct 17, 2017
 *      Author: alex
 */

#include "FEN.h"
#include "Utils.h"

FEN::FEN(string fenstr) {
	FEN::fenVect = Utils::splitString(fenstr, ' ');
}

FEN::~FEN() {
	// TODO Auto-generated destructor stub
}

string FEN::getPiecePlacement() {
	return fenVect.at(0);
}

char FEN::getActiveColor() {
	return fenVect.at(1).at(0);
}

string FEN::getCastling() {
	return fenVect.at(2);
}

string FEN::getEnPassant() {
	return fenVect.at(3);
}

string FEN::getHalfMove() {
	return fenVect.at(4);
}

string FEN::getFullMove() {
	return fenVect.at(5);
}

string FEN::toString() {
	return fenStr;
}
