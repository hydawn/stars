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
	Json::Value strategy;
	string		gamesFileName;
	string		settingsFileName;

	// file control
	void getFile();
	void writeGames();
	void writeSettings();

public:
	vector<oneMove> historyMove;
	Json::Value		games;
	BoardRecord() : historyMove(vector<oneMove>()), gamesFileName("Stars_games.json"),
		settingsFileName("Stars_settings.json") { getFile(); }

	// record
	void push_back(const oneMove &om) { historyMove.push_back(om); }
	void pop_back() { historyMove.pop_back(); }

	// save & clear & refresh
	void saveGame(BoardState &state);
	void saveGame(const string &gameName, BoardState &state);
	void saveSettings() { writeSettings(); }
	// below is used to change board
	void clearHistoryMove() { historyMove.clear(); }

	// settings
	bool getSettings(const string &situ, const string &item);
	int	 getSettingsItemNumber() { return settings.size(); }
	void showSettingsWithTags();
	bool changeSettingsUsingTags(int tag1, int tag2);

	// play back
	int			 getNumberOfSavedBoard() { return games.size(); }
	BoardState	 getState(const int &index) { return games[index]["state"]; }
	BoardState	 getInitState(const int &index);
	void		 showSavedBoard(const Json::Value &state);
	string		 showSavedGames(Json::Value& ret);
	void		 refreshHistoryMove(const Json::Value &hm);
};

#endif
