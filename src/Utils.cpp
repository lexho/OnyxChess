/*
 * Utils.cpp
 *
 *  Created on: Oct 17, 2017
 *      Author: alex
 */

#include "Utils.h"

vector<string> Utils::splitString(string s, char delim) {
	vector<string> splittedStr;
	size_t pos = 0;
	std::string token;
	int i = 0;
	while ((pos = s.find(delim)) != std::string::npos) {
	    token = s.substr(0, pos);
	    //std::cout << token << std::endl;
	    splittedStr[i] = s;
	    i++;
	    s.erase(0, pos + 1);
	}
	return splittedStr;
}
