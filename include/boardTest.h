#pragma once
#include "boardInterface.h"

// reload getInput to test
class BoardTest : public BoardInterface {
public:
	char		toWinn;
	static char lastMove;

	BoardTest() : BoardInterface(), toWinn('N') {}

    // virtual
    virtual string debugMode(oneMove& byPlayer);
	virtual bool   askToReverse(bool yes) { return false; }
	virtual void   askToSaveBoard(bool yes = false);
	virtual bool   isOver(const oneMove& move);

	// tools
	string getInput();
};
