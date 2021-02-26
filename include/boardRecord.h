#ifndef _BOARDRECORD_H_
#define _BOARDRECORD_H_ 1
#include <fstream>
#include <iostream>
#include "boardState.h"
#include "tools.h"

using std::cin;
using std::cout;
using std::endl;

typedef Json::Value::Members members;

class BoardInterface;

class BoardRecord {
	friend class BoardInterface;

private:
	vector<oneMove> historyMove;
	Json::Value     settings;
	Json::Value     games;
	string          gamesFileName     = "Stars_games.json";
	string          settingsFileName  = "Stars_settings.json";
	bool            settingsUnChanged = true;

public:
	BoardRecord() { getFile(); }
	BoardRecord(const string& gname, const string& sname)
		: gamesFileName(gname), settingsFileName(sname) {
		getFile();
	}
	~BoardRecord();

	// file control
	void getFile();
	void writeGames() const;
	void writeSettings() const;
	bool importInFileSettings(Json::Value* dest) const;

	// record
	void push_back(const oneMove& om) { historyMove.push_back(om); }
	void push_back(const oneMove&& om) { historyMove.push_back(om); }
	void pop_back() { historyMove.pop_back(); }

	// save & clear & refresh
	void saveGame(const BoardState& state);
	void saveGame(const string& gameName, const BoardState& state);
	void saveSettings() { writeSettings(); }
	// below is used to change board
	void clearHistoryMove() { historyMove.clear(); }

	// settings
	bool getDefaultSettings(const string& situ, const string& item) const;
	const Json::Value& getOtherSettings(const string& name) const;
	int         getDefaultSettingsItemNum() const {
        return settings["defaultSettings"].size();
	}
	void showSettingsWithTags() const;
	// change settings
	bool changeSettingsUsingTags(const int tag1, const int tag2);
	void changeOtherSettings(const string& name, const int time);

	// play back
	int        getNumberOfSavedBoard() const { return games.size(); }
	BoardState getState(const int& index) const {
		return games[index]["state"];
	}
	void   showSavedBoard(const Json::Value& state) const;
	string showSavedGames(Json::Value& ret);
	void   refreshHistoryMove(Json::Value& hm);

	// oper
	BoardRecord& operator=(const BoardRecord& br);

	// check
	bool match() const;
};

string toChinese(const string& word);
string toChinese(const bool word);

#endif // _BOARDRECORD_H_
