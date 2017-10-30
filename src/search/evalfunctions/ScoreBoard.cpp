/*
 * ScoreBoard.cpp
 *
 *  Created on: Sep 10, 2017
 *      Author: alex
 */

#include <bitset>
#include "../evalfunctions/ScoreBoard.h"

ScoreBoard::ScoreBoard() {

}

/**
 * This particular scoring function will get constructed each time a search is run.
 * It gets passed the starting state of the board, so it can judge wether a sequence
 * of moves during the search has led to a better state!
 * @param start
 * @param player_id
 * @param opponent_id
 */
ScoreBoard::ScoreBoard(const BitBoard& board) : start(start) {
	//this.start = start;
	if(start.isWhiteMove())
		player_id = WHITE;
	else
		player_id = BLACK;
	opponent_id = !player_id;
	scoreCounter = 0;

}

ScoreBoard::~ScoreBoard() {
	// TODO Auto-generated destructor stub
}

int ScoreBoard::apply(BitBoard board) {
	//TODO implement ScoreBoard::apply
	//cout << "evaluate" << endl;
	scoreCounter++;
	position = board;

	//board.print();

	// nobody won or lost so far
	if (board.isRunning())
	  {
		int score = 0;

		/* scoring dimensions */
		int mtrl = scoreMaterial(board);
		int mobility = scoreMobility(board) * 2;
		int pos = scorePosition(board);

		score = mtrl + mobility + pos;
		if(player_id == BLACK) score *= -1;

		//if(mtrl > 0) System.out.println("material: " + mtrl + ", mobility: " + mobility + ", position: " + pos);

		return score;
	  } else
	{
		// Who is checkmate?
		if(board.isInCheck(opponent_id)) {
			return 100000;
		} else if (board.isInCheck(player_id)) {
			return -100000;
		} else {
			return 0;
		}
	}
}

int ScoreBoard::scoreMaterial(BitBoard board) {
	int score;
	score = board.getWhiteMaterial();
	score -= board.getBlackMaterial();
	return score;
}

/* side to move relative score */
int ScoreBoard::scoreMobility(BitBoard board) {
	/* get number of pieces and active color */
	//TODO fix bitboard move count
	board.updateBitBoards();
	U64 validMovesW = BitBoard::whitePiecesValid(board);
	U64 validMovesB = BitBoard::blackPiecesValid(board);

	int wMobility = std::bitset<64>(validMovesW).count(); // number of valid white moves
	int bMobility = std::bitset<64>(validMovesB).count(); // number of valid black moves

	/*System.out.println(BitBoardUtils.bitboardToString(validMovesW, 'w'));
	System.out.println(BitBoardUtils.bitboardToString(validMovesB, 'b'));
	System.out.println("w mob: " + wMobility + ", b mob: " + bMobility);*/

	/* compute score */
	int score = (wMobility - bMobility);

	return score;
}

int ScoreBoard::scorePosition(BitBoard board) {
	int score = 0;

	array<short, 64> squares = board.getSquares();

	for(int i = 0; i < squares.size(); i++) {
		switch (squares[i]) {
		case BitBoard::WPAWN:
			score += PawnTable[i];
			break;
		case BitBoard::WKNIGHT:
			score += KnightTable[i];
			break;
		case BitBoard::WBISHOP:
			score += BishopTable[i];
			break;
		case BitBoard::WKING:
			score += KingTable[i];
			//TODO score king end game
			break;
		case BitBoard::BPAWN:
			//score -= PawnTable[(int)(((int)(i + 56)) - (int)((int)(i / 8) * 16))];
			score -= PawnTable[i ^ 56];
			break;
		case BitBoard::BKNIGHT:
			//score -= KnightTable[(int)(((int)(i + 56)) - (int)((int)(i / 8) * 16))];
			score -= KnightTable[i ^ 56];
			break;
		case BitBoard::BBISHOP:
			//score -= BishopTable[(int)(((int)(i + 56)) - (int)((int)(i / 8) * 16))];
			score -= BishopTable[i ^ 56];
			break;
		case BitBoard::BKING:
			//score -= KingTable[(int)(((int)(i + 56)) - (int)((int)(i / 8) * 16))];
			score -= KingTable[i ^ 56];
			//TODO score king end game
			break;
		}
	}
}

std::array<short, 64> ScoreBoard::PawnTable = {
	 0,  0,  0,  0,  0,  0,  0,  0,
	50, 50, 50, 50, 50, 50, 50, 50,
	10, 10, 20, 30, 30, 20, 10, 10,
	 5,  5, 10, 27, 27, 10,  5,  5,
	 0,  0,  0, 25, 25,  0,  0,  0,
	 5, -5,-10,  0,  0,-10, -5,  5,
	 5, 10, 10,-25,-25, 10, 10,  5,
	 0,  0,  0,  0,  0,  0,  0,  0
};

std::array<short, 64> ScoreBoard::KnightTable = {
	-50,-40,-30,-30,-30,-30,-40,-50,
	-40,-20,  0,  0,  0,  0,-20,-40,
	-30,  0, 10, 15, 15, 10,  0,-30,
	-30,  5, 15, 20, 20, 15,  5,-30,
	-30,  0, 15, 20, 20, 15,  0,-30,
	-30,  5, 10, 15, 15, 10,  5,-30,
	-40,-20,  0,  5,  5,  0,-20,-40,
	-50,-40,-20,-30,-30,-20,-40,-50,
};

std::array<short, 64> ScoreBoard::BishopTable = {
	-20,-10,-10,-10,-10,-10,-10,-20,
	-10,  0,  0,  0,  0,  0,  0,-10,
	-10,  0,  5, 10, 10,  5,  0,-10,
	-10,  5,  5, 10, 10,  5,  5,-10,
	-10,  0, 10, 10, 10, 10,  0,-10,
	-10, 10, 10, 10, 10, 10, 10,-10,
	-10,  5,  0,  0,  0,  0,  5,-10,
	-20,-10,-40,-10,-10,-40,-10,-20,
};

std::array<short, 64> ScoreBoard::KingTable = {
	-30, -40, -40, -50, -50, -40, -40, -30,
	-30, -40, -40, -50, -50, -40, -40, -30,
	-30, -40, -40, -50, -50, -40, -40, -30,
	-30, -40, -40, -50, -50, -40, -40, -30,
	-20, -30, -30, -40, -40, -30, -30, -20,
	-10, -20, -20, -20, -20, -20, -20, -10,
	 20,  20,   0,   0,   0,   0,  20,  20,
	 20,  30,  10,   0,   0,  10,  30,  20
};

std::array<short, 64> ScoreBoard::KingTableEndGame = {
	-50,-40,-30,-20,-20,-30,-40,-50,
	-30,-20,-10,  0,  0,-10,-20,-30,
	-30,-10, 20, 30, 30, 20,-10,-30,
	-30,-10, 30, 40, 40, 30,-10,-30,
	-30,-10, 30, 40, 40, 30,-10,-30,
	-30,-10, 20, 30, 30, 20,-10,-30,
	-30,-30,  0,  0,  0,  0,-30,-30,
	-50,-30,-30,-30,-30,-30,-30,-50,
};
