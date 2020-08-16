#ifndef _BOARDANALYSE_H_
#define _BOARDANALYSE_H_ 1

#include <string.h>
#include <chrono>
#include "boardRecord.h"

using std::string;
using namespace std::chrono;

class BoardAnalyse {
private:
	// analyse
	shortv firstPoint(const char plr);
	shortv firstPoint(const char plr, shortv& nfc);
	string analyse(const char plr, shortv& list);

public:
	BoardState state;
	BoardAnalyse() : state(8, 8, 4) {}
	BoardAnalyse(BoardState board_) : state(board_) {}
	BoardAnalyse(const short r, const short c, const short w) : state(r, c, w) {}

	// analyse function
	/*
	 * I should bear in mind that the whole concept of analyse function is to
	 * narrow down the freeList, if not to gain "good" or "bad"
	 */
	string returnMove(const char plr, shortv& list, const short depth);
	long long returnTime(const char plr, shortv& list, const short depth, string& word);
	long long recursiveTime(const char plr, shortv& list, const short returnMoveDepth, int countTop, string& word);
	int	   respond(const char plr, oneMove& thisMove, bool showCal, bool showTime, bool starsOn);

	// recursive analyse
	// simple and elegant, but too powerful for this game
	string returnSituation(const char plr, shortv& list, short returnMoveDepth = 3, int recursiveCount = 0, int countTop = 3);
	string recursiveSituation(const char plr, shortv& list, short returnMoveDepth = 3, int recursiveCount = 0, int countTop = 3);

	// change board
	void go(const char plr, const short move);
	void reverse(const short column);

	// is function
	// getter
	short getColTop(const short column) const { return state.top[column - 1]; }
	short getCols() const { return state.cols; }
	short getRows() const { return state.rows; }
	// else transfer
	bool colIsFull(const short col) { return state.colIsFull(col); }
	bool colIsEmpty(const short col) { return state.colIsEmpty(col); }
	char gameIsOver() { return state.isOver(); }
	bool boardIsFull() { return state.boardIsFull(); }
	char rPlayer(const char plr) { return state.rPlayer(plr); }
	// show
	void show() { state.show(); }
	void starShow() { state.starShow(); }

	// custom board
	void customBoard(const short cl, const short ro, const short wi) { state.customBoard(cl, ro, wi); }
};

#endif
