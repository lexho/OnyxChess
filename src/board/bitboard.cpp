/*
 * bitboard.cpp
 *
 *  Created on: Jul 25, 2017
 *      Author: alex
 */
#include "bitboard.h"
#include "lookuptables.h"
#include "move.h"
#include "exceptions/BitBoardError.cpp"
#include <iostream>
#include <sstream>
#include <bitset>
#include <deque>
#include <limits>
#include <vector>
#include <ctype.h>
#include "../Utils.h"

array<int, 13> BitBoard::pieceValue = {0, PAWN_V, KNIGHT_V, BISHOP_V, ROOK_V, QUEEN_V, KING_V, PAWN_V, KNIGHT_V, BISHOP_V, ROOK_V, QUEEN_V, KING_V};
int BitBoard::nPieceTypes = 13;


string BitBoard::developerInfo = "implemented moves\nrook [x], bishop [x], knight [x], queen [], king [x], pawn [*]";

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

	//TODO HACKS for testing
	//pieceTypeBB[1] = 0b0ULL; // no white pawns
	//pieceTypeBB[7] = 0b0ULL; // no black pawns
	//pieceTypeBB[1] = 0b0000000000000000000000000000000000000000000000001000000100000000ULL; // WhitePawns

	whiteBB = 0b0000000000000000000000000000000000000000000000001111111111111111ULL;
	blackBB = 0b1111111111111111000000000000000000000000000000000000000000000000ULL;
	allBB = 0b1111111111111111000000000000000000000000000000001111111111111111ULL;
	whiteMove = true;
	wMtrl = bMtrl = 8 * PAWN_V + 2 * ROOK_V + 2 * BISHOP_V + 2 * KNIGHT_V + QUEEN_V + KING_V;
	fullMoveCounter = 0;

	 squares = { WROOK, WKNIGHT, WBISHOP, WQUEEN, WKING, WBISHOP, WKNIGHT, WROOK,
				 WPAWN, WPAWN, WPAWN, WPAWN, WPAWN, WPAWN, WPAWN, WPAWN,
				 EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
				 EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
				 EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
				 EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
				 BPAWN, BPAWN, BPAWN, BPAWN, BPAWN, BPAWN, BPAWN, BPAWN,
				 BROOK, BKNIGHT, BBISHOP, BQUEEN, BKING, BBISHOP, BKNIGHT, BROOK};

	//cout << std::bitset<64>(pieceTypeBB[10]) << endl;
	//cout << std::bitset<64>(pieceTypeBB[4]) << endl;


	board = "................................................................";
}

BitBoard::BitBoard(const BitBoard& board) {
	//array<U64, 13>
	//pieceTypeBB = new long[nPieceTypes];
	for(int i = 0; i < nPieceTypes; i++) {
		pieceTypeBB[i] = board.pieceTypeBB[i];
	}

	whiteMove = board.whiteMove;

	for(int i = 0; i < squares.size(); i++) {
		squares[i] = board.squares[i];
	}

	wMtrl = board.wMtrl;
	bMtrl = board.bMtrl;
}

// convert char representation of piece to numeric board representation used internally
int BitBoard::charPieceToPieceType(int p) {
	int piece;

	switch (p) {
	case 'K':
		piece = WKING;
		break;
	case 'R':
		piece = WROOK;
		break;
	case 'N':
		piece = WKNIGHT;
		break;
	case 'B':
		piece = WBISHOP;
		break;
	case 'Q':
		piece = WQUEEN;
		break;
	case 'P':
		piece = WPAWN;
		break;
	case 'k':
		piece = BKING;
		break;
	case 'r':
		piece = BROOK;
		break;
	case 'n':
		piece = BKNIGHT;
		break;
	case 'b':
		piece = BBISHOP;
		break;
	case 'q':
		piece = BQUEEN;
		break;
	case 'p':
		piece = BPAWN;
		break;
	default:
		piece = EMPTY;
	}
	return piece;
}

BitBoard::BitBoard(FEN fen) {
	// TODO implement PositionBB byFen-constructor
	whiteMove = fen.getActiveColor() == 'w' ? true : false;

	//pieceTypeBB = new U64[nPieceTypes];

	/* Initialize Bitboards */
	for (int i = 0; i < nPieceTypes; i++) {
		 pieceTypeBB[i] = 0L;
	}
	whiteBB = blackBB = 0L;

	//squares = new int[64];

	// rnbqkbnr/pppp1ppp/8/8/8/8/PPPPQPPP/RNB1KBNR
	string piecestr = fen.getPiecePlacement();
	vector<string> rows = Utils::splitString(piecestr, '/');

	//pieces = new PieceList(this);
	for(int y = 7; y >= 0; y--) {
		string row = rows.at(7 - y);
		for(int x = 0, i = 0; x < 8; x++, i++) {
			if(i == row.length()) break;
			char p = row.at(i);
			if(isdigit(p)) {
				x += (p - 1) - '0'; // get numeric value
				continue;
			}
			int square = getSquare(x, y);
			int piece = charPieceToPieceType(p);
			//System.out.println("set " + piece + " to " + x + " / " + y);
			//System.out.println("set " + p + " to " + x + " / " + y);
			setPiece(square, piece);
			if(piece < 7)
				wMtrl += getPieceValue(piece);
			else
				bMtrl += getPieceValue(piece);
		}
	}
}

void BitBoard::init() {
	  initRookMagics();
	  initBishopMagics();
}

//TODO is compute_king incomplete?
U64 BitBoard::compute_king(U64 king_loc, U64 own_side)
{
	/* we can ignore the rank clipping since the overflow/underflow with
		respect to rank simply vanishes. We only care about the file
		overflow/underflow. */

	U64 king_clip_file_h = king_loc & LookUpTables::clearFile[FILE_H];
	U64 king_clip_file_a = king_loc & LookUpTables::clearFile[FILE_A];

	/* remember the representation of the board in relation to the bitindex
		when looking at these shifts.... */
	U64 spot_1 = king_clip_file_h << 7;
	U64 spot_2 = king_loc << 8;
	U64 spot_3 = king_clip_file_h << 9;
	U64 spot_4 = king_clip_file_h << 1;

	U64 spot_5 = king_clip_file_a >> 7;
	U64 spot_6 = king_loc >> 8;
	U64 spot_7 = king_clip_file_a >> 9;
	U64 spot_8 = king_clip_file_a >> 1;

	U64 king_moves = spot_1 | spot_2 | spot_3 | spot_4 | spot_5 | spot_6 |
						spot_7 | spot_8;

	U64 KingValid = king_moves & ~own_side;

	/* compute only the places where the king can move and attack. The caller
		will interpret this as a white or black king. */
	return KingValid;
}

U64 compute_knight(U64 knight_loc, U64 own_side) {

}

U64 compute_white_pawns(U64 white_pawn_loc, U64 all_pieces,
				U64 all_black_pieces) {

}

U64 compute_black_pawns(U64 black_pawn_loc, U64 all_pieces,
				U64 all_white_pieces) {

}

U64 BitBoard::whitePiecesValid(const BitBoard& board) {
	/*cout << "BitBoard::whitePiecesValid" << endl;
	whiteKingValid(board);
	cout << "king" << endl;
	whiteKnightsValid(board);
	cout << "knight" << endl;
	whiteBishopsValid(board);
	cout << "bishop" << endl;
	whiteRooksValid(board);
	cout << "rooks" << endl;*/
	return whiteKingValid(board) | whiteKnightsValid(board) |
	    			whiteBishopsValid(board) | whiteRooksValid(board) | whiteQueensValid(board) | whitePawnsValid(board);
}

U64 BitBoard::blackPiecesValid(const BitBoard& board) {
	//cout << "BitBoard::blackPiecesValid" << endl;
	return blackKingValid(board) | blackKnightsValid(board) |
	    			blackBishopsValid(board) | blackRooksValid(board) | blackQueensValid(board) | blackPawnsValid(board);
}

U64 BitBoard::whiteKingValid(const BitBoard& board) {
	return compute_king(board.pieceTypeBB[WKING], board.whiteBB);
}

U64 BitBoard::blackKingValid(const BitBoard& board) {
	return compute_king(board.pieceTypeBB[BKING], board.blackBB);
}

U64 BitBoard::whiteKnightsValid(const BitBoard& board) {
	return compute_knight(board.pieceTypeBB[board.WKNIGHT], board.whiteBB);
}

U64 BitBoard::blackKnightsValid(const BitBoard& board) {
	return compute_knight(board.pieceTypeBB[board.BKNIGHT], board.blackBB);
}

U64 BitBoard::whitePawnsValid(const BitBoard& board) {
	return compute_white_pawns(board.pieceTypeBB[board.WPAWN], board.allBB, board.blackBB);
}

U64 BitBoard::blackPawnsValid(const BitBoard& board) {
	return compute_black_pawns(board.pieceTypeBB[board.BPAWN], board.allBB, board.whiteBB);
}

U64 BitBoard::whiteBishopsValid(const BitBoard& board) {
	//cout << "BitBoard::whiteBishopsValid..." << endl;
    U64 squares = board.pieceTypeBB[board.WBISHOP];
    U64 m = 0x0ULL;
	while (squares != 0) {
        int sq = numberOfTrailingZeros(squares);
        m |= bishopAttacks(sq, board.allBB) & ~board.whiteBB;
        /*while (m != 0) {
            int sq1 = numberOfTrailingZeros(m);
            System.out.println(sq + " " + sq1);
            //setMove(moveList, sq, sq1, board.EMPTY);
            m &= (m - 1);
        }*/

        squares &= squares-1;
    }
	return m;
}

U64 BitBoard::blackBishopsValid(const BitBoard& board) {
    U64 squares = board.pieceTypeBB[board.BBISHOP];
    U64 m = 0x0L;
	while (squares != 0) {
        int sq = numberOfTrailingZeros(squares);
        m |= bishopAttacks(sq, board.allBB) & ~board.blackBB;

        /*while (m != 0) {
            int sq1 = numberOfTrailingZeros(m);
            System.out.println(sq + " " + sq1);
            //setMove(moveList, sq, sq1, board.EMPTY);
            m &= (m - 1);
        }*/

        squares &= squares-1;
    }
	return m;
}

U64 BitBoard::whiteRooksValid(const BitBoard& board) {
	U64 squares = board.pieceTypeBB[board.WROOK];
    U64 m = 0x0L;

	squares = board.pieceTypeBB[board.WROOK];
	while (squares != 0) {
        int sq = numberOfTrailingZeros(squares);
        m |= rookAttacks(sq, board.allBB) & ~board.whiteBB;
        squares &= squares-1;
    }

	//System.out.println(BitBoardUtils.bitboardToString(m, '1'));

	return m;
}

U64 BitBoard::blackRooksValid(const BitBoard& board) {
	U64 squares = board.pieceTypeBB[board.BROOK];
    U64 m = 0x0L;

	squares = board.pieceTypeBB[board.BROOK];
	while (squares != 0) {
        int sq = numberOfTrailingZeros(squares);
        m |= rookAttacks(sq, board.allBB) & ~board.blackBB;
        squares &= squares-1;
    }

	//System.out.println(BitBoardUtils.bitboardToString(m, '1'));

	return m;
}

U64 BitBoard::whiteQueensValid(const BitBoard& board) {
	U64 squares = board.pieceTypeBB[board.WQUEEN];
    U64 m = 0x0L;
	while (squares != 0) {
        int sq = numberOfTrailingZeros(squares);
        m |= bishopAttacks(sq, board.allBB) & ~board.whiteBB;
        squares &= squares-1;
    }

	squares = board.pieceTypeBB[board.WQUEEN];
	while (squares != 0) {
        int sq = numberOfTrailingZeros(squares);
        m |= rookAttacks(sq, board.allBB) & ~board.whiteBB;
        squares &= squares-1;
    }

	//System.out.println(BitBoardUtils.bitboardToString(m, '1'));

	return m;
}

U64 BitBoard::blackQueensValid(const BitBoard& board) {
	U64 squares = board.pieceTypeBB[board.BQUEEN];
    U64 m = 0x0L;
	while (squares != 0) {
        int sq = numberOfTrailingZeros(squares);
        m |= bishopAttacks(sq, board.allBB) & ~board.blackBB;
        squares &= squares-1;
    }

	squares = board.pieceTypeBB[board.BQUEEN];
	while (squares != 0) {
        int sq = numberOfTrailingZeros(squares);
        m |= rookAttacks(sq, board.allBB) & ~board.blackBB;
        squares &= squares-1;
    }

	//System.out.println(BitBoardUtils.bitboardToString(m, '1'));

	return m;
}

bool BitBoard::isRunning() {
	U64 validMoves(0LL);
	if(isWhiteMove()) {
		validMoves = whitePiecesValid(*this);
	} else
		validMoves = blackPiecesValid(*this);

	if(validMoves == 0LL) return false;
	else return true;
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

array<U64, 13> BitBoard::getPieceTypeBB() const {
	return pieceTypeBB;
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

/*BitBoard BitBoard::BitBoard copy() {
	return new BitBoard(this);
}*/

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

array<short, 64> BitBoard::getSquares() {
	return squares;
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

bool BitBoard::addMovesByMask(MoveList& moveList, int sq0, U64 mask) {
    while (mask != 0) {
    	//cout << "mask: " << std::bitset<64>(mask) << endl;
    	//cout << moveList.size() << endl;
        int sq = numberOfTrailingZeros(mask);
        //setMove(moveList, sq0, sq, Piece.EMPTY);
        moveList.push_back(Move(sq0, sq));
        mask &= (mask - 1);
    }
    //TODO BitBoard::addMovesByMask always returns false
    return false;
}

int BitBoard::getMoveNr() {
	return halfMoveClock;
}

bool BitBoard::isWhiteMove() {
	return whiteMove;
}

void BitBoard::setWhiteMove(bool whiteMove) {
	BitBoard::whiteMove = whiteMove;
}

/** Get the square index with the king on */
int BitBoard::getKingSq(bool isWhite) {
	if(isWhite)
    	for(int i = 0; i < squares.size(); i++) {
    		if(squares[i] == WKING) return i;
    	}
	else
    	for(int i = 0; i < squares.size(); i++) {
    		if(squares[i] == BKING) return i;
    	}
	return -1;
}

bool BitBoard::isInCheck(bool isWhite) {
	BitBoard temp(*this);
	int king;

	temp.setWhiteMove(!isWhite);

	// Is there a king on the board?
	try {
	king = temp.getKingSq(isWhite);
	} catch (exception e) {
		return true; //TODO handle no king error
	}

	//System.out.println("possible moves: " + temp.getPossibleMoves());

	/* Is any of the opponent's pieces threatening the king? */
	for(Move m : temp.getPseudoLegalMoves()) {
		//System.out.println(m + " " + m.getTarget8x8Index() + " =? " + king);
		if(m.getTarget8x8Index() == king) {
			//System.out.println(m + " is threatening the king");
			return true;
		}
	}

	return false;
}

int BitBoard::getWhiteMaterial() {
	return wMtrl;
}

int BitBoard::getBlackMaterial() {
	return bMtrl;
}

bool BitBoard::addPawnMovesByMask(MoveList& moveList, BitBoard pos, U64 mask,
	            int delta, bool allPromotions) {
	if (mask == 0)
		return false;
	U64 oKingMask = pos.pieceTypeBB[pos.isWhiteMove() ? BKING : WKING];
	if ((mask & oKingMask) != 0) {
		int sq = numberOfTrailingZeros(mask & oKingMask);
		moveList.push_back(Move(sq + delta, sq, EMPTY));
		return true;
	}
	U64 promMask = mask & LookUpTables::maskRow1Row8;
	mask &= ~promMask;
	while (promMask != 0) {
		int sq = numberOfTrailingZeros(promMask);
		int sq0 = sq + delta;
		if (sq >= 56) { // White promotion
			moveList.push_back(Move(sq0, sq, WQUEEN));
			moveList.push_back(Move(sq0, sq, WKNIGHT));
			if (allPromotions) {
				moveList.push_back(Move(sq0, sq, WROOK));
				moveList.push_back(Move(sq0, sq, WBISHOP));
			}
		} else { // Black promotion
			moveList.push_back(Move(sq0, sq, BQUEEN));
			moveList.push_back(Move(sq0, sq, BKNIGHT));
			if (allPromotions) {
				moveList.push_back(Move(sq0, sq, BROOK));
				moveList.push_back(Move(sq0, sq, BBISHOP));
			}
		}
		promMask &= (promMask - 1);
	}
	while (mask != 0) {
		int sq = numberOfTrailingZeros(mask);
		moveList.push_back(Move(sq + delta, sq, EMPTY));
		mask &= (mask - 1);
	}
	return false;
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

U64 BitBoard::addBishopRays(int x, int y, U64 occupied, bool inner) {
	U64 mask = 0;
    mask = addRay(mask, x, y,  1,  1, occupied, inner);
    mask = addRay(mask, x, y, -1, -1, occupied, inner);
    mask = addRay(mask, x, y,  1, -1, occupied, inner);
    mask = addRay(mask, x, y, -1,  1, occupied, inner);
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

array<vector<U64>, 64> BitBoard::bTables{};
array<U64, 64> BitBoard::bMasks;
int BitBoard::bBits[] = { 5, 4, 5, 5, 5, 5, 4, 5,
	            4, 4, 5, 5, 5, 5, 4, 4,
	            4, 4, 7, 7, 7, 7, 4, 4,
	            5, 5, 7, 9, 9, 7, 5, 5,
	            5, 5, 7, 9, 9, 7, 5, 5,
	            4, 4, 7, 7, 7, 7, 4, 4,
	            4, 4, 5, 5, 5, 5, 4, 4,
	            5, 4, 5, 5, 5, 5, 4, 5 };

void BitBoard::initRookMagics() { // Rook magics
	//rTables = new U64[64][];
	//rMasks = new U64[64];
	for (int sq = 0; sq < 64; sq++) {
		int x = getX(sq);
		int y = getY(sq);
		rMasks[sq] = addRookRays(x, y, 0ULL, true);
		int tableSize = 1 << rBits[sq];
		vector<U64> table (tableSize, -1);
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

void BitBoard::initBishopMagics() {
    for (int sq = 0; sq < 64; sq++) {
        int x = getX(sq);
        int y = getY(sq);
        bMasks[sq] = addBishopRays(x, y, 0L, true);
        int tableSize = 1 << bBits[sq];
        vector<U64> table (tableSize, -1);
        int nPatterns = 1 << std::bitset<64>(bMasks[sq]).count();
        for (int i = 0; i < nPatterns; i++) {
        	U64 p = createPattern(i, bMasks[sq]);
            int entry = (int)((p * LookUpTables::bMagics[sq]) >> (64 - bBits[sq]));
            U64 atks = addBishopRays(x, y, p, false);
            if (table[entry] == -1) {
                table[entry] = atks;
            } else if (table[entry] != atks) {
            	throw runtime_error("Error: Something went wrong in initBishopMagics()");
            }
        }
        bTables[sq] = table;
    }
}

U64 BitBoard::rookAttacks(int sq, U64 occupied) {
		/* cout << "occupied: " << std::bitset<64>(occupied) << endl;
		cout << "rMask   : " << std::bitset<64>(rMasks[sq]) << endl;
		cout << "rMagic  : " << std::bitset<64>(LookUpTables::rMagics[sq]) << endl;
		cout << "rBit    : " << std::bitset<64>(rBits[sq]) << endl; */
		return rTables[sq][(int)(((occupied & rMasks[sq]) * LookUpTables::rMagics[sq]) >> (64 - rBits[sq]))];
}

U64 BitBoard::bishopAttacks(int sq, U64 occupied) {
	/*cout << "bMask: " << to_string(bMasks[sq]) << endl;
	cout << "bMagic: " << to_string(LookUpTables::bMagics[sq]) << endl;
	cout << "bBits: " << to_string(bBits[sq]) << endl;
	cout << "index: " << to_string((int)(((occupied & bMasks[sq]) * LookUpTables::bMagics[sq]) >> (64 - bBits[sq]))) << endl;
	cout << bTables[sq][(int)(((occupied & bMasks[sq]) * LookUpTables::bMagics[sq]) >> (64 - bBits[sq]))] << endl;*/
	return bTables[sq][(int)(((occupied & bMasks[sq]) * LookUpTables::bMagics[sq]) >> (64 - bBits[sq]))];
}

/** Get the white rooks possible moves */
MoveList BitBoard::whiteRooksMoves(U64 squares, U64 allBB, U64 colorBB) {
U64 m = 0x0ULL;
MoveList possible;


while (squares != 0) {
	int sq = numberOfTrailingZeros(squares);
	m |= rookAttacks(sq, allBB) & ~whiteBB;
	//cout << "rookAttack: " << std::bitset<64>(m) << endl;

	// add move to movelist
	while (m != 0) {
		int sq1 = BitBoard::numberOfTrailingZeros(m);
		possible.push_back(Move(sq, sq1));
		m &= (m - 1);
	}
	//cout << "squares &= squares-1; " << endl;
	squares &= squares-1;
}

return possible;
}

/** Get the black rooks possible moves */
MoveList BitBoard::blackRooksMoves(U64 squares, U64 allBB, U64 colorBB) {
U64 m = 0x0ULL;
MoveList possible;


while (squares != 0) {
	int sq = numberOfTrailingZeros(squares);
	m |= rookAttacks(sq, allBB) & ~blackBB;
	//cout << "rookAttack: " << std::bitset<64>(m) << endl;

	// add move to movelist
	while (m != 0) {
		int sq1 = BitBoard::numberOfTrailingZeros(m);
		possible.push_back(Move(sq, sq1));
		m &= (m - 1);
	}
	//cout << "squares &= squares-1; " << endl;
	squares &= squares-1;
}

return possible;
}

U64 BitBoard::compute_king_incomplete(U64 king_loc, U64 own_side) {
	/* we can ignore the rank clipping since the overflow/underflow with
		respect to rank simply vanishes. We only care about the file
		overflow/underflow. */

	U64 king_clip_file_h = king_loc & LookUpTables::clearFile[FILE_H];
	U64 king_clip_file_a = king_loc & LookUpTables::clearFile[FILE_A];

	/* remember the representation of the board in relation to the bitindex
		when looking at these shifts.... */
	U64 spot_1 = king_clip_file_h << 7;
	U64 spot_2 = king_loc << 8;
	U64 spot_3 = king_clip_file_h << 9;
	U64 spot_4 = king_clip_file_h << 1;

	U64 spot_5 = king_clip_file_a >> 7;
	U64 spot_6 = king_loc >> 8;
	U64 spot_7 = king_clip_file_a >> 9;
	U64 spot_8 = king_clip_file_a >> 1;

	U64 king_moves = spot_1 | spot_2 | spot_3 | spot_4 | spot_5 | spot_6 |
						spot_7 | spot_8;

	U64 KingValid = king_moves & ~own_side;

	/* compute only the places where the king can move and attack. The caller
		will interpret this as a white or black king. */
	return KingValid;
}

U64 BitBoard::compute_white_pawns(U64 white_pawn_loc, U64 all_pieces,
		U64 all_black_pieces) {
	//TODO implement in BitBoard
}

U64 BitBoard::compute_black_pawns(U64 black_pawn_loc, U64 all_pieces,
				U64 all_white_pieces) {
	//TODO implement in BitBoard
}

U64 BitBoard::compute_knight(U64 knight_loc, U64 own_side) {
	//TODO implement in BitBoard
}

MoveList BitBoard::whitePawnMoves(BitBoard pos, U64 pawnsBB, U64 occupied, U64 blackBB) {
	MoveList possible;

	// Pawn moves
    U64 m = (pawnsBB << 8) & ~occupied;
    if (addPawnMovesByMask(possible, pos, m, -8, true)) return possible;
    m = ((m & LookUpTables::maskRow3) << 8) & ~occupied;
    //TODO implement PawnDoubleMovesByMask
    //addPawnDoubleMovesByMask(possible, pos, m, -16);

    int epSquare = pos.getEpSquare();
    U64 epMask = (epSquare >= 0) ? (1L << epSquare) : 0L;
    m = (pawnsBB << 7) & LookUpTables::maskAToGFiles & (blackBB | epMask);
    if (addPawnMovesByMask(possible, pos, m, -7, true)) return possible;

    m = (pawnsBB << 9) & LookUpTables::maskBToHFiles & (blackBB | epMask);
    if (addPawnMovesByMask(possible, pos, m, -9, true)) return possible;

    return possible;
}

MoveList BitBoard::blackPawnMoves(BitBoard pos, U64 pawnsBB, U64 occupied, U64 whiteBB) {
	MoveList possible;

	// Pawn moves
    U64 m = (pawnsBB >> 8) & ~occupied;
    if (addPawnMovesByMask(possible, pos, m, 8, true)) return possible;

    m = ((m & LookUpTables::maskRow6) >> 8) & ~occupied;
    //TODO implement PawnDoubleMovesByMask
    //addPawnDoubleMovesByMask(possible, pos, m, 16);

    int epSquare = pos.getEpSquare();
    U64 epMask = (epSquare >= 0) ? (1L << epSquare) : 0L;
    m = (pawnsBB >> 9) & LookUpTables::maskAToGFiles & (whiteBB | epMask);
    if (addPawnMovesByMask(possible, pos, m, 9, true)) return possible;

    m = (pawnsBB >> 7) & LookUpTables::maskBToHFiles & (whiteBB | epMask);
    if (addPawnMovesByMask(possible, pos, m, 7, true)) return possible;

    return possible;
}

MoveList BitBoard::whiteKingMoves() {
	MoveList possible;

	U64 kingAttacks = compute_king_incomplete(pieceTypeBB[WKING], whiteBB);
	//cout << "kingAttacks: " << std::bitset<64>(kingAttacks) << endl;
	U64 m = kingAttacks & ~whiteBB;
	//cout << "mask: " << std::bitset<64>(m) << endl;
    int sq = numberOfTrailingZeros(pieceTypeBB[WKING]);
    //cout << "sq: " << sq << endl;

    addMovesByMask(possible, sq, m);
    //cout << "nr of king moves: " << possible.size() << endl;

    return possible;
}

MoveList BitBoard::blackKingMoves() {
	MoveList possible;

	U64 kingAttacks = compute_king_incomplete(pieceTypeBB[BKING], blackBB);
	U64 m = kingAttacks & ~blackBB;
    int sq = numberOfTrailingZeros(pieceTypeBB[BKING]);

    addMovesByMask(possible, sq, m);

    return possible;
}


MoveList BitBoard::bishopsMoves(U64 squares, U64 allBB, U64 playerBB) {
    U64 m = 0x0ULL;
    MoveList possible;

	while (squares != 0) {
        int sq = numberOfTrailingZeros(squares);
        m |= bishopAttacks(sq, allBB) & ~playerBB;

        /* add move to movelist */
        while (m != 0) {
            int sq1 = numberOfTrailingZeros(m);
            //System.out.println(sq + " " + sq1);
            //setMove(moveList, sq, sq1, Piece.EMPTY);
            possible.push_back(Move(sq, sq1));
            m &= (m - 1);
        }

        squares &= squares-1;
    }
	return possible;
}

MoveList BitBoard::whiteKnightsMoves(U64 squares, U64 whiteBB) {
	MoveList possible;

    while (squares != 0) {
        int sq = numberOfTrailingZeros(squares);
        U64 m = LookUpTables::knightAttacks[sq] & ~whiteBB;
        //TODO BitBoard::addMovesByMask always returns false
        if (addMovesByMask(possible, sq, m)) return possible;
        squares &= squares-1;
    }

	return possible;
}

MoveList BitBoard::blackKnightsMoves(U64 squares, U64 blackBB) {
	MoveList possible;

    while (squares != 0) {
        int sq = numberOfTrailingZeros(squares);
        U64 m = LookUpTables::knightAttacks[sq] & ~blackBB;
        //TODO BitBoard::addMovesByMask always returns false
        if (addMovesByMask(possible, sq, m)) return possible;
        squares &= squares-1;
    }

	return possible;
}

MoveList BitBoard::queenMoves(U64 squares, U64 allBB, U64 playerBB) {
	U64 m = 0x0ULL;
	MoveList possible;

	while (squares != 0) {
		int sq = numberOfTrailingZeros(squares);
		m |= rookAttacks(sq, allBB) & ~playerBB;
		m |= bishopAttacks(sq, allBB) & ~playerBB;

		/* add move to movelist */
		while (m != 0) {
			int sq1 = numberOfTrailingZeros(m);
			//System.out.println(sq + " " + sq1);
			//setMove(moveList, sq, sq1, Piece.EMPTY);
			possible.push_back(Move(sq, sq1));
			m &= (m - 1);
		}

		squares &= squares-1;
	}
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
	MoveList moves;

	// TODO add rest of the moves
	if(whiteMove) {
		moves = whiteRooksMoves(pieceTypeBB[WROOK], allBB, whiteBB);
		possible.insert(end(possible), begin(moves), end(moves));

		moves = whiteKingMoves();
		possible.insert(end(possible), begin(moves), end(moves));

		moves = whitePawnMoves(*this, pieceTypeBB[WPAWN], occupied, blackBB);
		possible.insert(end(possible), begin(moves), end(moves));

		moves = bishopsMoves(pieceTypeBB[WBISHOP], allBB, whiteBB);
		possible.insert(end(possible), begin(moves), end(moves));

		moves = whiteKnightsMoves(pieceTypeBB[WKNIGHT], whiteBB);
		possible.insert(end(possible), begin(moves), end(moves));

		moves = queenMoves(pieceTypeBB[WQUEEN], allBB, whiteBB);
		possible.insert(end(possible), begin(moves), end(moves));
	} else {
		moves = blackRooksMoves(pieceTypeBB[BROOK], allBB, blackBB);
		possible.insert(end(possible), begin(moves), end(moves));

		moves = blackKingMoves();
		possible.insert(end(possible), begin(moves), end(moves));

		moves = blackPawnMoves(*this, pieceTypeBB[BPAWN], occupied, whiteBB);
		possible.insert(end(possible), begin(moves), end(moves));

		moves = bishopsMoves(pieceTypeBB[BBISHOP], allBB, blackBB);
		possible.insert(end(possible), begin(moves), end(moves));

		moves = whiteKnightsMoves(pieceTypeBB[BKNIGHT], blackBB);
		possible.insert(end(possible), begin(moves), end(moves));

		moves = queenMoves(pieceTypeBB[BQUEEN], allBB, blackBB);
		possible.insert(end(possible), begin(moves), end(moves));
	}
	//cout << "return pseudo legal" << endl;
	return possible;
}

MoveList BitBoard::getPossibleMoves() {
	MoveList possible = getPseudoLegalMoves();
	//possible = removeIllegal(this, possible); // remove illegal moves (check situations)
	//TODO test moves for debugging
	for(Move m : possible) {
		if(m.getSource8x8Index() > 63 || m.getSource8x8Index() < 0) cerr << "Illegal Move in possible Moves!";
		if(m.getTarget8x8Index() > 63 || m.getTarget8x8Index() < 0) cerr << "Illegal Move in possible Moves!";
	}
	return possible;
}

bool BitBoard::isWhitePiece(int p) {
	return p < 7 ? true : false;
}

int BitBoard::getPieceValue(int piece) {
	return pieceValue[piece];
}

void BitBoard::setPiece(int square, int piece) {
	int removedPiece = squares[square];
	if (removedPiece != EMPTY) {
		if (isWhitePiece(removedPiece)) {
			wMtrl -= getPieceValue(removedPiece);
		} else {
			bMtrl -= getPieceValue(removedPiece);
		}
	}
	squares[square] = piece;


	// Update bitboards
	const U64 sqMask = 1L << square;
	pieceTypeBB[removedPiece] &= ~sqMask;
	pieceTypeBB[piece] |= sqMask;

	if (removedPiece != EMPTY) {
		if (isWhitePiece(removedPiece)) {
			whiteBB &= ~sqMask;
		} else {
			blackBB &= ~sqMask;
		}
	}

	if (piece != EMPTY) {
		if (isWhitePiece(piece)) {
			whiteBB |= sqMask;
			wMtrl += getPieceValue(piece);
		}
		else {
			blackBB |= sqMask;
			bMtrl += getPieceValue(piece);
		}
	}
}

void BitBoard::movePieceNotPawn(int from, int to) {
	const int piece = squares[from];

	squares[from] = EMPTY;
	squares[to] = piece;

	U64 sqMaskF = 1ULL << from;
	U64 sqMaskT = 1ULL << to;
	pieceTypeBB[piece] &= ~sqMaskF;
	pieceTypeBB[piece] |= sqMaskT;
	if (isWhitePiece(piece)) {
		whiteBB &= ~sqMaskF;
		whiteBB |= sqMaskT;
		/*if (piece == WKING)
			wKingSq = to;*/
	} else {
		blackBB &= ~sqMaskF;
		blackBB |= sqMaskT;
		/*if (piece == BKING)
			bKingSq = to;*/
	}
}

void BitBoard::setCastleMask(int castleMask) {
	BitBoard::castleMask = castleMask; // TODO check assignment syntax castleMask
}

void BitBoard::setEpSquare(int epSquare) {
    if (BitBoard::epSquare != epSquare) { // TODO check epSquare comparison syntax
        //hashKey ^= epHashKeys[(this.epSquare >= 0) ? getX(this.epSquare) + 1 : 0];
        //hashKey ^= epHashKeys[(epSquare >= 0) ? getX(epSquare) + 1 : 0];
    	BitBoard::epSquare = epSquare;
    }
}

int BitBoard::getEpSquare() {
	return epSquare;
}

string BitBoard::str() {
	updateBitBoards();
	pieceTypeBB[0] = 0ULL; //TODO empty square bitboard is not up to date
	convertBitBoardTo8x8Board(); // create 8x8 board
	ostringstream result;
	string boardstr(board);

	/* Add newlines after each rank */
	for(int j = 64 - 8; j >= 0; j -= 8) {
		result << boardstr.substr(j, 8);
		result << '\n';
	}

	return result.str();
}

std::ostream& operator<<(std::ostream &strm, const BitBoard &b) {
	ostringstream result;
	string boardstr(b.board);

	/* Add newlines after each rank */
	for(int j = 64 - 8; j >= 0; j -= 8) {
		result << boardstr.substr(j, 8);
		result << '\n';
	}

	cout << result.str() << endl;
}

bool BitBoard::makeMove(Move move, UndoInfo ui) {
	//throw string("nicht vollständig implementiert");
	ui.capturedPiece = squares[move.getTarget8x8Index()];
	//cout << "capt. piece: " << ui.capturedPiece << endl;
	if(ui.capturedPiece < 0 || ui.capturedPiece > 13) throw BitBoardError(*this, "invalid captured piece " + to_string(ui.capturedPiece) + " in BitBoard::makeMove()");
		//throw string("invalid captured piece " + to_string(ui.capturedPiece));
	ui.castleMask = castleMask;
	//cout << "castleMask: " << castleMask << endl;
	ui.epSquare = epSquare;
	ui.halfMoveClock = halfMoveClock;
	bool wtm = whiteMove;
	//if(wtm) cout << "is white move" << endl;

	const int p = squares[move.getSource8x8Index()];
	int capP = squares[move.getTarget8x8Index()];
	long fromMask = 1L << move.getSource8x8Index();

	//int prevEpSquare = epSquare;
	//setEpSquare(-1);

	if ((capP != EMPTY) || (((pieceTypeBB[WPAWN] | pieceTypeBB[BPAWN]) & fromMask) != 0)) {
		halfMoveClock = 0;

		// Handle en passant and epSquare
		/*if (p == WPAWN) {
			if (move.getTarget8x8Index() - move.getSource8x8Index() == 2 * 8) {
				int x = getX(move.getTarget8x8Index());
				if (    ((x > 0) && (squares[move.getTarget8x8Index() - 1] == BPAWN)) ||
						((x < 7) && (squares[move.getTarget8x8Index() + 1] == BPAWN))) {
					setEpSquare(move.getSource8x8Index() + 8);
				}
			} else if (move.getTarget8x8Index() == prevEpSquare) {
				setPiece(move.getTarget8x8Index() - 8, EMPTY);
			}
		} else if (p == BPAWN) {
			if (move.getTarget8x8Index() - move.getSource8x8Index() == -2 * 8) {
				int x = getX(move.getTarget8x8Index());
				if (    ((x > 0) && (squares[move.getTarget8x8Index() - 1] == WPAWN)) ||
						((x < 7) && (squares[move.getTarget8x8Index() + 1] == WPAWN))) {
					setEpSquare(move.getSource8x8Index() - 8);
				}
			} else if (move.getTarget8x8Index() == prevEpSquare) {
				setPiece(move.getTarget8x8Index() + 8, EMPTY);
			}
		}

		if (((pieceTypeBB[WKING] | pieceTypeBB[BKING]) & fromMask) != 0) {
			if (wtm) {
				setCastleMask(castleMask & ~(1 << A1_CASTLE));
				setCastleMask(castleMask & ~(1 << H1_CASTLE));
			} else {
				setCastleMask(castleMask & ~(1 << A8_CASTLE));
				setCastleMask(castleMask & ~(1 << H8_CASTLE));
			}
		}*/

		// Perform move
		setPiece(move.getSource8x8Index(), EMPTY);
		// Handle promotion
		/*if (move.getPromoteTo() != EMPTY) {
			setPiece(move.getTarget8x8Index(), move.getPromoteTo());
		} else {*/
			setPiece(move.getTarget8x8Index(), p);
		//}
	} else {
		halfMoveClock++;

		// Handle castling
		/*if (((pieceTypeBB[WKING] | pieceTypeBB[BKING]) & fromMask) != 0) {
			int k0 = move.getSource8x8Index();
			if (move.getTarget8x8Index() == k0 + 2) { // O-O
				movePieceNotPawn(k0 + 3, k0 + 1);
			} else if (move.getTarget8x8Index() == k0 - 2) { // O-O-O
				movePieceNotPawn(k0 - 4, k0 - 1);
			}
			if (wtm) {
				setCastleMask(castleMask & ~(1 << A1_CASTLE));
				setCastleMask(castleMask & ~(1 << H1_CASTLE));
			} else {
				setCastleMask(castleMask & ~(1 << A8_CASTLE));
				setCastleMask(castleMask & ~(1 << H8_CASTLE));
			}
		}*/

		// Perform move
		movePieceNotPawn(move.getSource8x8Index(), move.getTarget8x8Index());
	}
	if (wtm) {
		// Update castling rights when rook moves
		/*if ((LookUpTables.maskCorners & fromMask) != 0) {
			if (p == WROOK)
				removeCastleRights(move.getSource8x8Index());
		}
		if ((LookUpTables.maskCorners & (1L << move.getTarget8x8Index())) != 0) {
			if (capP == BROOK)
				removeCastleRights(move.getTarget8x8Index());
		}*/
	} else {
		fullMoveCounter++;
		// Update castling rights when rook moves
		/*if ((LookUpTables.maskCorners & fromMask) != 0) {
			if (p == BROOK)
				removeCastleRights(move.getSource8x8Index());
		}
		if ((LookUpTables.maskCorners & (1L << move.getTarget8x8Index())) != 0) {
			if (capP == WROOK)
				removeCastleRights(move.getTarget8x8Index());
		}*/
	}

	//hashKey ^= whiteHashKey;
	whiteMove = !wtm;
	return true; // TODO return false on invalid move
}

bool BitBoard::makeMove(Move move) {
	UndoInfo ui;
	makeMove(move, ui);
}

bool BitBoard::unMakeMove(Move m, UndoInfo ui) {
	//hashKey ^= whiteHashKey;
	whiteMove = !whiteMove;
	int p = squares[m.getTarget8x8Index()];
	setPiece(m.getSource8x8Index(), p);
	setPiece(m.getTarget8x8Index(), ui.capturedPiece);
	setCastleMask(ui.castleMask);
	setEpSquare(ui.epSquare);
	halfMoveClock = ui.halfMoveClock;
	bool wtm = whiteMove;
	if (m.getPromoteTo() != EMPTY) {
		p = wtm ? WPAWN : BPAWN;
		setPiece(m.getSource8x8Index(), p);
	}
	if (!wtm) {
		fullMoveCounter--;
	}

	// Handle castling
	int king = wtm ? WKING : BKING;
	if (p == king) {
		int k0 = m.getSource8x8Index();
		if (m.getTarget8x8Index() == k0 + 2) { // O-O
			movePieceNotPawn(k0 + 1, k0 + 3);
		} else if (m.getTarget8x8Index() == k0 - 2) { // O-O-O
			movePieceNotPawn(k0 - 1, k0 - 4);
		}
	}

	// Handle en passant
	if (m.getTarget8x8Index() == epSquare) {
		if (p == WPAWN) {
			setPiece(m.getTarget8x8Index() - 8, BPAWN);
		} else if (p == BPAWN) {
			setPiece(m.getTarget8x8Index() + 8, WPAWN);
		}
	}
	return true; // TODO return false on invalid move
}
