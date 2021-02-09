#ifndef _BOARDRECORD_H_
#define _BOARDRECORD_H_ 1
#include <fstream>
#include <iostream>
#include "boardState.h"

using std::cin;
using std::cout;
using std::endl;

typedef Json::Value::Members members;

class BoardRecord {
private:
	Json::Value settings;
	string      gamesFileName;
	string      settingsFileName;
	bool        settingsUnChanged;

public:
	vector<oneMove> historyMove;
	Json::Value     games;
	BoardRecord()
		: historyMove(vector<oneMove>()),
		  gamesFileName("Stars_games.json"),
		  settingsFileName("Stars_settings.json"),
		  settingsUnChanged(true) {
		getFile();
	}
	BoardRecord(const string& gname, const string& sname)
		: historyMove(vector<oneMove>()),
		  gamesFileName(gname),
		  settingsFileName(sname),
		  settingsUnChanged(true) {
		getFile();
	}
	~BoardRecord();

	// file control
	void getFile();
	void writeGames();
	void writeSettings();
	bool importInFileSettings(Json::Value* dest);

	// record
	void push_back(const oneMove& om) { historyMove.push_back(om); }
	void pop_back() { historyMove.pop_back(); }

	// save & clear & refresh
	void saveGame(BoardState& state);
	void saveGame(const string& gameName, BoardState& state);
	void saveSettings() { writeSettings(); }
	// below is used to change board
	void clearHistoryMove() { historyMove.clear(); }

	// settings
	bool         getDefaultSettings(const string& situ, const string& item);
	Json::Value& getOtherSettings(const string& name);
	int          getDefaultSettingsItemNum() {
        return settings["defaultSettings"].size();
	}
	void showSettingsWithTags();
	// change settings
	bool changeSettingsUsingTags(int tag1, int tag2);
	void changeOtherSettings(const string& name, const int time);

	// play back
	int        getNumberOfSavedBoard() { return games.size(); }
	BoardState getState(const int& index) { return games[index]["state"]; }
	BoardState getInitState(const int& index);
	void       showSavedBoard(const Json::Value& state);
	string     showSavedGames(Json::Value& ret);
	void       refreshHistoryMove(const Json::Value& hm);

	//oper
	BoardRecord& operator=(const BoardRecord& br);

	// check
	bool match();
};

string toChinese(const string& word);
string toChinese(const bool word);

#endif // _BOARDRECORD_H_
