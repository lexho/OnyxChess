/*
 * LookUpTables.h
 *
 *  Created on: Jul 28, 2017
 *      Author: alex
 */

#include <array>
#include "../datatypes.h"

#ifndef LOOKUPTABLES_H_
#define LOOKUPTABLES_H_

class LookUpTables {
	public:
	const static U64 clearFile[];

	const static U64 maskRank[];

	const static U64 maskRow1 = 0x00000000000000FFL;
	const static U64 maskRow2 = 0x000000000000FF00L;
	const static U64 maskRow3 = 0x0000000000FF0000L;
	const static U64 maskRow4 = 0x00000000FF000000L;
	const static U64 maskRow5 = 0x000000FF00000000L;
	const static U64 maskRow6 = 0x0000FF0000000000L;
	const static U64 maskRow7 = 0x00FF000000000000L;
	const static U64 maskRow8 = 0xFF00000000000000L;
	const static U64 maskRow1Row8 = 0xFF000000000000FFL;

	const static U64 maskAToGFiles = 0x7F7F7F7F7F7F7F7FL;
    const static U64 maskBToHFiles = 0xFEFEFEFEFEFEFEFEL;
    const static U64 maskAToFFiles = 0x3F3F3F3F3F3F3F3FL;
    const static U64 maskCToHFiles = 0xFCFCFCFCFCFCFCFCL;

	const static U64 bMagics[];

	const static U64 rMagics[];

	const static U64 maskCorners   = 0x8100000000000081L;

	static std::array<U64, 64> knightAttacks;

	static void init();

};



#endif /* LOOKUPTABLES_H_ */
