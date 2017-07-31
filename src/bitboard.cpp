/*
 * bitboard.cpp
 *
 *  Created on: Jul 25, 2017
 *      Author: alex
 */
#include "bitboard.h"
#include "lookuptables.h"
#include "move.h"
#include <iostream>
#include <sstream>
#include <bitset>
#include <deque>
#include <limits>
#include <vector>

BitBoard::BitBoard() {
	static_assert((numeric_limits<U64>::digits == 64), "!!! U64 does not have 64 Bits !!!");

	occupied = 0ULL;

    // Initial position
	pieceTypeBB[1] = 0b0000000000000000000000000000000000000000000000001111111100000000ULL; // WhitePawns
	pieceTypeBB[2] = 0b0000000000000000000000000000000000000000000000000000000001000010ULL; // WhiteKnights
	pieceTypeBB[3] = 0b0000000000000000000000000000000000000000000000000000000000100100ULL; // WhiteBishops
	pieceTypeBB[4] = 0b0000000000000000000000000000000000000000000000000000000010000001ULL; // WhiteRooks
	pieceTypeBB[5] = 0b0000000000000000000000000000000000000000000000000000000000001000ULL; // WhiteQueens
	pieceTypeBB[6] = 0b0000000000000000000000000000000000000000000000000000000000010000ULL; // WhiteKing

	pieceTypeBB[7] = 0b0000000011111111000000000000000000000000000000000000000000000000ULL; // BlackPawns
	pieceTypeBB[8] = 0b0100001000000000000000000000000000000000000000000000000000000000ULL; // BlackKnights
	pieceTypeBB[9] = 0b0010010000000000000000000000000000000000000000000000000000000000ULL; // BlackBishops
	pieceTypeBB[10] = 0b1000000100000000000000000000000000000000000000000000000000000000ULL; // BlackRooks
	pieceTypeBB[11] = 0b0000100000000000000000000000000000000000000000000000000000000000ULL; // BlackQueens
	pieceTypeBB[12] = 0b0001000000000000000000000000000000000000000000000000000000000000ULL; // BlackKing

	whiteBB = 0b0000000000000000000000000000000000000000000000001111111111111111ULL;
	blackBB = 0b1111111111111111000000000000000000000000000000000000000000000000ULL;
	allBB = 0b1111111111111111000000000000000000000000000000001111111111111111ULL;
	whiteMove = true;
	wMtrl = bMtrl = 8 * PAWN_V + 2 * ROOK_V + 2 * BISHOP_V + 2 * KNIGHT_V + QUEEN_V + KING_V;

	//cout << std::bitset<64>(pieceTypeBB[10]) << endl;
	//cout << std::bitset<64>(pieceTypeBB[4]) << endl;


	board = "................................................................";
}

/** Creates a 8x8 Board from BitBoards */
void BitBoard::convertBitBoardTo8x8Board() {
	char pieceLabels[]{'.', 'P', 'N', 'B', 'R', 'Q', 'K', 'p', 'n', 'b', 'r', 'q', 'k'};

	/* Squares where no piece is on */
	vector<int> squares(64);
	for(int j = 0; j < 64; j++) {
		squares.push_back(j);
	}

	/*for(int sq : squares) {
		cout << sq << endl;
	}*/

	/* Bit-Masks for detecting pieces on squares */
	U64 mask = 0b0000000000000000000000000000000000000000000000000000000000000001ULL;
	array<U64, 64> masks;
	for(int i = 0; i < 64; i++) {
		masks[i] = mask;
		mask = mask << 1;
	}

	//cout << "squares size: " << squares.size() << endl;
	/*cout << string(10, 'x') << endl;
	for(auto m : masks) {
		cout << m << endl;
	}
	cout << string(10, 'x') << endl;*/

	/* Detect empty squares */
	for (auto i = squares.begin(); i != squares.end() ;) {
		int sq = *i;

		/*cout << "sq: " << sq << endl;
		cout << "allBB: " << allBB << endl;
		cout << "masks[sq] " << masks[sq] << endl;
		//cout << (allBB & masks[sq]) << " ==" << masks[sq] << endl;*/

		if(!((allBB & masks[sq]) == masks[sq])) {
			board[sq] = '.';
			squares.erase(i);
			//cout << "empty square at " << sq << endl;
			//cout << "allm: " << std::bitset<64>((allBB & masks[sq])) << endl;
			//cout << "mask: " << std::bitset<64>(masks[sq]) << endl;
			//cout << endl;
		} else
			++i; // otherwise increment it by yourself
	}

	/* Detect pieces */
	int pieceIndex = 0;
	for(U64 bitboard : pieceTypeBB) {
	//for(int i = 1; i < pieceTypeBB.size(); i++) {
		//U64 bitboard = pieceTypeBB.at(i);
		//cout << " bitb: " << std::bitset<64>(bitboard) << endl;
		for (auto i = squares.begin(); i != squares.end() ;) {
			int sq = *i;

			//cout << sq << " mask: " << std::bitset<64>(masks[sq]) << endl;
			if((bitboard & masks[sq]) == masks[sq]) {
				board[sq] = pieceLabels[pieceIndex];
				squares.erase(i);
				//cout << "piece " << pieceLabels[pieceIndex] << " at square " << sq << endl;
			} else
				++i; // otherwise increment it by yourself
		}
		pieceIndex++;
	}

	//cout << "board: "<< board << endl;
}

void BitBoard::updateBitBoards() {
    	createWhiteBB();
    	createBlackBB();
    	createAllPiecesBB();
}

void BitBoard::printBitBoards() {
		cout << "all: " << std::bitset<64>(allBB) << endl;
		cout << "white: " << std::bitset<64>(whiteBB) << endl;
		cout << "black: " << std::bitset<64>(blackBB) << endl;
		/*for(int p = 0; p < 13; p++) {
			printSingleBB(p);
		}*/
	}

void BitBoard::print() {
	/*cout << "Bitboards" << endl;
	cout << "==============================" << endl;
	for(const U64 &bb : pieceTypeBB)
		cout << bb << endl;
	cout << "==============================" << endl;*/

	//string inCheck = isInCheck() ? "ch" : "";
	//System.out.println(getActiveColor() + " " + inCheck);
	printBitBoards();
	cout << endl; // empty line to separate bitboard strings from board representation
	updateBitBoards();
	pieceTypeBB[0] = 0ULL; //TODO empty square bitboard is not up to date
	convertBitBoardTo8x8Board(); // create 8x8 board

	ostringstream result;
	//cout << "board: "<< board << endl;
	string boardstr(board);
	//cout << boardstr << endl;
	//cout << "boardstr size: " << boardstr.size() << endl;

	/* Add newlines after each rank */
	for(int j = 64 - 8; j >= 0; j -= 8) {
		//cout << j << " " << j + 8 << endl;
		//cout << "x: " << boardstr.substr(j, 8) << endl;
		result << boardstr.substr(j, 8);
		result << '\n';
	}
	//cout << boardstr << endl;
	//result << boardstr;

	cout << result.str() << endl;
}


void BitBoard::createWhiteBB() {
	whiteBB = 0ULL; // init white pieces bitboard with zeros
	for(int i = 1; i < 7; i++) {
		whiteBB |= pieceTypeBB[i];
	}
	//cout << "whiteBB: " << std::bitset<64>(whiteBB) << endl;
}

void BitBoard::createBlackBB() {
	blackBB = 0ULL; // init black pieces bitboard with zeros
	for(int i = 7; i < 13; i++) {
		//cout << "i: " << i << endl;
		//cout << "b: " << std::bitset<64>(blackBB) << endl;
		blackBB |= pieceTypeBB[i];
		//cout << "p: " << std::bitset<64>(pieceTypeBB[i]) << endl;
		//cout << endl;
	}
}

void BitBoard::createAllPiecesBB() {
	allBB = whiteBB | blackBB;

	//cout << "allBB: " << std::bitset<64>(allBB) << endl;
}

int BitBoard::getSquare(int x, int y) {
	return y * 8 + x;
}

	/** Return x position (file) corresponding to a square. */
int BitBoard::getX(int square) {
	return square & 7;
}

/** Return y position (rank) corresponding to a square. */
int BitBoard::getY(int square) {
	return square >> 3;
}

int BitBoard::trailingZ[] = {
		63,  0, 58,  1, 59, 47, 53,  2,
		60, 39, 48, 27, 54, 33, 42,  3,
		61, 51, 37, 40, 49, 18, 28, 20,
		55, 30, 34, 11, 43, 14, 22,  4,
		62, 57, 46, 52, 38, 26, 32, 41,
		50, 36, 17, 19, 29, 10, 13, 21,
		56, 45, 25, 31, 35, 16,  9, 12,
		44, 24, 15,  8, 23,  7,  6,  5
	};

int BitBoard::numberOfTrailingZeros(U64 mask) {
	return trailingZ[(int)(((mask & -mask) * 0x07EDD5E59A4E28C2L) >> 58)];
}

U64 BitBoard::addRay(U64 mask, int x, int y, int dx, int dy,
		U64 occupied, bool inner) {
	int lo = inner ? 1 : 0;
	int hi = inner ? 6 : 7;
	while (true) {
	if (dx != 0) {
	x += dx; if ((x < lo) || (x > hi)) break;
	}
	if (dy != 0) {
	y += dy; if ((y < lo) || (y > hi)) break;
	}
	int sq = BitBoard::getSquare(x, y); //Position12x10.coordToIndex(new int[] {x, y}); // was getSquare(x, y) before
	mask |= 1ULL << sq;
	if ((occupied & (1ULL << sq)) != 0)
	break;
	}
	return mask;
}

U64 BitBoard::addRookRays(int x, int y, U64 occupied, bool inner) {
		U64 mask = 0;
		mask = addRay(mask, x, y,  1,  0, occupied, inner);
		mask = addRay(mask, x, y, -1,  0, occupied, inner);
		mask = addRay(mask, x, y,  0,  1, occupied, inner);
		mask = addRay(mask, x, y,  0, -1, occupied, inner);
		return mask;
}

U64 BitBoard::createPattern(int i, U64 mask) {
	U64 ret = 0ULL;
	for (int j = 0; ; j++) {
		U64 nextMask = mask & (mask - 1);
		U64 bit = mask ^ nextMask;
		if ((i & (1L << j)) != 0)
			ret |= bit;
		mask = nextMask;
		if (mask == 0)
			break;
	}
	return ret;
}

/*uint32_t BitBoard::bitCount(U64 n)
{
	 // Java: use >>> instead of >>
	 // C or C++: use uint32_t
	 n = n - ((n >> 1) & 0x55555555);
	 n = (n & 0x33333333) + ((n >> 2) & 0x33333333);
	 return (((n + (n >> 4)) & 0x0F0F0F0F) * 0x01010101) >> 24;
}*/

array<vector<U64>, 64> BitBoard::rTables{};
array<U64, 64> BitBoard::rMasks;
int BitBoard::rBits[] = { 12, 11, 11, 11, 11, 11, 11, 12,
        11, 10, 10, 11, 10, 10, 10, 11,
        11, 10, 10, 10, 10, 10, 10, 11,
        11, 10, 10, 10, 10, 10, 10, 11,
        11, 10, 10, 10, 10, 10, 10, 11,
        11, 10, 10, 11, 10, 10, 10, 11,
        10,  9,  9,  9,  9,  9, 10, 10,
        11, 10, 10, 10, 10, 11, 10, 11 };

void BitBoard::initRookMagics() { // Rook magics
	//rTables = new U64[64][];
	//rMasks = new U64[64];
	for (int sq = 0; sq < 64; sq++) {
		int x = getX(sq);
		int y = getY(sq);
		rMasks[sq] = addRookRays(x, y, 0ULL, true);
		int tableSize = 1 << rBits[sq];
		vector<U64> table (tableSize, -1);
		//for (int i = 0; i < tableSize; i++) table[i] = -1;
		int nPatterns = 1 << std::bitset<64>(rMasks[sq]).count();
		//cout << "pattern: " << std::bitset<64>(rMasks[sq]) << endl;
		//cout << "count: " << std::bitset<64>(rMasks[sq]).count() << endl;
		for (int i = 0; i < nPatterns; i++) {
			U64 p = createPattern(i, rMasks[sq]);
			int entry = (int)((p * LookUpTables::rMagics[sq]) >> (64 - rBits[sq]));
			U64 atks = addRookRays(x, y, p, false);
			//cout << "atks: " << std::bitset<64>(atks) << endl;
			if (table[entry] == -1) {
				table[entry] = atks;
			} else if (table[entry] != atks) {
				throw runtime_error("Error: Something went wrong in initRookMagics()");
			}
		}
		rTables[sq] = table;
	}
}

U64 BitBoard::rookAttacks(int sq, U64 occupied) {
		/* cout << "occupied: " << std::bitset<64>(occupied) << endl;
		cout << "rMask   : " << std::bitset<64>(rMasks[sq]) << endl;
		cout << "rMagic  : " << std::bitset<64>(LookUpTables::rMagics[sq]) << endl;
		cout << "rBit    : " << std::bitset<64>(rBits[sq]) << endl; */
		return rTables[sq][(int)(((occupied & rMasks[sq]) * LookUpTables::rMagics[sq]) >> (64 - rBits[sq]))];
}

/** Get the white rooks possible moves */
MoveList BitBoard::whiteRooksMoves(U64 squares, U64 allBB, U64 whiteBB) {
	//cout << " white rooks moves" << endl;
//U64 squares = pos.pieceTypeBB[Piece.WROOK];
U64 m = 0x0ULL;
MoveList possible;
//possible.push_back(Move(0, 0));


while (squares != 0) {
	int sq = numberOfTrailingZeros(squares);
	m |= rookAttacks(sq, allBB) & ~whiteBB;
	//cout << "rookAttack: " << std::bitset<64>(m) << endl;

	// add move to movelist
	while (m != 0) {
		int sq1 = BitBoard::numberOfTrailingZeros(m);
		//System.out.println(sq + " " + sq1);
		//setMove(moveList, sq, sq1, Piece.EMPTY);
		possible.push_back(Move(sq, sq1));
		/*Move move(sq, sq1);
		possible.push_back(move);*/
		m &= (m - 1);
	}
	//cout << "squares &= squares-1; " << endl;
	squares &= squares-1;
}

//cout << "return rook moves " << endl;
return possible;
}

MoveList BitBoard::getPseudoLegalMoves() {
	// Update Bitboards
	BitBoard::createWhiteBB();
	BitBoard::createBlackBB();
	BitBoard::createAllPiecesBB();
	occupied = whiteBB | blackBB;

	//cout << "get pseudo legal moves" << endl;

	MoveList possible;

	if(whiteMove) {
		MoveList moves;
		moves = whiteRooksMoves(pieceTypeBB[WROOK], allBB, whiteBB);
		for ( Move m  : moves )
			possible.push_back(m);
		//possible.insert(end(possible), begin(moves), end(moves));

		/*moves = whiteKingMoves();
		possible.insert(end(possible), begin(moves), end(moves));
		moves = whitePawnMoves(pieceTypeBB[WPAWN], occupied, blackBB);
		possible.insert(end(possible), begin(moves), end(moves));
		moves = whiteBishopsMoves(pieceTypeBB[WBISHOP], allBB, whiteBB);
		possible.insert(end(possible), begin(moves), end(moves));
		moves = whiteQueenMoves(pieceTypeBB[WQUEEN], allBB, whiteBB);
		possible.insert(end(possible), begin(moves), end(moves));
		moves = whiteKnightsMoves(pieceTypeBB[WKNIGHT], whiteBB);
		possible.insert(end(possible), begin(moves), end(moves));
	} else {
		// TODO add black moves
    	/*possible.addAll(Movement.blackRooksMoves(pos.pieceTypeBB[Piece.BROOK], pos.allBB, pos.blackBB));
    	possible.addAll(Movement.blackKingMoves(this));
    	possible.addAll(Movement.blackPawnMoves(this, pos.pieceTypeBB[Piece.BPAWN], pos.occupied, pos.whiteBB));
    	possible.addAll(Movement.blackBishopsMoves(pos.pieceTypeBB[Piece.BBISHOP], pos.allBB, pos.blackBB));
    	possible.addAll(Movement.blackQueenMoves(pos.pieceTypeBB[Piece.BQUEEN], pos.allBB, pos.blackBB));
    	possible.addAll(Movement.blackKnightsMoves(pos.pieceTypeBB[Piece.BKNIGHT], pos.blackBB));*/
	}
	//cout << "return pseudo legal" << endl;
	return possible;
}

MoveList BitBoard::getPossibleMoves() {
	MoveList possible = getPseudoLegalMoves();
	//possible = removeIllegal(this, possible); // remove illegal moves (check situations)
	return possible;
}
