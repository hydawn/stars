#include "boardRecord.h"

std::ostream &operator<<(std::ostream &os, oneMove &move) {
	os << "mode = " << move.mode << "\n";
	if (move.mode=="normal" || move.mode=="debug") {
		os  << "time used: " << move.time << "ms, hint on: " << std::boolalpha
			<< move.hintOn << ", suggestion = " << move.suggestion
			<< "\nword = " << move.word << ", list = [ ";
		for (short i : move.list)
			os << i << " ";
		os << "]\nmove = " << move.move;
		if (move.byComputer)
			os << " by computer '" << move.player << "':\n";
		else
			os << " by player '" << move.player << "':\n";
	}
	else if (move.mode == "add")
		os << "add '" << move.player << "' in column " << move.move << endl;
	else if (move.mode == "reverse")
		os << "remove column " << move.move << endl;
	return os;
}

void BoardRecord::getFile() {
	std::ifstream inGames(gamesFileName);
	if (!inGames.is_open()) {
		cout << "failed to open file \"" << gamesFileName << "\" to read\n";
		cout << "will create one when necessary.\n";
	}
	else
		inGames >> games;
	std::ifstream inSet(settingsFileName);
	if (!inSet.is_open()) {
		cout << "failed to open file \"" << settingsFileName << "\" to read\n";
		cout<< "creating a new file\n";
		std::ofstream outSet(settingsFileName);
		// hard core show
		string addon = "";
		string defaultSettings = addon +
			"{\n" +
			"	\"changeBoard\" : \n" +
			"	{\n" +
			"		\"askToSaveBoard\" : false,\n" +
			"		\"defaultSaveBoard\" : false\n" +
			"	},\n" +
			"	\"exitNormal\" : \n" +
			"	{\n" +
			"		\"askToDebug\" : false,\n" +
			"		\"askToSaveBoard\" : true,\n" +
			"		\"defaultDebug\" : false,\n" +
			"		\"defaultSaveBoard\" : false\n" +
			"	},\n" +
			"	\"gameIsOver\" : \n" +
			"	{\n" +
			"		\"askToDebug\" : true,\n" +
			"		\"askToSaveBoard\" : true,\n" +
			"		\"defaultDebug\" : false,\n" +
			"		\"defaultSaveBoard\" : false\n" +
			"	},\n" +
			"	\"inDebugMode\" : \n" +
			"	{\n" +
			"		\"hintOn\" : false,\n" +
			"		\"showCalculate\" : false,\n" +
			"		\"showTime\" : false\n" +
			"	},\n" +
			"	\"inNormalMode\" : \n" +
			"	{\n" +
			"		\"showCalculate\" : false,\n" +
			"		\"showTime\" : false\n" +
			"	},\n" +
			"	\"whenSaveGame\" : \n" +
			"	{\n" +
			"		\"askGiveName\" : true,\n" +
			"		\"defaultGiveName\" : false\n" +
			"	},\n" +
			"	\"inToDebugMode\" : \n" +
			"	{\n" +
			"		\"askToImport\" : false,\n" +
			"		\"askToSaveBoard\" : false,\n" +
			"		\"defaultImport\" : false,\n" +
			"		\"defaultSaveBoard\" : false\n" +
			"	}\n" +
			"}\n";
		outSet << defaultSettings;
		if(!outSet.is_open()) {
			throw runtime_error("failed to create file, mission aborted\n ");
		}
		outSet.close();
		std::ifstream in(settingsFileName);
		in >> settings;
	}
	else
		inSet >> settings;
}

void BoardRecord::writeGames() {
	std::ofstream outFile(gamesFileName);
	if (!outFile.is_open()) {
		cout << "failed to open file \"" << gamesFileName << "\" to write\n";
		cout<< "write action aborted\n ";
		return;
	}
	outFile << games;
}

void BoardRecord::writeSettings() {
	std::ofstream outFile(settingsFileName);
	if (!outFile.is_open()) {
		cout << "failed to open file \"" << settingsFileName << "\" to write\n";
		cout<< "write action aborted\n ";
		return;
	}
	outFile << settings;
}

void BoardRecord::saveGame(BoardHandle& state) {
	string gameName = "no one";
	if (getSettings("whenSaveGame", "askGiveName")) {
		bool defaultGiveName = getSettings("whenSaveGame", "defaultGiveName");
		if (defaultGiveName)
			printf("Care to give the board a name? (yes as default) (no/Yes)> ");
		else
			printf("Care to give the board a name? (none as default) (No/yes)> ");
		char name[256];
		cin.getline(name, 256);
		if ((defaultGiveName && !strlen(name)) || !strcmp(name, "yes")) {
			cin.getline(name, 256);
			if (strlen(name))
				gameName = name;
		}
	}
	saveGame(gameName, state);
}

void BoardRecord::saveGame(const string& gameName, BoardHandle& state) {
	Json::Value oneGame;
	oneGame["name"] = gameName;
	time_t now = time(NULL);
    char *date = ctime(&now);
	oneGame["date"] = date;
	oneGame["state"] = state;
	for (oneMove move : historyMove)
		oneGame["historyMove"].append(move);
	games.append(oneGame);
	writeGames();
}

void BoardRecord::showSettingsWithTags() {
	members member = settings.getMemberNames();
	cout << "situation\t" << "item\t\t\t" << "true/false\t" <<"tagNumber\n";
	int x = 0;
	for (members::iterator i = member.begin(); i != member.end();++i) {
		int y = 0;
		members inset = settings[*i].getMemberNames();
		printf("\n");
		for (members::iterator j = inset.begin(); j != inset.end();++j) {
			if ((*j).size() > 15)
				cout << *i << "\t" << *j << "\t" << settings[*i][*j] << "\t\t" << x << y++ << endl;
			else if ((*j).size()<7)
				cout << *i << "\t" << *j << "\t\t\t" << settings[*i][*j] << "\t\t" << x << y++ << endl;
			else
				cout << *i << "\t" << *j << "\t\t" << settings[*i][*j] << "\t\t" << x << y++ << endl;
		}
		++x;
	}
}

bool BoardRecord::changeSettingsUsingTags(int tag1, int tag2) {
	members member = settings.getMemberNames();
	int x = 0;
	for (members::iterator i = member.begin(); i != member.end();++i) {
		int y = 0;
		members inset = settings[*i].getMemberNames();
		for (members::iterator j = inset.begin(); j != inset.end();++j) {
			if (x==tag1&&y==tag2) {
				settings[*i][*j] = !settings[*i][*j].asBool();
				saveSettings();
				cout << "debuginfo: " << *i << ": " << *j << " is changed from "
					<< std::boolalpha << !settings[*i][*j].asBool()
					<< " to " << settings[*i][*j] << endl;
				return true;
			}
			++y;
		}
		++x;
	}
	return false;
}

Json::Value* BoardRecord::showSavedGames() {
	int i = 1;
	for (Json::Value::iterator iter = games.begin(); iter != games.end();++iter) {
		// date, name, board, (index)
		printf("\ndate: %s\nname: %s\nboard:\n", (*iter)["date"].asCString(), (*iter)["name"].asCString());
		showSavedBoard((*iter)["state"]);
		printf("index number: %d\n> ", i++);

		while(true) {
			char input[8];
			cin.getline(input, 8);
			if (input[0]=='\0')
				break;
			if (!strcmp(input, "0")||!strcmp(input, "q")||!strcmp(input, "exit")||!strcmp(input, "quit"))
				return nullptr;
			if (!strcmp(input, "c")||!strcmp(input, "current"))
				return &(*iter);
			try {
				int number = atoi(input);
				if (number > 0 && number <= getNumberOfSavedBoard())
					return &games[number-1];
			} catch(const std::exception& e) {
				std::cerr << e.what() << endl;
			}
			cout << "Pardon?\n";
		}
	}
	return nullptr;
}

void BoardRecord::showSavedBoard(const Json::Value& state) {
	BoardHandle bstate(state);
	bstate.show();
}

void BoardRecord::refreshHistoryMove(const Json::Value& hm) {
	for (oneMove om : hm) {
		historyMove.push_back(om);
	}
}
