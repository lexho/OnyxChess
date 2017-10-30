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
#include <chrono>
#include <random>
#include "board/bitboard.h"
#include "board/lookuptables.h"
#include "onyxbot.h"

using namespace std;

random_device rn;
mt19937 rnd_engine(rn());

void printPossibleMovesWithTime(MoveList possible) {
	// Measure time of possible moves
	chrono::time_point<chrono::steady_clock> start, stop;
	start = chrono::steady_clock::now();

	stop = chrono::steady_clock::now();
	auto dur = stop-start;
	chrono::milliseconds mil = chrono::duration_cast<chrono::milliseconds>(dur);
	if(mil.count() == 0)
		cout << possible.size() << " moves in " << dur.count() << " ns" << endl;
	else
		cout << possible.size() << " moves in " << mil.count() << " ms" << endl;

	// print a list of possible moves
	for ( Move x  : possible ) std::cout << x << ' ';
	std::cout <<  std::endl;
	cout << endl;
}

void makeRandomMove(BitBoard& board) {
	Move m;
	MoveList possible = board.getPossibleMoves();

	// make random move
	uniform_int_distribution<int> rand(0, possible.size() - 1);
	m = possible.at(rand(rnd_engine));
	string color;
	if(board.isWhiteMove()) color = "white";
	else color = "black";
	cout << color << " makes move " << m << "..." << endl;
	board.makeMove(m);
	board.print();
}

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
	BitBoard::init();
	BitBoard bitboard;
	cout << bitboard.developerInfo << endl;
	bitboard.print();

	MoveList possible = bitboard.getPossibleMoves();
	printPossibleMovesWithTime(possible);

	// make white's first move
	makeRandomMove(bitboard);

	possible = bitboard.getPossibleMoves();
	printPossibleMovesWithTime(possible);

	// make black's first move
	makeRandomMove(bitboard);

	possible = bitboard.getPossibleMoves();
	printPossibleMovesWithTime(possible);

	cout << "create new bot" << endl;
	OnyxBot bot;
	Move nextMove = bot.getNextMove();
	cout << "next move: " << nextMove << ", score: " << bot.getNextMoveScore() << endl;

	return 0;
}
