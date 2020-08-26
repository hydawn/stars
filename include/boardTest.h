#pragma once
#include "boardInterface.h"

// reload getInput to test
class BoardTest : public BoardInterface {
public:
	char		toWinn;
	bool		showCalculate;
	bool		showTime;
	bool		lessPrint;
	bool		noPrint;
	static char lastMove;

	BoardTest() : BoardInterface(), toWinn('N'),
		showCalculate(record.getDefaultSettings("inDebugMode", "showCalculate")),
		showTime(record.getDefaultSettings("inDebugMode", "showTime")),
		lessPrint(false), noPrint(false) {}
	BoardTest(const string& option);

	// virtual
	virtual bool   askToReverse(bool yes) { return false; }
	virtual void   askToSaveBoard(bool yes = false);
	virtual bool   isOver(const oneMove& move);
    virtual string debugMode(oneMove& byPlayer);
	virtual bool   controlMode();

	// tools
	string getInput();
};

void autoTest(int n, string option = string());
