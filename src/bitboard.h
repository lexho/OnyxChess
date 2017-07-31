/*
 * bitboard.h
 *
 *  Created on: Jul 25, 2017
 *      Author: alex
 */
#include <array>
#include <vector>
#include "move.h"
#include "datatypes.h"

#ifndef BITBOARD_H_
#define BITBOARD_H_

using namespace std;

typedef std::vector<Move> MoveList;

class BitBoard {
	static int trailingZ[];

	static array<vector<U64>, 64> rTables;
	static array<U64, 64> rMasks;
	static int rBits[];

	array<U64, 13> pieceTypeBB;
	U64 whiteBB, blackBB, allBB;
	U64 occupied;
	string board; // 8x8 printable board

	int wMtrl = 0;
	int bMtrl = 0;

	bool whiteMove;
	MoveList getPseudoLegalMoves();
	MoveList whitePawnMoves(U64 pawnsBB, U64 occupied, U64 blackBB);
	MoveList blackPawnMoves(U64 pawnsBB, U64 occupied, U64 whiteBB);
	MoveList whiteKingMoves();
	MoveList blackKingMoves();
	MoveList whiteBishopsMoves(U64 squares, U64 allBB, U64 whiteBB);
	MoveList blackBishopsMoves(U64 squares, U64 allBB, U64 blackBB);
	MoveList whiteRooksMoves(U64 squares, U64 allBB, U64 whiteBB);
	MoveList blackRooksMoves(U64 squares, U64 allBB, U64 blackBB);
	MoveList whiteQueenMoves(U64 squares, U64 allBB, U64 whiteBB);
	MoveList blackQueenMoves(U64 squares, U64 allBB, U64 blackBB);
	MoveList whiteKnightsMoves(U64 squares, U64 whiteBB);
	MoveList blackKnightsMoves(U64 squares, U64 blackBB);

	void createWhiteBB();
	void createBlackBB();
	void createAllPiecesBB();
	void convertBitBoardTo8x8Board();
	void updateBitBoards();
	void printBitBoards();

	static int numberOfTrailingZeros(U64 mask);
	static U64 addRay(U64 mask, int x, int y, int dx, int dy,
			U64 occupied, bool inner);
	static U64 addRookRays(int x, int y, U64 occupied, bool inner);
	static U64 createPattern(int i, U64 mask);
	static U64 rookAttacks(int sq, U64 occupied);
	//static uint32_t bitCount(U64 n);
public:
	// Piece bitboard indices
	const static short EMPTY = 0;
	const static short WPAWN = 1;
	const static short WKNIGHT = 2;
	const static short WBISHOP = 3;
	const static short WROOK = 4;
	const static short WQUEEN = 5;
	const static short WKING = 6;

	const static short BPAWN = 7;
	const static short BKNIGHT = 8;
	const static short BBISHOP = 9;
	const static short BROOK = 10;
	const static short BQUEEN = 11;
	const static short BKING = 12;

	const static int KING_V = 10000;
	const static int QUEEN_V = 900;
	const static int ROOK_V = 465;
	const static int BISHOP_V = 325;
	const static int KNIGHT_V = 275;
	const static int PAWN_V = 100;
	BitBoard();
	void print();
	MoveList getPossibleMoves();
	static void initRookMagics();
	static int getSquare(int x, int y);
	static int getX(int square);
	static int getY(int square);
};
#endif /* BITBOARD_H_ */
