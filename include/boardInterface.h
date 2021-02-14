#ifndef _BOARDINTERFACE_H_
#define _BOARDINTERFACE_H_ 1
#include "boardAnalyse.h"

#define INTER_MAX_INPUT 256

using std::cin;
using std::getline;
using std::string;

// input
vector<string> inputBoard();
bool transformInput(
	vector<string>& dest, vector<string>& src, const int cols, const int rows);

// ask
bool askToReverse(const bool yes); // if yes == true, then default yes
bool askToSaveBoard(const bool yes);

// convert
int reverseStringConvert(string input);

// print
string getHelp(const string& mode);
string getInfo(const string& input);

class BoardInterface {
	friend class BoardTest;
	friend class BoardRecord;
	friend void autoTest(int n, const vector<string>& args);

private:
	BoardAnalyse* analyse; // this might be more important?
	BoardRecord   record;
	oneMove       byOpponent;
	oneMove       byPlayer;

public:
	BoardInterface();
	BoardInterface(const BoardAnalyse& hb);
	virtual ~BoardInterface();

	// general
	string getInput();
	string getInput(char plr, int64_t& inputTime, const string& mode);
	short  getCustomInput(const string item);
	bool   getStateFromInput();

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
	void          add(string& input);
	bool          reverse(string& input);
	bool          reverse(const int num);
	bool          reverseStringCheck(string& input);
	bool          addStringConvert(string& input);
	bool          addStringConvert(string input, oneMove& move);
	virtual short respond();

	// ask & do
	void importNewBoard();

	// refresh
	void refreshRecord(const BoardRecord& record_) { record = record_; }

	// print
	void showComment(const oneMove& move);

	// check
	virtual bool isOver(const oneMove& move, const string& mode);
};

#endif // _BOARDINTERFACE_H_
