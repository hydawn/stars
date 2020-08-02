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
	string hiddenAnalyse(const char plr, shortv& list);
	string analyse(const char plr, shortv& list);
	string recursiveSituation(const char plr, shortv& list, short returnMoveDepth = 3, int recursiveCount = 0, int countTop = 3);

public:
	BoardState state;
	BoardAnalyse() : state(8, 8, 4) {}
	BoardAnalyse(BoardState board_) : state(board_) {}
	BoardAnalyse(const short r, const short c, const short w) : state(r, c, w) {}

	void show() { state.show(); }

	// analyse function
	/*
	 * I should bear in mind that the whole concept of analyse function is to
	 * narrow down the freeList, if not to gain "good" or "bad"
	 */
	string returnMove(const char plr, shortv& list, const short depth = 3);
	string returnSituation(const char plr, shortv& list, short returnMoveDepth = 3, int recursiveCount = 0, int countTop = 3);
	int	   respond(const char plr, oneMove& thisMove, bool showCal, bool showTime);

	// change board
	void go(const char plr, const short move);
	void reverse(const short column);

	// getter
	short getColTop(const short column) const { return state.top[column - 1]; }
	short getColumn() const { return state.column; }
	short getRow() const { return state.row; }

	// is function
	// else transfer
	bool colIsFull(const short col) { return state.colIsFull(col); }
	bool colIsEmpty(const short col) { return state.colIsEmpty(col); }
	char gameIsOver() { return state.isOver(); }
	bool boardIsFull() { return state.boardIsFull(); }
	char rPlayer(const char plr) { return state.rPlayer(plr); }

	// custom board
	void customBoard(const short cl, const short ro, const short wi) { state.customBoard(cl, ro, wi); }
};

#endif