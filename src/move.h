/*
 * move.h
 *
 *  Created on: Jul 25, 2017
 *      Author: alex
 */

#include <iostream>
#include <string>

#ifndef MOVE_H_
#define MOVE_H_

class Move {
	int src8x8;
	int trg8x8;
	int promPiece = 0;
	friend std::ostream& operator<<(std::ostream&, const Move &m);
public:
	Move();
	Move(int src8x8, int trg8x8);
};



#endif /* MOVE_H_ */
