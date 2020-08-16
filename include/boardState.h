#ifndef _BOARDHANDLE_H_
#define _BOARDHANDLE_H_ 1

#include <stdio.h>
#include <time.h>
#include <stdexcept>
#include <string>
#include <vector>
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
	short* top;	 // 0~column, 0 means no piece, column means full
	short* starArea;
	short  row;
	short  column;
	short  winn;  // winning number?
	bool   starsOn;
	/************************debug***************************/
	static int removeNumber;
	static int addNumber;
	/************************debug***************************/

	BoardState() : row(8), column(8), winn(4), starsOn(false) { generate(); }
	BoardState(const BoardState& input) : row(input.row), column(input.column), winn(input.winn), starsOn(false) { generate(input.board, input.top); }
	BoardState(const Json::Value& root);
	BoardState(const short r, const short c, const short w) : row(r), column(c), winn(w), starsOn(false) { generate(); }
	~BoardState() { free(); }

	// construct
	void generate();
	void generate(char** b, const short* t);

	// destruct
	void free();

	// operator
	operator Json::Value() {
		Json::Value root;
		root["board"]  = boardToJson();
		root["top"]	   = topToJson();
		root["row"]	   = row;
		root["column"] = column;
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

	// getter
	short getRow() { return row; }
	short getColumn() { return column; }
	short getWinn() { return winn; }

	// is function
	bool colIsFull(const short col) { return top[col - 1] == row; }
	bool colIsEmpty(const short col) { return top[col - 1] == 0; }
	bool boardIsFull();
	bool winPieceNearBy(const short col, const short ro);
	char isOver();

	// small tools
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
		if (plr != 'X' && plr != '0')
			throw runtime_error("wrong player!\n");
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

	// performance
	void areaTopTransform();
	void areaTopRestore();
	void starShow();
	void setATopWithTop(short i, short t);
	void setATopWithNumber(short i, short n) {
		if (i >= 0 && i < column && starArea[i] < n)
			starArea[i] = n;
	}
	shortv aTopFullColumn();
	int	   starNumber();

	// history move
	void retInit(vector<oneMove>& his);
};

#endif
