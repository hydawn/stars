#ifndef _BOARDANALYSE_H_
#define _BOARDANALYSE_H_ 1

#include <string.h>
#include <chrono>
#include "boardRecord.h"
#include "boardRoute.h"

using std::string;
using namespace std::chrono;

class BoardAnalyse {
	friend class BoardTest;
	friend class BoardInterface;
	friend void autoTest(int n, const vector<string>& args);

private:
	// analyse
	shortv firstPoint(const char plr, shortv& nfc);
	string analyse(const char plr, shortv& list);

	int        maxcaltime;
	BoardState state;
	RouteTree  routes;
public:
	BoardAnalyse() : state(8, 8, 4), routes(RouteTree()), maxcaltime(81) {}
	BoardAnalyse(BoardAnalyse& ba)
		: maxcaltime(ba.maxcaltime), state(ba.state), routes(ba.routes) {}
	BoardAnalyse(BoardState board_)
		: state(board_), routes(RouteTree()), maxcaltime(81) {}
	BoardAnalyse(const short r, const short c, const short w)
		: state(r, c, w), routes(RouteTree()), maxcaltime(81) {}

	// analyse function
	/*
	 * I should bear in mind that the whole concept of analyse function is to
	 * narrow down the freeList, if not to gain "good" or "bad"
	 */
	string oneMoveAnalyseDebug(
		const char plr, const short col, short goodNode, short badNode,
		const short depth, const short maxDepth = 5);
	string oneMoveAnalyse(
		const char plr, const short col, const short depth,
		const short maxDepth = 5);
	long long returnTime(
		const char plr, shortv& list, const int depth, string& word);
	int respond(
		const char plr, oneMove& thisMove, bool showCal, bool showTime,
		bool starsOn, bool trackRoute);

	// recursive analyse
	string recursiveSituationRoute(
		const char plr, shortv& list, int returnMoveDepth = 3,
		int recCount = 0, int countTop = 3);
	string recursiveSituation(
		const char plr, shortv& list, int returnMoveDepth = 3,
		int recCount = 0, bool firstRound = true);

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
	void customBoard(const short cl, const short ro, const short wi) {
		state.customBoard(cl, ro, wi);
	}

	// check matching
	void checkMatch();
	void clearMatch() {
		state.addNumber    = 0;
		state.removeNumber = 0;
	}
};

#endif // _BOARDANALYSE_H_
