#ifndef _BOARDHANDLE_H_
#define _BOARDHANDLE_H_ 1

#include <stdio.h>
#include <time.h>
#include <stdexcept>
#include <string>
#include <vector>
#include "json.h"
#include "shortList.h"

using std::runtime_error;
using std::string;
using std::vector;
/*****
typedef vector<short>::iterator vIter;
typedef vector<short> shortv;
*****/
typedef ShortList::iterator vIter;
typedef ShortList			shortv;

class BoardState {
	// handles the memory allocate and ways to change the board
public:
	char** board;
	short* top;	 // 0~column, 0 means no piece, column means full
	short  row;
	short  column;
	short  winn;  // winning number?
	/************************debug***************************/
	static int removeNumber;
	static int addNumber;
	/************************debug***************************/

	BoardState() : row(8), column(8), winn(4) { generate(); }
	BoardState(const BoardState& input) : row(input.row), column(input.column), winn(input.winn) { generate(input.board, input.top); }
	BoardState(const Json::Value& root);
	BoardState(const short r, const short c, const short w) : row(r), column(c), winn(w) { generate(); }
	~BoardState() { free(); }

	// construct
	void generate();
	void generate(char** b, const short* t);

	// destruct
	void free();

	// operator
	BoardState& operator=(const BoardState& bh);
				 operator Json::Value() {
		 Json::Value root;
		 root["board"]	= boardToJson();
		 root["top"]	= topToJson();
		 root["row"]	= row;
		 root["column"] = column;
		 root["winn"]	= winn;
		 return root;
	}
	Json::Value boardToJson();
	template <typename T>
	Json::Value arraryToJson(T a[], int n);
	Json::Value topToJson();

	// show
	void show();

	// getter
	short getRow() { return row; }
	short getColumn() { return column; }
	short getWinn() { return winn; }

	// is function
	bool colIsFull(const short col) { return top[col - 1] == column; }
	bool colIsEmpty(const short col) { return top[col - 1] == 0; }
	bool boardIsFull();
	bool winPieceNearBy(const short col, const short ro);
	char isOver();

	// return vector
	void nonFullColumn(shortv& nonFull);
	char rPlayer(const char plr);

	// random
	short randomMove();
	short randomMove(shortv& list);
	short randomSuggestion(const char plr, shortv& list, const string& mode = "progressive");
	short randomSuggestion(const char plr, shortv& list, shortv oppList, const string& mode = "progressive");

	// change function
	void add(const char plr, const short col) {
		/*****************************debug**********************************/
		// debug, feel free to remove this for performance reasons
		if (top[col - 1] == row)
			throw runtime_error("trying to add to a full column.\n");
		++addNumber;
		/*****************************debug**********************************/
		board[col - 1][top[col - 1]++] = plr;
	}
	void remove(const short col) {
		/*****************************debug**********************************/
		// debug, feel free to remove this for performance reasons
		if (top[col - 1] == 0)
			throw runtime_error("trying to remove from an empty column.\n");
		++removeNumber;
		/*****************************debug**********************************/
		board[col - 1][(top[col - 1]--) - 1] = ' ';
	}
	void remove(const short first, const short second, const short third) {
		remove(first);
		remove(second);
		remove(third);
	}
	// debug
	int getRemoveNumber() { return removeNumber; }
	int getAddNumber() { return addNumber; }

	// refresh
	void refreshBoard(char** b);
	void refreshTop();

	// custom
	void customBoard(const short cl, const short ro, const short wi);
};

#endif
