#ifndef _BOARDHANDLE_H_
#define _BOARDHANDLE_H_ 1

#include <stdio.h>
#include <time.h>
#include <stdexcept>
#include <string>
#include <vector>
#include "shortList.h"

using std::logic_error;
using std::runtime_error;
using std::string;
using std::vector;
/*****
typedef vector<short>::iterator vIter;
typedef vector<short> shortv;
*****/
typedef ShortList::iterator vIter;
typedef ShortList			shortv;

class oneMove {
public:
	/*
	respond can give oneMove player, word,
	list, time(by computer),
	mode function can give oneMove mode, move, time(by a person), byComputer,
	suggestion, player(reverse & add), then push back to boardRecord
	*/
	/*
	debug has them all
	normal doesn't have suggestion
	add mode just have mode, move and player
	reverse mode just have mode and move
	*/
	bool   byComputer;	// is this move taken by the computer
	bool   hintOn;
	shortv list;
	string mode;
	short  move;  // the move that is actually been taken after the computer analyse
	char   player;
	short  suggestion;	// suggested move by the computer
	double time;		// time taken for the computer to respond
	string word;

	oneMove() : byComputer(true), hintOn(true), list(shortv()), mode(string()),
				player(' '), suggestion(0), time(0), word(string()) {}
	oneMove(Json::Value root) {
		byComputer = root["byComputer"].asBool();
		hintOn	   = root["hintOn"].asBool();
		list	   = root["list"];
		mode	   = root["mode"].asString();
		move	   = root["move"].asInt();
		player	   = root["player"].asInt();
		suggestion = root["suggestion"].asInt();
		time	   = root["time"].asDouble();
		word	   = root["word"].asString();
	}
	operator Json::Value() {
		Json::Value root;
		root["byComputer"] = byComputer;
		root["hintOn"]	   = hintOn;
		root["list"]	   = list;
		root["mode"]	   = mode;
		root["move"]	   = move;
		root["player"]	   = player;
		root["suggestion"] = suggestion;
		root["time"]	   = time;
		root["word"]	   = word;
		return root;
	}
	friend std::ostream &operator<<(std::ostream &os, oneMove &move);
};

class BoardState {
	// handles the memory allocate and ways to change the board
public:
	char** board;
	short* top;
	short* starArea;
	short  rows;
	short  cols;
	short  winn;  // winning number
	/************************debug***************************/
	static int removeNumber;
	static int addNumber;
	/************************debug***************************/

	BoardState() : rows(8), cols(8), winn(4) { generate(); }
	BoardState(const BoardState& input) : rows(input.rows), cols(input.cols), winn(input.winn) { generate(input.board, input.top); }
	BoardState(const Json::Value& root);
	BoardState(const short r, const short c, const short w) : rows(r), cols(c), winn(w) { generate(); }
	~BoardState() { free(); }

	// construct
	void generate();
	void generate(char** b, const short* t);

	// destruct
	void free();
	void freeBoard(char** b, int length);

	// operator
	operator Json::Value() {
		Json::Value root;
		root["board"]  = boardToJson();
		root["top"]	   = topToJson();
		root["row"]	   = rows;
		root["column"] = cols;
		root["winn"]   = winn;
		return root;
	}
	BoardState& operator=(const BoardState& bh);
	Json::Value boardToJson();
	template <typename T>
	Json::Value arraryToJson(T a[], int n);
	Json::Value topToJson();

	// show
	void show();
	void printHead();

	// getter
	short getWinn() { return winn; }

	// is function
	bool colIsFull(const short col) { return top[col - 1] == rows; }
	bool colIsEmpty(const short col) { return top[col - 1] == 0; }
	bool boardIsFull();
	bool winPieceNearBy(const short col, const short ro);
	bool winPieceButOne(const short col, const short ro, const short win);
	char isOver();

	// tools
	void nonFullColumn(shortv& nonFull);
	void sweepFullColumn(shortv& nonFull, short col);
	char rPlayer(const char plr);

	// random
	short randomMove();
	short randomMove(shortv& list);
	short randomSuggestion(const char plr, shortv& list, const string& mode = "progressive");
	short randomSuggestion(const char plr, shortv& list, shortv oppList, const string& mode = "progressive");

	// change function
	void add(const char plr, const short col) {
		/*****************************debug**********************************/
		// debug, remove this for performance reasons
		if (col < 0 || col > cols)
			throw logic_error("trying to add in a wrong place");
		if (top[col - 1] == rows)
			throw logic_error("trying to add to a full column.\n");
		if (plr != 'X' && plr != '0')
			throw logic_error("wrong player!\n");
		++addNumber;
		/*****************************debug**********************************/
		board[col - 1][top[col - 1]++] = plr;
	}
	void remove(const short col) {
		/*****************************debug**********************************/
		// debug, remove this for performance reasons
		if (col < 0 || col > cols)
			throw logic_error("trying to remove in a wrong place");
		if (top[col - 1] == 0)
			throw logic_error("trying to remove from an empty column.\n");
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
	bool match();

	// refresh
	void refreshBoard(char** b);
	void refreshTop();

	// custom
	void customBoard(const short cl, const short ro, const short wi);

	// performance
	void areaTopTransform();
	void areaTopRestore();
	void starShow();
	void setATopWithTop(short i, short t);
	void setATopWithNumber(short i, short n) {
		if (i >= 0 && i < cols && starArea[i] < n)
			starArea[i] = n;
	}
	shortv aTopFullColumn();
	int	   starNumber();
	int	   threeRowCount(const char plr, shortv& safeList);
	shortv makeThreeCols(const char plr, shortv& safeList);
	bool   specialPiece(const short col, const short ro);

	// history move
	void retInit(vector<oneMove>& his);
};

#endif
