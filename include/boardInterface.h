#ifndef _BOARDINTERFACE_H_
#define _BOARDINTERFACE_H_ 1
#include "boardAnalyse.h"

#define INTER_MAX_INPUT 256

using std::cin;
using std::getline;
using std::string;

class BoardInterface {
public:
	BoardAnalyse* analyse; // this might be more important?
	string        gamesFilename;
	string        settingsFilename;
	BoardRecord   record;
	oneMove       byOpponent;
	oneMove       byPlayer;

	BoardInterface();
	BoardInterface(BoardAnalyse& hb);
	virtual ~BoardInterface();

	void generate(short c, short r, short w) {
		analyse = new BoardAnalyse(c, r, w);
	}

	// general
	string getInput();
	string getInput(char plr, double& inputTime, const string& mode);
	short  getCustomInput(const string item);
	bool   getStateFromInput();
	bool   transformInput(
		  char** dest, vector<string>& src, const int cols, const int rows);

	// mode
	string         reverseMode();
	virtual string debugMode(const string& mode = "debug");
	string         settingsMode();
	string         defaultSettings();
	string         otherSettings();
	string         playbackMode();
	string         customMode();
	virtual bool   controlMode(const string& firstMode = "debug");
	string         showRoutesMode();

	// tools for mode
	void          add(string input);
	void          reverse(string input);
	bool          addStringConvert(string input);
	bool          addStringConvert(string input, oneMove& move);
	bool          reverseStringConvert(string input);
	bool          reverseStringConvert(string input, oneMove& move);
	virtual short respond();

	// ask & do
	virtual bool askToReverse(bool yes); // if yes == true, then default yes
	virtual void askToSaveBoard(bool yes);
	void         importNewBoard();

	// info
	string getHelp(string mode);
	string getInfo(string input);

	// refresh
	void refreshRecord(const BoardRecord& record_) { record = record_; }

	// show
	void showComment(oneMove& move);

	// check
	virtual bool isOver(const oneMove& move, const string& mode);
	int          autoTestMode(int startMove);
};

int  myStoi(string word);
bool xtoiFit(string word, int num);

#endif
