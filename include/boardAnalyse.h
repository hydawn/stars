#ifndef _BOARDANALYSE_H_
#define _BOARDANALYSE_H_ 1

#include <memory>
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

	std::shared_ptr<BoardState> state;
	RouteTree                   routes;
	int                         maxcaltime;

public:
	BoardAnalyse()
		: state(new BoardState(8, 8, 4)), routes(RouteTree()), maxcaltime(81) {}
	// copy the board
	BoardAnalyse(const BoardAnalyse& old)
		: state(new BoardState(*(old.state))),
		  routes(old.routes),
		  maxcaltime(old.maxcaltime) {}

	// this construct a new board
	BoardAnalyse(const BoardState& board_)
		: state(std::make_shared<BoardState>(board_)),
		  routes(RouteTree()),
		  maxcaltime(81) {}
	BoardAnalyse(const short r, const short c, const short w)
		: state(new BoardState(r, c, w)), routes(RouteTree()), maxcaltime(81) {}

	// analyse function
	/*
	 * I should bear in mind that the whole concept of analyse function is to
	 * narrow down the freeList, if not to gain "good" or "bad"
	 * */
	long long returnTime(
		const char plr, shortv& list, const int depth, string& word,
		const bool track);
	int respond(
		const char plr, oneMove& thisMove, bool showCal, bool showTime,
		bool starsOn, bool trackRoute);

	// recursive analyse
	string recursiveSituationTrackRoute(
		const char plr, shortv& list, int returnMoveDepth = 3,
		int recCount = 0);
	string recursiveSituationTrackRouteFirstRound(
		const char plr, shortv& list, int returnMoveDepth = 3,
		int recCount = 0);
	string recursiveSituation(
		const char plr, shortv& list, int returnMoveDepth = 3,
		int recCount = 0);
	string recursiveSituationFirstRound(
		const char plr, shortv& list, int returnMoveDepth = 3,
		int recCount = 0);
	string oneMoveAnalyseTrackRoute(
		const char plr, const short col, short goodNode, short badNode,
		const short depth = 0, const short maxDepth = 5);
	string oneMoveAnalyse(
		const char plr, const short col, const short depth = 0,
		const short maxDepth = 5);

	// change board
	void go(const char plr, const short move) { state->add(plr, move); }
	void reverse(const int column);

	// is function
	// getter
	short getColTop(const short column) const { return state->top[column - 1]; }
	short getCols() const { return state->cols; }
	short getRows() const { return state->rows; }
	// else transfer
	bool colIsFull(const short col) { return state->colIsFull(col); }
	bool colIsEmpty(const short col) { return state->colIsEmpty(col); }
	char gameIsOver() { return state->isOver(); }
	bool boardIsFull() { return state->boardIsFull(); }
	// show
	void show() { state->show(); }
	void starShow() { state->starShow(); }

	// custom board
	void customBoard(const short cl, const short ro, const short wi) {
		state->customBoard(cl, ro, wi);
	}

	// check matching
	void checkMatch();
	void clearMatch() {
		state->addNumber    = 0;
		state->removeNumber = 0;
	}
};

#endif // _BOARDANALYSE_H_
