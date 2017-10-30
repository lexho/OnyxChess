#include <exception>
#include <sstream>
#include <bitset>
#include "../bitboard.h"

class BitBoardError : public std::exception {
	BitBoard board;
public:
	BitBoardError(BitBoard board, const std::string& message) : board(board), msg_(message){}
	string getDebugInfo() {
		std::stringstream ss;
		if(board.isRunning()) ss << "board is running";
		else ss << "board is not running";
		ss << endl;
		//board.printBitBoards();

		ss << "bitboards:" << endl;
		for(U64 bitboard : board.getPieceTypeBB()) {
			ss << std::bitset<64>(bitboard) << endl;
		}

		ss << "squares: " << endl;
		for(short sq : board.getSquares()) {
			ss << to_string(sq) << " ";
		}
		ss << endl;

		return ss.str();
		//board.printBitBoards();
		/*if(board.str().size() == 0) return "<empty board string>";
		else return "else";//to_string(board.str().size());*/
	}
	const char* what() const throw (){
		std::stringstream ss;
		//ss << msg_ << board << endl;
		return msg_.c_str();
	    //return ss.str().c_str();
	}
protected:
    /** Error message.
     */
    std::string msg_;
};
