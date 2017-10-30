/*
 * Utils.h
 *
 *  Created on: Oct 17, 2017
 *      Author: alex
 */

#ifndef UTILS_H_
#define UTILS_H_

#include <vector>
#include <string>

using namespace std;

class Utils {
public:
	static vector<string> splitString(string piecestr, char delim);
};

#endif /* UTILS_H_ */
