/*
 * move.cpp
 *
 *  Created on: Jul 28, 2017
 *      Author: alex
 */

#include "move.h"
#include "bitboard.h"

Move::Move() {
	Move::src8x8 = -1;
	Move::trg8x8 = -1;
}

/*Move::Move(const Move& m) : src8x8(m.src8x8), trg8x8(m.trg8x8), promPiece(m.promPiece) {

}*/

Move::Move(int src8x8, int trg8x8) {
	Move::src8x8 = src8x8;
	Move::trg8x8 = trg8x8;
}

/** Create a Move by source and target indexes of board
	 * @param src board index of move's source
	 * @param trg board index of move'S target
	 * @param promPiece the piece to promote to
	 * */
Move::Move(int src8x8, int trg8x8, int promPiece) {
	createMove(src8x8, trg8x8, 1);
	Move::promPiece = promPiece;
}

void Move::createMove(int src, int trg, int type) {
	switch(type) {
	case 1: // create by 8x8 board indices
		Move::src8x8 = src;
		Move::trg8x8 = trg;

		break;
	//default: // create by 12x10 board indices

		/* Create 8x8 board index */
		//TODO implement if needed
		//Move::src8x8 = BitBoardUtils.index12x10ToBBSquare(src);
		//Move::trg8x8 = BitBoardUtils.index12x10ToBBSquare(trg);
	}
}

std::ostream& operator<<(std::ostream &strm, const Move &m) {
  //return strm << "(" << m.src8x8 << " " << m.trg8x8 << ")";

	//if(command != null) return command;
	/* create long algebraic move string */
	int src[]{BitBoard::getX(m.src8x8), BitBoard::getY(m.src8x8)};
	int trg[]{BitBoard::getX(m.trg8x8), BitBoard::getY(m.trg8x8)};
	//System.out.println(src[1] + " " + trg[1]);
	char alpha = (char) ((char) src[0] + 'a');
	char num = (char) ((char) src[1] + '0' + 1);
	char alpha1 = (char) ((char) trg[0] + 'a');
	char num1 = (char) ((char) trg[1] + '0' + 1);
	std::string command{alpha, num, alpha1, num1};

	/* add promotion piece */
	switch (m.promPiece) {
	case BitBoard::WQUEEN:
	case BitBoard::BQUEEN:
		command += "q";
		break;
	case BitBoard::WROOK:
	case BitBoard::BROOK:
		command += "r";
		break;
	case BitBoard::WBISHOP:
	case BitBoard::BBISHOP:
		command += "b";
		break;
	case BitBoard::WKNIGHT:
	case BitBoard::BKNIGHT:
		command += "n";
		break;
	default:
		break;
	}
	return strm << command;
}

int Move::getSource8x8Index() {
	return src8x8;
}

int Move::getTarget8x8Index() {
	return trg8x8;
}

int Move::getPromoteTo() {
	return promPiece;
}


bool Move::compare(std::array<short, 64> board_raw, Move a, Move b) {
	//TODO delete or link to MoveComparator
	return false;
}
