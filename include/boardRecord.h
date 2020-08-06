#ifndef _BOARDRECORD_H_
#define _BOARDRECORD_H_ 1
#include <fstream>
#include <iostream>

#include "boardState.h"

using std::cin;
using std::cout;
using std::endl;

typedef Json::Value::Members members;

class oneMove {
public:
	/*
	respond can give oneMove player, word,
	list, time(by computer),
	mode function can give oneMove mode, move, time(by a person), byComputer,
	suggestion, player(reverse & add), then push back to boardRecord
	*/
	/*
	debug has them all
	normal doesn't have suggestion
	add mode just have mode, move and player
	reverse mode just have mode and move
	*/
	bool   byComputer;	// is this move taken by the computer
	bool   hintOn;
	shortv list;
	string mode;
	short  move;  // the move that is actually been taken after the computer analyse
	char   player;
	short  suggestion;	// suggested move by the computer
	double time;		// time taken for the computer to respond
	string word;

	oneMove() : byComputer(true), hintOn(true), list(shortv()), mode(string()),
				player(' '), suggestion(0), time(0), word(string()) {}
	oneMove(Json::Value root) {
		byComputer = root["byComputer"].asBool();
		hintOn	   = root["hintOn"].asBool();
		list	   = root["list"];
		mode	   = root["mode"].asString();
		move	   = root["move"].asInt();
		player	   = root["player"].asInt();
		suggestion = root["suggestion"].asInt();
		time	   = root["time"].asDouble();
		word	   = root["word"].asString();
	}
	operator Json::Value() {
		Json::Value root;
		root["byComputer"] = byComputer;
		root["hintOn"]	   = hintOn;
		root["list"]	   = list;
		root["mode"]	   = mode;
		root["move"]	   = move;
		root["player"]	   = player;
		root["suggestion"] = suggestion;
		root["time"]	   = time;
		root["word"]	   = word;
		return root;
	}
	friend std::ostream &operator<<(std::ostream &os, oneMove &move);
};

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
	void		 showSavedBoard(const Json::Value &state);
	Json::Value *showSavedGames();
	void		 refreshHistoryMove(const Json::Value &hm);
	BoardState	 getState(const int &index) { return games[index]["state"]; }
};

#endif
