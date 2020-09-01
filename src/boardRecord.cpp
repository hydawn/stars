/*-- encoding: UTF-8 --*/
#include "boardRecord.h"

// hard core code
string addon		   = "";
string inFileSettings = addon +
	"{\n" +
	"	\"defaultSettings\": {\n" +
	"		\"changeBoard\": {\n" +
	"			\"askToSaveBoard\": false,\n" +
	"			\"defaultSaveBoard\": false\n" +
	"		},\n" +
	"		\"gameIsOver\": {\n" +
	"			\"askToReverse\": true,\n" +
	"			\"askToSaveBoard\": true,\n" +
	"			\"defaultReverse\": false,\n" +
	"			\"defaultSaveBoard\": false\n" +
	"		},\n" +
	"		\"inCustomMode\": {\n" +
	"			\"askToSaveBoard\": false,\n" +
	"			\"defaultSaveBoard\": false\n" +
	"		},\n" +
	"		\"inDebugMode\": {\n" +
	"			\"hintOn\": false,\n" +
	"			\"showCalculate\": false,\n" +
	"			\"showTime\": false,\n" +
	"			\"starrySky\": false,\n" +
	"			\"starsOn\": false,\n" +
	"			\"trackRoutes\": false\n" +
	"		},\n" +
	"		\"whenSaveGame\": {\n" +
	"			\"askGiveName\": true,\n" +
	"			\"defaultGiveName\": false\n" +
	"		}\n" +
	"	},\n" +
	"	\"otherSettings\": {\n" +
	"		\"maxcaltime\":81\n" +
	"	}\n" +
	"}\n";

BoardRecord::~BoardRecord() {
	try {
		match();
	}
	catch (const std::logic_error& e) {
		cout << "logic_error when destory BoardRecord:\n\t";
		std::cerr << e.what() << '\n';
	}
	catch (const std::runtime_error& e) {
		cout << "runtime_error when destory BoardRecord:\n\t";
		std::cerr << e.what() << '\n';
	}
	if(!settingsUnChanged)
		writeSettings();
	if (!games.empty())
		writeGames();
}

std::ostream& operator<<(std::ostream& os, oneMove& move) {
	os << "    mode = " << move.mode << ";\n";
	

	if (move.mode == "add")
		os << "    add '" << move.player << "' in column " << move.move << endl;
	else if (move.mode == "reverse")
		os << "    remove column " << move.move << endl;
	else if (
		move.mode == "debug" || move.mode == "test-debug" ||
		move.mode == "test-play") {
		if (move.mode != "debug" && move.mode != "test-debug" &&
			move.mode != "test-play")
			os << "this mode " << move.mode
			   << " is not supported, but treated as debug mode\n";
		os << "    time used: " << move.time
		   << "ms; hint on: " << std::boolalpha << move.hintOn
		   << "; suggestion = " << move.suggestion
		   << "\n    word = " << move.word << "; list = [ ";
		for (short i : move.list)
			os << i << " ";
		os << "]\n    move = " << move.move;
		if (move.byComputer)
			os << " by computer '" << move.player << "':\n";
		else
			os << " by player '" << move.player << "':\n";
	}
	return os;
}

void BoardRecord::getFile() {
	std::ifstream in(gamesFileName);
	if (in.is_open()) // have savings
		in >> games;
	in.close();
	in.open(settingsFileName);
	if (in.is_open()) { // have savings
		in >> settings;
		return;
	}
	// else
	if(!importInFileSettings(&settings))
		throw logic_error("can't parse in-file settings");
}

void BoardRecord::writeGames() {
	std::ofstream outFile(gamesFileName);
	if (!outFile.is_open()) {
		cout << "Failed to open file \"" << gamesFileName << "\" to write\n";
		cout << "write action aborted\n ";
		throw runtime_error("writeGames: failed to open file to write");
	}
	outFile << games;
}

void BoardRecord::writeSettings() {
	if (settingsUnChanged)
		return;
	std::ofstream outFile(settingsFileName);
	if (!outFile.is_open()) {
		cout << "Failed to open file \"" << settingsFileName << "\" to write\n";
		cout << "write action aborted\n ";
		throw runtime_error("writeSettings: can't open file to write settings");
	}
	outFile << settings;
}

bool BoardRecord::importInFileSettings(Json::Value* dest) {
	Json::String            errors;
	Json::CharReaderBuilder charRB;
	Json::CharReader*       charReader(charRB.newCharReader());
	if (!charReader->parse(
			inFileSettings.c_str(),
			inFileSettings.c_str() + inFileSettings.size(), dest, &errors))
		return false;
	return true;
}

void BoardRecord::saveGame(BoardState& state) {
	string gameName = "no one";
	if (getDefaultSettings("whenSaveGame", "askGiveName")) {
		bool defaultGiveName =
			getDefaultSettings("whenSaveGame", "defaultGiveName");
#ifndef STARS_LANG_CHINESE
		if (defaultGiveName)
			printf(
				"Care to give the board a name? (yes as default) (no/Yes)> ");
		else
			printf("Care to give the board a name? (no as default) (No/yes)> ");
#else
		if (defaultGiveName)
			printf("给存档命名? (默认 yes) (no/Yes)> ");
		else
			printf("给存档命名? (默认 no) (No/yes)> ");
#endif // STARS_LANG_CHINESE

		char name[256];
		cin.getline(name, 256);
		if ((defaultGiveName && !strlen(name)) || !strcmp(name, "yes") ||
			!strcmp(name, "y") || !strcmp(name, "Y")) {
			cin.getline(name, 256);
			if (strlen(name))
				gameName = name;
		}
	}
	saveGame(gameName, state);
#ifndef STARS_LANG_CHINESE
	cout << "game \"" << gameName << "\" is saved.\n";
#else
	cout << "游戏 \"" << gameName << "\" 已存档\n";
#endif // STARS_LANG_CHINESE
}

void BoardRecord::saveGame(const string& gameName, BoardState& state) {
	Json::Value oneGame;
	oneGame["name"]  = gameName;
	time_t now       = time(NULL);
	char*  date      = ctime(&now);
	oneGame["date"]  = date;
	oneGame["state"] = state;
	for (oneMove move : historyMove)
		oneGame["historyMove"].append(move);
	games.append(oneGame);
	writeGames();
}

bool BoardRecord::getDefaultSettings(const string& situ, const string& item) {
	int          i               = 0;
	Json::Value& defaultSettings = settings["defaultSettings"];
	while (i < 3) {
		if (defaultSettings.isMember(situ)) {
			if (defaultSettings[situ].isMember(item))
				return defaultSettings[situ][item].asBool();
			else if (i < 2) {
				std::ofstream outSet(settingsFileName);
				outSet << inFileSettings;
				outSet.close();
				std::ifstream in(settingsFileName);
				in >> settings;
				++i;
				continue;
			}
			else {
				cout << "situ = " << situ << " item = " << item << endl;
				throw runtime_error("no such item in Stars_settings.json");
			}
		}
		else if (i < 2) {
			std::ofstream outSet(settingsFileName);
			outSet << inFileSettings;
			outSet.close();
			std::ifstream in(settingsFileName);
			in >> settings;
			defaultSettings = settings["defaultSettings"];
			++i;
			continue;
		}
		else {
			cout << "situ = " << situ << endl;
			throw runtime_error("no such situation in Stars_settings.json");
		}
	}
#ifdef STARS_DEBUG_INFO
	throw logic_error(
		"control flow to the end of BoardRecord::getDefaultSettings");
#endif
	return false;
}

Json::Value& BoardRecord::getOtherSettings(const string& name) {
	bool         ret           = true;
	Json::Value& otherSettings = settings["otherSettings"];
	while (true) {
		if (otherSettings.isMember(name))
			return otherSettings[name];
		// if not, try writting first
		if (ret) {
			std::ofstream outSet(settingsFileName);
			outSet << inFileSettings;
			outSet.close();
			std::ifstream in(settingsFileName);
			in >> settings;
			otherSettings = settings["otherSettings"];
			ret           = false;
			continue;
		}
		// if tried and failed
		throw runtime_error("no such other settings");
	}
}

void BoardRecord::showSettingsWithTags() {
	Json::Value& defaultSettings = settings["defaultSettings"];
	members      member          = defaultSettings.getMemberNames();
#ifndef STARS_LANG_CHINESE
	cout << "situation\t"
		 << "item\t\t\t"
		 << "true/false\t"
		 << "tagNumber\n";
	cout << "---------\t"
		 << "----\t\t\t"
		 << "----------\t"
		 << "---------\n";
	char x = 'a';
	for (members::iterator i = member.begin(); i != member.end(); ++i) {
		char    y     = 'a';
		members inset = defaultSettings[*i].getMemberNames();
		printf("\n");
		for (members::iterator j = inset.begin(); j != inset.end(); ++j) {
			if ((*j).size() > 15)
				cout << *i << "\t" << *j << "\t" << defaultSettings[*i][*j]
					 << "\t\t" << x << y++ << endl;
			else if ((*j).size() < 8)
				cout << *i << "\t" << *j << "\t\t\t" << defaultSettings[*i][*j]
					 << "\t\t" << x << y++ << endl;
			else
				cout << *i << "\t" << *j << "\t\t" << defaultSettings[*i][*j]
					 << "\t\t" << x << y++ << endl;
		}
		++x;
	}
#else
	cout << "情况 -- "
		 << "项目 -- "
		 << "真假 -- "
		 << "标签\n";
	cout << "---------"
		 << "--------"
		 << "--------"
		 << "----\n";
	char x = 'a';
	for (members::iterator i = member.begin(); i != member.end(); ++i) {
		char y = 'a';
		members inset = defaultSettings[*i].getMemberNames();
		printf("\n");
		for (members::iterator j = inset.begin(); j != inset.end(); ++j) {
			cout << toChinese(*i) << " -- " << toChinese(*j) << " -- "
				 << toChinese(defaultSettings[*i][*j].asBool()) << " -- " << x
				 << y++ << endl;
		}
		++x;
	}
#endif // STARS_LANG_CHINESE
}

bool BoardRecord::changeSettingsUsingTags(int tag1, int tag2) {
	int          x               = 0;
	Json::Value& defaultSettings = settings["defaultSettings"];
	members      member          = defaultSettings.getMemberNames();
	for (members::iterator i = member.begin(); i != member.end(); ++i) {
		int     y     = 0;
		members inset = defaultSettings[*i].getMemberNames();
		for (members::iterator j = inset.begin(); j != inset.end(); ++j) {
			if (x == tag1 && y == tag2) {
				defaultSettings[*i][*j] = !defaultSettings[*i][*j].asBool();
#ifndef STARS_LANG_CHINESE
				cout << *i << ": " << *j << " is changed from "
					 << std::boolalpha << !defaultSettings[*i][*j].asBool()
					 << " to " << defaultSettings[*i][*j] << endl;
#else
				cout << toChinese(*i) << ": " << toChinese(*j) << " 已从 "
					 << std::boolalpha
					 << toChinese(!defaultSettings[*i][*j].asBool())
					 << " 更改到 "
					 << toChinese(defaultSettings[*i][*j].asBool()) << endl;
#endif // STARS_LANG_CHINESE
				settingsUnChanged = false;
				return true;
			}
			++y;
		}
		++x;
	}
	return false;
}

void BoardRecord::changeOtherSettings(const string& name, const int time) {
	if (settings["otherSettings"][name].asInt() != time) {
		settings["otherSettings"][name] = time;
		settingsUnChanged               = false;
	}
}

string BoardRecord::showSavedGames(Json::Value& ret) {
	unsigned int i = 0;
	while (i < games.size()) {
#ifndef STARS_LANG_CHINESE
		printf(
			"\ndate: %sname: %s\nboard:\n", games[i]["date"].asCString(),
			games[i]["name"].asCString());
		showSavedBoard(games[i]["state"]);
		printf("index number: %d/%d\n> ", i + 1, games.size());
#else
		printf(
			"\n日期: %s名: %s\n棋盘:\n", games[i]["date"].asCString(),
			games[i]["name"].asCString());
		showSavedBoard(games[i]["state"]);
		printf("索引号: %d/%d\n> ", i + 1, games.size());
#endif // STARS_LANG_CHINESE

		while (true) {
			char input[8];
			cin.getline(input, 8);
			if (input[0] == '\0')
				break;
			if (!strcmp(input, "e") || !strcmp(input, "exit"))
				return "exit";
			if (!strcmp(input, "q") || !strcmp(input, "quit"))
				return "quit";
			if (!strcmp(input, "c") || !strcmp(input, "current")) {
				ret = games[i];
				return "yes";
			}
			else if (
				!strcmp(input, "d") || !strcmp(input, "rm") ||
				!strcmp(input, "delete") || !strcmp(input, "rm")) {
				Json::Value removed;
				games.removeIndex(i--, &removed);
				writeGames();
				break;
			}
			int number;
			try {
				number = ToInt::myStoi(input);
			}
			catch (const std::invalid_argument&) {
				number = 0;
			}
			if (number && number <= getNumberOfSavedBoard()) {
				ret = games[number - 1];
				return "yes";
			}
#ifndef STARS_LANG_CHINESE
			cout << "Pardon?\n> ";
#else
			cout << "啥？\n> ";
#endif // STARS_LANG_CHINESE
		}
		++i;
	}
	return "exit";
}

void BoardRecord::showSavedBoard(const Json::Value& state) {
	BoardState bstate(state);
	bstate.show();
}

void BoardRecord::refreshHistoryMove(const Json::Value& hm) {
	for (oneMove om : hm) {
		historyMove.push_back(om);
	}
}

BoardRecord& BoardRecord::operator=(const BoardRecord& br) {
	if (this == &br)
		return *this;
	settings         = br.settings;
	gamesFileName    = br.gamesFileName;
	settingsFileName = br.settingsFileName;
	historyMove      = br.historyMove;
	games            = br.games;
	return *this;
}

// check if settings match
bool BoardRecord::match() {
	if (settingsUnChanged)
		return true;
	string dsString = "defaultSettings", osString = "otherSettings",
		   mctString = "maxcaltime";
#ifndef STARS_DEBUG_INFO
	if (!settings.isMember(dsString) || !settings.isMember(osString))
		return false;
	Json::Value &ds = settings["defaultSettings"],
				os  = settings["otherSettings"];
	if (!os.isMember(mctString) || os[mctString].asInt() < 10)
		return false;
#endif // !STARS_DEBUG_INFO
#ifdef STARS_DEBUG_INFO
	if (!settings.isMember(dsString)) {
		cout << "no such member as " << dsString << endl;
		return false;
	}
	if (!settings.isMember(osString)) {
		cout << "no such member as " << osString << endl;
		return false;
	}
	Json::Value &ds = settings["defaultSettings"],
				os  = settings["otherSettings"];
	if (!os.isMember(mctString)) {
		cout << "no such member as " << mctString << endl;
		return false;
	}
	if (os[mctString].asInt() < 10) {
		cout << mctString << " went nuts" << endl;
		return false;
	}
#endif // STARS_DEBUG_INFO

	// get the "right default settings"
	Json::Value root, rightData;
	if(!importInFileSettings(&root))
		throw logic_error(
			"can't parse in file settings file, match check aborted");
	rightData = root["defaultSettings"];

	// check if default settings match
	members member = ds.getMemberNames();
	for (members::iterator i = member.begin(); i != member.end(); ++i) {
		if (!rightData.isMember(*i))
			return false;
		members inset = ds[*i].getMemberNames();
		for (members::iterator j = inset.begin(); j != inset.end(); ++j) {
			if (!rightData[*i].isMember(*j)) {
#ifdef STARS_DEBUG_INFO
				cout << *i << " has no member " << *j << endl;
#endif // STARS_DEBUG_INFO
				return false;
			}
		}
	}
	return true;
}
