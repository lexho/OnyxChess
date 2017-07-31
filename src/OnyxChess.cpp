//============================================================================
// Name        : OnyxChess.cpp
// Author      : Alexander Hoertenhuber
// Version     :
// Copyright   : © 2017 Alexander Hoertenhuber All Rights Reserved
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <array>
#include <iostream>
#include <limits>
#include "bitboard.h"
#include "lookuptables.h"

using namespace std;

int main() {

	//U64 bitboard = 0b0000000000000000000000000000000000000000000000001111111100000000L; // WhitePawns;
	//U64* pieceTypeBB = new U64[12];

	array<U64, 12> pieceTypeBB;

	cout << "*** OnyxChess ***" << endl; // prints !!!Hello World!!!
	cout << endl;

	// Determine size of datatype U64 (should be 64 Bits for optimal performance
	//cout << "Datatype U64 has " << sizeof(U64) << " Bytes" << endl;
	//cout << "Datatype U64 has " << numeric_limits<U64>::digits << " Bits "<< endl << endl;

	LookUpTables::init();
	BitBoard::initRookMagics();
	BitBoard bitboard;
	bitboard.print();

	// Get possible moves and print them
	MoveList possible = bitboard.getPossibleMoves();
	cout << "Nr of possible moves: " << possible.size() << endl;
	for ( Move x  : possible ) std::cout << x << ' ';
	std::cout <<  std::endl;

	return 0;
}
