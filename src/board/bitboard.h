/*
 * bitboard.h
 *
 *  Created on: Jul 25, 2017
 *      Author: alex
 */
#include <array>
#include <vector>
#include "move.h"
#include "../datatypes.h"

#ifndef BITBOARD_H_
#define BITBOARD_H_

using namespace std;

typedef std::vector<Move> MoveList;

class BitBoard {
	static const short FILE_A = 0;
	static const short FILE_B = 1;
	static const short FILE_C = 2;
	static const short FILE_D = 3;
	static const short FILE_E = 4;
	static const short FILE_F = 5;
	static const short FILE_G = 6;
	static const short FILE_H = 7;

	static const short RANK_1 = 0;
	static const short RANK_2 = 1;
	static const short RANK_3 = 2;
	static const short RANK_4 = 3;
	static const short RANK_5 = 4;
	static const short RANK_6 = 5;
	static const short RANK_7 = 6;
	static const short RANK_8 = 7;

    /** Bit definitions for the castleMask bit mask. */
	static const int A1_CASTLE = 0; /** White long castle. */
	static const int H1_CASTLE = 1; /** White short castle. */
	static const int A8_CASTLE = 2; /** Black long castle. */
	static const int H8_CASTLE = 3; /** Black short castle. */

 	static void initRookMagics();

	static int trailingZ[];

	static array<vector<U64>, 64> rTables;
	static array<U64, 64> rMasks;
	static int rBits[];

	static int nPieceTypes;
	array<U64, 13> pieceTypeBB;
	U64 whiteBB, blackBB, allBB;
	U64 occupied;
	array<short, 64> squares;
	string board; // 8x8 printable board

	int wMtrl = 0;
	int bMtrl = 0;
	bool whiteMove;
	int castleMask;
	int epSquare;
	int halfMoveClock; // Number of half-moves since last 50-move reset.
	int fullMoveCounter; /** Game move number, starting from 1. */

	void setPiece(int square, int piece);
	static bool isWhitePiece(int p);
	void movePieceNotPawn(int from, int to);

	void setCastleMask(int castleMask);
	void setEpSquare(int epSquare);
	int getEpSquare();

	MoveList getPseudoLegalMoves();
	static MoveList whitePawnMoves(BitBoard pos, U64 pawnsBB, U64 occupied, U64 blackBB);
	MoveList blackPawnMoves(BitBoard pos, U64 pawnsBB, U64 occupied, U64 whiteBB);
	MoveList whiteKingMoves();
	MoveList blackKingMoves();
	U64 compute_king_incomplete(U64 king_loc, U64 own_side);
	MoveList whiteBishopsMoves(U64 squares, U64 allBB, U64 whiteBB);
	MoveList blackBishopsMoves(U64 squares, U64 allBB, U64 blackBB);
	MoveList whiteRooksMoves(U64 squares, U64 allBB, U64 colorBB);
	MoveList blackRooksMoves(U64 squares, U64 allBB, U64 colorBB);
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
	void addMovesByMask(MoveList& moveList, int sq0, U64 mask);
	static bool addPawnMovesByMask(MoveList& moveList, BitBoard pos, U64 mask,
	            int delta, bool allPromotions);
	static U64 addRay(U64 mask, int x, int y, int dx, int dy,
			U64 occupied, bool inner);
	static U64 addRookRays(int x, int y, U64 occupied, bool inner);
	static U64 createPattern(int i, U64 mask);
	static U64 rookAttacks(int sq, U64 occupied);
public:
	static string developerInfo;
	class UndoInfo {
	public:
		int capturedPiece;
		int castleMask;
		int epSquare;
		int halfMoveClock;
	};
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
	static array<int, 13> pieceValue;
	static int getPieceValue(int piece);
	BitBoard();
	BitBoard(const BitBoard& board);
	void print();
	MoveList getPossibleMoves();
	static void init();
	static int getSquare(int x, int y);
	static int getX(int square);
	static int getY(int square);
	array<short, 64> getSquares();
	bool isWhiteMove();
	bool makeMove(Move move);
	bool makeMove(Move m, UndoInfo ui);
	bool unMakeMove(Move m, UndoInfo ui);
	//BitBoard& copy();
};
#endif /* BITBOARD_H_ */
