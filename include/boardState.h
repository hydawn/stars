#ifndef _BOARDHANDLE_H_
#define _BOARDHANDLE_H_ 1

#include <stdio.h>
#include <time.h>
#include <stdexcept>
#include <string>
#include <vector>
#include "tools.h"

using std::logic_error;
using std::runtime_error;
using std::string;
using std::vector;
typedef vector<int> shortv;

class BoardState;
class BoardAnalyse;
class BoardInterface;
class BoardTest;

class oneMove {
	friend class BoardState;
	friend class BoardAnalyse;
	friend class BoardInterface;
	friend class BoardTest;
	friend void autoTest(int n, const vector<string>& args);

private:
	/*
	 * respond can give oneMove player, word, list, time(by computer),
	 * mode function can give oneMove mode, move, time(by a person),
	 * byComputer, suggestion, player(reverse & add), then push back to
	 * boardRecord debug has them all normal doesn't have suggestion add
	 * mode just have mode, move and player reverse mode just have mode and
	 * move
	 * */
	string mode;
	string word;
	shortv list;
	double time = 0; // time taken for the computer to respond
	// the move that is been taken after the computer analyse
	int  move       = 0;
	int  suggestion = 0; // suggested move by the computer
	char player     = '0';
	bool byComputer = true; // is this move taken by the computer
	bool hintOn     = true;

public:
	oneMove() {}
	oneMove(const Json::Value& root);
						 operator Json::Value();
	friend std::ostream& operator<<(std::ostream& os, oneMove& move);
};

class BoardState {
	// handles the memory allocate and ways to change the board
	friend class BoardAnalyse;
	friend class BoardInterface;

private:
	vector<string> board;
	vector<int>    top;
	vector<int>    starArea;
	int            rows;
	int            cols;
	int            winn;
	static int     removeNumber;
	static int     addNumber;

public:
	BoardState() : BoardState(8, 8, 4) {}
	BoardState(const Json::Value& root);
	BoardState(const int r, const int c, const int w)
		: board(vector<string>(c, string(r, ' '))),
		  top(vector<int>(c, 0)),
		  starArea(vector<int>(c, r)),
		  rows(r),
		  cols(c),
		  winn(w) {}

	// operator
	operator Json::Value() {
		Json::Value root;
		root["board"]  = std::move(boardToJson());
		root["top"]    = std::move(topToJson());
		root["row"]    = rows;
		root["column"] = cols;
		root["winn"]   = winn;
		return root;
	}
	Json::Value boardToJson();
	Json::Value topToJson();

	// show
	void show() const;
	void printHead() const;

	// check
	bool colCanAdd(const int col);
	bool colCanRemove(const int col);

	// getter
	int  getWinn() { return winn; }
	char getTopPiece(const int col);

	// is function
	bool colIsFull(const int col) { return top.at(col - 1) == rows; }
	bool colIsEmpty(const int col) { return top.at(col - 1) == 0; }
	bool boardIsFull();
	bool winPieceNearBy(const int col, const int ro);
	bool winPieceButOne(const int col, const int ro, const int win);
	char isOver();

	// tools
	void nonFullColumn(shortv& nonFull);
	void sweepFullColumn(shortv& nonFull, int col);
	char rPlayer(const char plr) { return plr ^ 0x68; }
	int  pieceCount();

	// random
	int randomMove();
	int randomMove(shortv& list);
	int randomSuggestion(
		const char plr, shortv& list, const string& mode = "progressive");
	int randomSuggestion(
		const char plr, shortv& list, shortv oppList,
		const string& mode = "progressive");

	// change board
	void add(const char plr, const int col) {
#ifdef STARS_DEBUG_INFO
		if (col < 1 || col > cols)
			throw logic_error("trying to add in a wrong place");
		if (top[col - 1] == rows)
			throw logic_error("trying to add to a full column");
		if (plr != 'X' && plr != '0')
			throw logic_error("wrong player!");
		++addNumber;
#endif
		board[col - 1][top[col - 1]++] = plr;
	}
	void remove() {}
	template <typename T, typename... Args>
	void remove(const T col, Args... args) {
#ifdef STARS_DEBUG_INFO
		if (col < 1 || col > cols)
			throw logic_error("trying to remove in a wrong place");
		if (top[col - 1] == 0)
			throw logic_error("trying to remove from an empty column");
		++removeNumber;
#endif
		board[col - 1][(top[col - 1]--) - 1] = ' ';
		remove(args...);
	}
	// debug
	bool match() { return removeNumber == addNumber; }
	bool valid();

	// custom
	void customBoard(const int cl, const int ro, const int wi);

	// performance
	void areaTopTransform();
	void areaTopRestore();
	void starShow();
	void setATopWithTop(const int i, const int t);
	void setATopWithNumber(const int i, const int n);
	shortv aTopFullColumn();
	int    starNumber();
	int    threeRowCount(const char plr, shortv& safeList);
	shortv makeThreeCols(const char plr, shortv& safeList);
	bool   specialPiece(const int col, const int ro);

	// input a new board
	void refreshTop() noexcept;

	// history move
	void retInit(const vector<oneMove>& history);
};

#endif // _BOARDHANDLE_H_
