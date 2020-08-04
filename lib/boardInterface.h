#ifndef _BOARDINTERFACE_H_
#define _BOARDINTERFACE_H_ 1
#include "boardAnalyse.h"

#define INTER_MAX_INPUT 64

using std::cin;
using std::string;

class BoardInterface {
private:
	BoardAnalyse* analyse;	// this might be more important?
	BoardRecord	  record;

public:
	BoardInterface() { generate(8, 8, 4); }
	BoardInterface(BoardAnalyse& hb) { analyse = &hb; }

	void generate(short c, short r, short w) { analyse = new BoardAnalyse; }

	// general
	string getInput(const string mode);
	string getInput(const string mode, char plr, double& inputTime);
	short  getCustomInput(const string item);
	void   getStateFromInput();

	// mode
	string addMode();
	string reverseMode();
	string debugMode(oneMove& byPlayer);
	string normalMode();
	string settingsMode();
	string playMode();
	string playBackMode();
	string customMode();
	void controlMode();

	// ask & do
	bool askToDebug(bool yes);	// if yes == true, then default yes
	void askToSaveBoard(bool yes);
	void importNewBoard();

	// info
	string getHelp(string mode = "normal");
	string getInfo(string input = "info");

	// refresh
	void refreshRecord(const BoardRecord& record_) { record = record_; }

	// show
	void showComment(oneMove& move);

	// check
	bool isOver(const oneMove& move);
};

#endif
