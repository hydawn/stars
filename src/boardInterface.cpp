#include "BoardInterface.h"

BoardInterface::BoardInterface() {
	generate(8, 8, 4);
	analyse->maxcaltime = record.getOtherSettings("maxcaltime").asInt();
}

BoardInterface::BoardInterface(BoardAnalyse& hb) {
	analyse				= new BoardAnalyse(hb);
	analyse->maxcaltime = record.getOtherSettings("maxcaltime").asInt();
}

BoardInterface::~BoardInterface() {
	record.getOtherSettings("maxcaltime") = analyse->maxcaltime;
	delete analyse;
}

bool BoardInterface::getStateFromInput() {
	short  rows = analyse->getRows(), cols = analyse->getCols();
	vector<string> input;
	string inputEater;
	analyse->state.printHead();

	for (int i = 0; i < rows; ++i) {
		getline(cin, inputEater);
		if (inputEater == "0" || inputEater == "exit")
			return false;
		if (inputEater.size() != cols * 2 + 1) {
			cout << "Something wrong going on with the input board.\n";
			return false;
		}
		input.push_back(inputEater);
	}

	// transform
	char** temp = new char*[cols];
	if (transformInput(temp, input, cols, rows)) {
		analyse->state.refreshBoard(temp);
		analyse->state.refreshTop();
		analyse->state.freeBoard(temp, rows);
		return true;
	}
	analyse->state.freeBoard(temp, rows);
	return false;
}

bool BoardInterface::transformInput(char** dest, vector<string>& src,
	const int cols, const int rows) {
	bool rax = true;
	for (short i = 0; i < cols; ++i) {
		dest[i] = new char[rows];
		for (short j = 0; j < rows && rax; ++j) {
			// check
			if (src[rows - j - 1][i * 2 + 1] == '+')
				src[rows - j - 1][i * 2 + 1] = ' ';
			if (src[rows - j - 1][i * 2 + 1] != ' ' &&
				src[rows - j - 1][i * 2 + 1] != 'X' &&
				src[rows - j - 1][i * 2 + 1] != '0') {
				cout << "Something wrong going on with the input board while "
					<< "transforming.\n";
				rax = false;
				break;
			}
			// real transform
			dest[i][j] = src[rows - j - 1][i * 2 + 1];
		}
	}
	return rax;
}

// mode = "reverse", "add"
string BoardInterface::getInput(const string mode) {
	while (true) {
		if (mode == "reverse")
			printf("\nIn reverse mode\nEnter move> ");
		else if (mode == "add")
			printf("\nIn Add mode\nEnter player(blank space)move> ");
		char input[INTER_MAX_INPUT];
		cin.getline(input, INTER_MAX_INPUT);

		if (input[0] == '\0' || !strcmp(input, "0") || !strcmp(input, "exit"))
			return "exit";
		else if (!strcmp(input, "quit") || !strcmp(input, "q"))
			return "quit";
		else if (!strcmp(input, "r") || !strcmp(input, "reverse")) {
			if (mode != "add")
				printf("We are already in reverse mode\n");
			else
				return "reverse";
		} else if (!strcmp(input, "a") || !strcmp(input, "add")) {
			if (mode != "reverse")
				printf("We are already in add mode\n");
			else
				return "add";
		} else if (!strcmp(input, "S") || !strcmp(input, "show"))
			analyse->show();
		else if (!strcmp(input, "h") || !strcmp(input, "help"))
			cout << getHelp(mode) << endl;
		else if (!strcmp(input, "i") || !strcmp(input, "info") ||
			!strcmp(input, "story") || !strcmp(input, "t") ||
			!strcmp(input, "tips"))
			cout << getInfo(input) << endl;
		else if (!strcmp(input, "m") || !strcmp(input, "move"))
			printf("Why don't we try this in debug mode to see what it does?\n");
		else if (mode != "add") {
			int num = atoi(input);
			if (num > 0 && num <= analyse->getCols() && !analyse->colIsEmpty(num))
				return input;
			cout << "Invalid " << mode << " mode number input, let\'s try again\n";
		} else if (mode != "reverse") {
			char temp[INTER_MAX_INPUT];
			strcpy(temp, input + 2);
			int num = atoi(temp);
			if ((input[0] == 'X' || input[0] == 'x' || input[0] == '0') && num > 0 &&
				num <= analyse->getCols() && !analyse->colIsFull(num))
				return input;
		}
	}
}

// mode = "debug"
string BoardInterface::getInput(char plr, double& inputTime) {
	int						 counter = 0;
	system_clock::time_point start;
	system_clock::time_point end;
	string					 getter;
	while (true) {
		if (counter > 32)
			cin.clear();
		cout << "\nIn debug mode\nPlayer '" << plr << "' move> ";
		char input[INTER_MAX_INPUT];
		start = system_clock::now();
		cin.getline(input, INTER_MAX_INPUT);
		end			 = system_clock::now();
		auto elapsed = duration_cast<milliseconds>(end - start);
		inputTime	 = elapsed.count();

		int num = atoi(input);
		if (num > 0 && num <= analyse->getCols() && !analyse->colIsFull(num))
			return input;
		// excute
		if (!strcmp(input, "0") || !strcmp(input, "exit"))
			return "exit";
		else if (input[0] == '\0') {
			++counter;
			cout << "Invalid empty input, let\'s try again\n";
		}
		else if (!strcmp(input, "a") || !strcmp(input, "add"))
			return "add";
		else if (!strcmp(input, "C") || !strcmp(input, "custom"))
			return "custom";
		else if (!strcmp(input, "H") || !strcmp(input, "hint"))
			return input;
		else if (!strcmp(input, "h") || !strcmp(input, "help")) {
			getter = getHelp("debug");
			if (getter != "quit")
				cout << getter << endl;
			else
				return getter;
		}
		else if (!strcmp(input, "i") || !strcmp(input, "info") ||
			!strcmp(input, "story") || !strcmp(input, "t") ||
			!strcmp(input, "tips") || !strcmp(input, "a song, please")) {
			getter = getInfo(input);
			if (getter != "quit")
				cout << getter << endl;
			else
				return getter;
		}
		else if (!strcmp(input, "m") || !strcmp(input, "move") ||
			!strcmp(input, "I") || !strcmp(input, "import") ||
			!strcmp(input, "c") || !strcmp(input, "change"))
			return input;
		else if (!strcmp(input, "P") || !strcmp(input, "play back") ||
			!strcmp(input, "playback"))
			return "playBack";
		else if (!strcmp(input, "p") || !strcmp(input, "play") ||
			!strcmp(input, "play"))
			return "play";
		else if (!strcmp(input, "q") || !strcmp(input, "quit"))
			return "quit";
		else if (!strcmp(input, "r") || !strcmp(input, "reverse"))
			return "reverse";
		else if (!strcmp(input, "S") || !strcmp(input, "show")) {
			if (record.getDefaultSettings("inDebugMode", "starrySky"))
				analyse->starShow();
			else
				analyse->show();
		}
		else if (!strcmp(input, "s") || !strcmp(input, "setting") ||
			!strcmp(input, "settings"))
			return "settings";
		else if (!strcmp(input, "sa") || !strcmp(input, "st") ||
			!strcmp(input, "show stars"))
			analyse->starShow();
		else if (!strcmp(input, "sp") || !strcmp(input, "self play")) {
			if(selfPlayMode() == "quit")
				return "quit";
			continue;
		}
		else if (!strcmp(input, "sv") || !strcmp(input, "save"))
			record.saveGame(analyse->state);
		else if (!strcmp(input, "sr") || !strcmp(input, "show routes")) {
			analyse->routes.crnt = analyse->routes.head;
			if (showRoutesMode() == "quit")
				return "quit";
		}
		else if (!strcmp(input, "w") || !strcmp(input, "winn"))
			cout << "winn = " << analyse->state.winn << endl;
		else
			cout << "No worries, Let\'s try again?\n";
	}
}

short BoardInterface::getCustomInput(const string item) {
	char  input[16];
	short customNumber;
	while (true) {
		cout << item << " (2~" << SHORTV_LENGTH - 1 << ") = ";
		cin.getline(input, 16);
		if (!strcmp(input, "0") || !strcmp(input, "exit"))
			return 0;
		customNumber = atoi(input);
		if (customNumber > 1 && customNumber < SHORTV_LENGTH - 1)
			return customNumber;
		cout << "Let's try again\n";
	}
}

string BoardInterface::addMode() {
	string	input;
	oneMove move;
	move.mode = "add";
	while (true) {
		input = getInput("add");
		if (input == "exit")
			return "debug";
		if (input == "quit" | input == "reverse")
			return input;
		if (input[0] == '0')
			move.player = input[0];
		else
			move.player = 'X';
		move.move = atoi(input.c_str() + 2);
		printf("Player '%c' is added to '%d' as you like it:\n", move.player, move.move);
		analyse->go(move.player, move.move);
		analyse->show();
		record.push_back(move);
	}
}

string BoardInterface::reverseMode() {
	string	input;
	oneMove move;
	move.mode = "reverse";
	while (true) {
		input = getInput("reverse");
		if (input == "exit")
			return "debug";
		if (input == "quit" || input == "add")
			return input;
		move.move = atoi(input.c_str());
		// record which move is reversed
		move.player = analyse->state.board[move.move - 1][analyse->state.top[move.move - 1] - 1];
		analyse->reverse(move.move);
		printf("Remove %d as you like it:\n", move.move);
		record.push_back(move);
		analyse->show();
	}
}

string BoardInterface::debugMode(oneMove& byPlayer) {
	// init
	oneMove byOpponent;
	string	input;
	byPlayer.mode		  = "debug";
	byPlayer.byComputer	  = false;
	byOpponent.mode		  = "debug";
	byOpponent.byComputer = true;
	byOpponent.player	  = analyse->rPlayer(byPlayer.player);
	printf("We are in debug mode now\n");
	analyse->show();

	// main loop
	while (true) {
		input = getInput(byPlayer.player, byPlayer.time);
		if (input == "exit" || input == "quit") {
			printf("Exit from debug mode ...\n");
			return input;
		}
		if (input == "add" || input == "reverse" || input == "settings" ||
			input == "custom" || input == "play" || input == "playBack")
			return input;
		if (input == "c" || input == "change") {
			// change player
			std::swap(byPlayer.player, byOpponent.player);
			std::swap(byPlayer.word, byOpponent.word);
			std::swap(byPlayer.list, byOpponent.list);
			std::swap(byPlayer.suggestion, byOpponent.suggestion);

			// print hint
			if (byPlayer.mode == "debug" && record.getDefaultSettings("inDebugMode", "hintOn")) {
				printf("Here was %c's word, list, and suggestion:\n", byPlayer.player);
				cout << "word = " << byPlayer.word << "\nlist = [ ";
				for (short move : byPlayer.list)
					printf("%d ", move);
				printf("]\n");
			}
			continue;
		}
		else if (input == "I" || input == "import") {
			if (record.getDefaultSettings("changeBoard", "askToSaveBoard"))
				askToSaveBoard(record.getDefaultSettings("changeBoard", "defaultSaveBoard"));
			else if (record.getDefaultSettings("changeBoard", "defaultSaveBoard"))
				record.saveGame(analyse->state);
			// new board
			importNewBoard();
			continue;
		}
		else if (input == "H" || input == "hint") {
			if (byPlayer.mode != "debug") {
				printf("The last move is not in debug move, hint was lost.\n");
				continue;
			}
			printf("this is the hint for the previous move:\n");
			cout << "word = " << byPlayer.word << "\nlist = [ ";
			for (short i : byPlayer.list)
				printf("%d ", i);
			printf("]\n");
			continue;
		}
		else if (input != "m" && input != "move") {
			// player goes
			byPlayer.move = atoi(input.c_str());
			analyse->go(byPlayer.player, byPlayer.move);
			record.push_back(byPlayer);	 // byPlayer end here
			if (isOver(byPlayer)) {
				printf("Exit from debug mode ...\n");
				return "over";
			}
		}
		else if (isOver(byPlayer)) {
			printf("Exit from debug mode ...\n");
			return "over";
		}

		// opp respond
		if (record.getDefaultSettings("inDebugMode", "showCalculate") &&
			record.getDefaultSettings("inDebugMode", "showTime"))
			printf("Info for the computer:\n");
		byOpponent.move = analyse->respond(byOpponent.player, byOpponent,
			record.getDefaultSettings("inDebugMode", "showCalculate"),
			record.getDefaultSettings("inDebugMode", "showTime"),
			record.getDefaultSettings("inDebugMode", "starsOn"),
			record.getDefaultSettings("inDebugMode", "trackRoutes"));
#ifdef STARS_DEBUG_INFO
		if (!byOpponent.list.empty() &&
			!MyShortList::inList(byOpponent.list, byOpponent.move))
			throw logic_error("suggestion not in safe list\n");
#endif
		analyse->go(byOpponent.player, byOpponent.move);
		byOpponent.suggestion = byOpponent.move;
		record.push_back(byOpponent);
		if (record.getDefaultSettings("inDebugMode", "showTime"))
			cout << "    input time used: " << byPlayer.time << "ms\n";
		printf("    %c goes here %d\n", byOpponent.player, byOpponent.move);
		if (isOver(byOpponent)) {
			printf("Exit from debug mode ...\n");
			return "over";
		}

		// recommend
		byPlayer.hintOn	= record.getDefaultSettings("inDebugMode", "hintOn");
		if (byPlayer.hintOn)
			cout << "\nHere is hint provided for you\n";
		byPlayer.suggestion = analyse->respond(byPlayer.player, byPlayer,
			byPlayer.hintOn,
			record.getDefaultSettings("inDebugMode", "showTime"),
			record.getDefaultSettings("inDebugMode", "starsOn"),
			false);
		showComment(byPlayer);
		if (byPlayer.hintOn && byPlayer.word != "bad")
			printf("    %d is recommended\n", byPlayer.suggestion);

		// show
		printf("\n");
		if (record.getDefaultSettings("inDebugMode", "starrySky"))
			analyse->starShow();
		else
			analyse->show();
		printf("\n");
	}
#ifdef STARS_DEBUG_INFO
	throw logic_error("control flow into the end of debug mode\n");
#endif
}

string BoardInterface::defaultSettings() {
	cout << "We have " << record.getDefaultSettingsItemNum() << " situations and "
		 << "within them we have multiple items each marked with tags\n"
		 << "below, type in tags to change these settings, 0 to exit, S/show "
		 << "to show current\nsettings, h/help for help\n";
	// show
	record.showSettingsWithTags();
	cout << "\nCare to change one of them?\n";
	while (true) {
		printf("> ");
		char input[8];
		cin.getline(input, 8);
		if (input[0] == '\0' || !strcmp(input, "0") || !strcmp(input, "exit") ||
			!strcmp(input, "no")) {
			record.saveSettings();
			cout << "Exit from settings mode\n";
			return "debug";
		}
		if (!strcmp(input, "q") || !strcmp(input, "quit")) {
			cout << "Exit from settings mode\n";
			record.saveSettings();
			return "quit";
		}
		else if (!strcmp(input, "h") || !strcmp(input, "help"))
			cout << getHelp("settings");
		else if (!strcmp(input, "S") || !strcmp(input, "show"))
			record.showSettingsWithTags();
		else if (strlen(input) == 2) {
			int tag1 = input[0] - 'a', tag2 = input[1] - 'a';
			if (!(tag1 < 26 && tag1 > -1 && tag2 < 26 && tag2 > -1 &&
				record.changeSettingsUsingTags(tag1, tag2))) {
				cout << "Invalid input, let's try again.\n";
				cout << "And if you don't know what's going on, you can always"
						" type help, happy to help as always.\n";
			}
		} else {
			cout << "Invalid input, let's try again.\n";
			cout << "And if you don't know what's going on, you can always "
					"type help, happy to help as always.\n";
		}
	}
#ifdef STARS_DEBUG_INFO
	throw logic_error("control flow into the end of settings mode\n");
	return "quit";
#endif
}

string BoardInterface::otherSettings() {
	if (analyse->maxcaltime < 0)
		printf("analyse->maxcaltime went nuts, better fix it\n");
	cout << "Here you can change computer's max calculation time settings, the\n"
		<< "current cal time is "
		<< analyse->maxcaltime
		<< "ms. You can reset it so the calculation time might be\ndifferent. "
		<< "nothing or 0 to exit, q to quit\n";
	char in[16];
	int	 trans = 0;
	do {
		printf("(10ms ~ 9999999ms)> ");
		cin.getline(in, 16);
		if (in[0] == '\0' || !strcmp(in, "0") || !strcmp(in, "exit"))
			return "debug";
		if (!strcmp(in, "q") || !strcmp(in, "quit"))
			return "quit";
		trans = atoi(in);
		if (trans > 9 && trans < 9999999) {
			analyse->maxcaltime = trans;
			return "debug";
		}
		printf("let's try again\n");
	} while (true);
}

string BoardInterface::settingsMode() {
	printf("We are in settings mode now:\n");
	// default settings, other settings
	char input[8];
	printf("1. default settings\n2. other settings\n");
	do {
		printf("(enter number)> ");
		cin.getline(input, 8);
		if (input[0] == '\0' || !strcmp(input, "0") || !strcmp(input, "exit"))
			return "debug";
		if (!strcmp(input, "q") || !strcmp(input, "quit"))
			return "quit";
		if (!strcmp(input, "1"))
			return defaultSettings();
		if (!strcmp(input, "2"))
			return otherSettings();
		else
			printf("let's try again\n");
	} while (true);
}

string BoardInterface::playMode() {
	if (!record.getNumberOfSavedBoard()) {
		cout << "No games has been recorded yet. If you want to try this mode, "
				"save a game first\nTill next time.\nExit from play mode...\n";
		return "debug";
	}
	printf("We are here in play mode.\n");
	printf("You can choose from %d saved games.\n", record.getNumberOfSavedBoard());
	printf("Hit 'Enter' to see more, type in index number to went into one of those games.\n");
	printf("Or type c to continue to play the board that is currently displayed. ");
	printf("Use d/rm/delete/remove to delete the current board, 0 to exit.\n");
	printf("Explore!\n");
	Json::Value game;
	string result = record.showSavedGames(game);

	if (result == "exit") {
		printf("Exit from play mode...\n");
		return "debug";
	}
	if (result == "quit") {
		printf("Exit from play mode...\n");
		return "quit";
	}

	// set up a new game
	BoardAnalyse   analyser(game["state"]);
	BoardInterface interface(analyser);
	BoardRecord	   newRecord;
	newRecord.refreshHistoryMove(game["historyMove"]);
	interface.refreshRecord(newRecord);
	printf("Into a new control mode:\n");
	printf("Use 1~8 to place a piece, h for help, 0 to exit.\n");
	printf("Game is on->\n");

	if(!interface.controlMode())
		return "quit";
	printf("Exit from play mode...\n");
	return "debug";
}

string BoardInterface::playBackMode() {
	if (!record.getNumberOfSavedBoard()) {
		printf(
			"No games has been recorded yet. If you want to try this mode, save a game first\nTill next "
			"time.\n");
		printf("Exit from play back mode...\n");
		return "debug";
	}
	printf("We are here in play back mode.\n");
	printf("You can choose from %d saved games.\n", record.getNumberOfSavedBoard());
	printf("Hit 'Enter' to see more, type in index number to went into one of those games.\n");
	printf("Or type c to continue to play the board that is currently displayed. ");
	printf("Use d/rm/delete/remove to delete the current board, 0 to exit.\n");
	printf("Explore!\n");
	Json::Value game;
	string result = record.showSavedGames(game);
	if (result == "exit") {
		printf("Exit from play back mode...\n");
		return "debug";
	}
	if (result == "quit") {
		printf("Exit from play back mode...\n");
		return "quit";
	}

	// set up a new game and play back the moves
	BoardState	 state = game["state"];
	BoardRecord	 newRecord, tempRecord;
	newRecord.refreshHistoryMove(game["historyMove"]);
	// return to the initial state
	state.retInit(newRecord.historyMove);
	BoardAnalyse analyser(state);

	printf("Here's the play back of an old game:\n");
	printf("Use c to cut in and play, h for help, b to go back to the previous step, 0\nto exit.\n");
	char					  input[16];
	bool					  wentBack		= false;
	char					  reversePlayer = ' ';
	vector<oneMove>::iterator iter			= newRecord.historyMove.begin();
	while (iter != newRecord.historyMove.end()) {
		// show
		printf("This is step %zu/%zu\n", tempRecord.historyMove.size() + 1, newRecord.historyMove.size());
		if (wentBack)
			wentBack = false;
		else {
			cout << *iter;
			tempRecord.push_back(*iter);
			if (iter->mode != "reverse")
				analyser.go(iter->player, iter->move);
			else {
				reversePlayer = analyser.state.board[iter->move - 1][analyser.state.top[iter->move - 1] - 1];
				analyser.reverse(iter->move);
			}
		}
		analyser.show();

		// get input
		printf("> ");
		cin.getline(input, 16);
		if (!strcmp(input, "0") || !strcmp(input, "exit"))
			break;
		if (!strcmp(input, "q") || !strcmp(input, "quit"))
			return "quit";
		else if (!strcmp(input, "c") || !strcmp(input, "cut") || !strcmp(input, "cut in")) {
			BoardAnalyse   tempAnalyser = analyser;
			BoardInterface interface(tempAnalyser);
			interface.refreshRecord(tempRecord);
			printf("Go into a new control mode:\n");
			if(!interface.controlMode())
				return "quit";
			printf("We're back in play back mode again\n");
		}
		else if (!strcmp(input, "b") || !strcmp(input, "back") || !strcmp(input, "go back")) {
			if (tempRecord.historyMove.empty())
				printf("This is the beginning, there is no going back.\n");
			else {
				tempRecord.pop_back();
				if (iter->mode != "reverse")
					analyser.reverse(iter->move);
				else
					analyser.go(reversePlayer, iter->move);
				wentBack = true;
				--iter;
				continue;
			}
		}
		else if (!strcmp(input, "h") || !strcmp(input, "help"))
			printf("b to go back to the previous move, c to cut in and play, q to quit\n");
			printf("the whole game, 0 to exit this mode\n");
		++iter;
	}
	printf("Exit from play back mode...\n");
	return "debug";
}

string BoardInterface::customMode() {
	printf("In custom mode ...\n");
	printf("Note that the more width you set, the clumsier the computer will be\n");
	printf("It is recommended to set the width lesser than 16\n");
	if (record.getDefaultSettings("inCustomMode", "askToSaveBoard"))
		askToSaveBoard(record.getDefaultSettings("inCustomMode", "defaultSaveBoard"));
	else if (record.getDefaultSettings("inCustomMode", "defaultSaveBoard"))
		record.saveGame(analyse->state);
	short column, row, winn;
	column = getCustomInput("board width");
	if (!column)
		return "debug";
	row = getCustomInput("board height");
	if (!row)
		return "debug";
	winn = getCustomInput("win number");
	if (!winn)
		return "debug";
	analyse->customBoard(column, row, winn);
	record.clearHistoryMove();
	printf("Here's the board we're gonna use:\n");
	analyse->show();
	printf("Exit from custom mode ...\n");
	return "debug";
}

bool BoardInterface::controlMode() {
	oneMove byPlayer;
	byPlayer.player = 'X';
	string	advice	= debugMode(byPlayer);
	int		i		= 0;
	while (i < 100) {
		if (advice == "quit" || advice == "exit")
			break;
		else if (advice == "add")
			advice = addMode();
		else if (advice == "reverse")
			advice = reverseMode();
		else if (advice == "debug")
			advice = debugMode(byPlayer);
		else if (advice == "settings")
			advice = settingsMode();
		else if (advice == "play")
			advice = playMode();
		else if (advice == "playBack")
			advice = playBackMode();
		else if (advice == "custom")
			advice = customMode();
		else if (advice == "over") {
			if (record.getDefaultSettings("gameIsOver", "askToReverse") &&
				askToReverse(record.getDefaultSettings("gameIsOver", "defaultReverse"))) {
				advice = reverseMode();
				continue;
			}
			else if (record.getDefaultSettings("gameIsOver", "defaultReverse")) {
				advice = reverseMode();
				continue;
			}
			if (record.getDefaultSettings("gameIsOver", "askToSaveBoard"))
				askToSaveBoard(record.getDefaultSettings("gameIsOver", "defaultSaveBoard"));
			else if (record.getDefaultSettings("gameIsOver", "defaultSaveBoard"))
				record.saveGame(analyse->state);
			break;
		}
		++i;
	}
	if (!record.match())
		throw runtime_error("current settings var in BoardRecord doesn't match with the default settings");
	if (i == 100)
		throw runtime_error("too much unhandled advice\n");
	if (advice == "quit")
		return false;
	cout << "Exit from controller\n";
	return true;
}

string BoardInterface::showRoutesMode() {
	RouteTree&			routes		  = analyse->routes;
	vector<RouteNode*>& next		  = routes.crnt->next;
	bool				nearFlag	  = false;
	long long routeBranches = routes.getBranches();
	cout << "The computer examined " << routeBranches
		 << " possibilities in this node\n" << routes.getBranches(-2)
		 << " of them is free, " << routes.getBranches(-1)
		 << " of them is good and " << routes.getBranches(0)
		 << " of them is bad for the computer\n";
	cout << "We'll be showing:\na. free routes\nb. good routes\nc. bad "
		 << "routes\nd. all routes\n";
	if (next.size() == 1 && next[0]->next.empty() &&
		(next[0]->data == routes.goodNode || next[0]->data == routes.badNode ||
		next[0]->data == routes.freeNode))
		nearFlag = true; // next is a flag
	else {
		cout << "or see less in the next node choosing from:\n[ ";
		vRi iter = next.begin();
		for (; iter != next.end(); ++iter)
			cout << (*iter)->data << " ";
		cout << "]\n";
	}
	if (routes.crnt->prev)
		cout << "Enter B or back to go back to the formal node\n";
	string in;
	int num = 0;
	do {
		cout << "> ";
		getline(cin, in);
		if (in == "q" || in == "quit")
			return "quit";
		if (in == "0" || in == "exit" || in.empty())
			return "debug";
		if (in == "a") {
			num = 1;
			break;
		}
		else if (in == "b") {
			num = 2;
			break;
		}
		else if (in == "c") {
			num = 3;
			break;
		}
		else if (in == "d") {
			num = 4;
			break;
		}
		else if (in == "B" || in == "back") {
			if (routes.crnt->prev) {
				routes.backward();
				// 新时代的老递归怪
				return showRoutesMode();
			}
			cout << "There's no going back, let's try again.\n";
			continue;
		}
		else if (!nearFlag) {
			num = atoi(in.c_str());
			// find_if by me
			vRi iter = next.begin();
			for (; iter != next.end() &&
				(*iter)->data != num;++iter);
			if (iter == next.end()) {
				cout << "No such move, let's try again.\n";
				continue;
			}
			routes.forward(num);
			// 老递归怪了
			return showRoutesMode();
		}
		else
			cout << "No such option, let's try again.\n";
	} while (true);


	// print
	routeBranches = routes.getBranches(num - 3);
	if (routeBranches > 1024) {
		cout << "There are " << routeBranches
			<< " branches, sure you want to print them all?\n"
			<< "(Yes/no)> ";
		getline(cin, in);
		if (in != "no" && in != "n")
			routes.showRoute(num - 3);
	}
	else
		routes.showRoute(num - 3);
	return "debug";
}

string BoardInterface::selfPlayMode() {
	cout << "With hit or without hit (w/Wo)>";
	string input;
	bool   with = false;
	//getline(cin, input);
	if (input == "w")
		with = true;
	else if (input == "q" || input == "quit")
		return input;
	else if (input == "0" || input == "exit")
		return "debug";
	
	// show time
	oneMove byOpponent, byPlayer;
	// string	input;
	// byPlayer.mode		  = "selfPlay";
	byPlayer.player		  = 'X';
	// byPlayer.byComputer	  = true;
	byPlayer.move		  = 5;
	// byOpponent.mode		  = "selfPlay";
	// byOpponent.byComputer = true;
	byOpponent.player	  = '0';
	
	printf("We are in self play mode now\n");
	analyse->show();

	// main loop
	while (true) {
		if (with)
			break;
		// player goes
		analyse->go(byPlayer.player, byPlayer.move);
		printf("    %c goes here %d\n", byPlayer.player, byPlayer.move);
		// record.push_back(byPlayer);	 // byPlayer end here
		if (isOver(byPlayer)) {
			printf("Exit from self play mode ...\n");
			return "quit";
		}

		// opp respond
		// printf("Info for the computer:\n");
		byOpponent.move = analyse->respond(byOpponent.player, byOpponent,
			false, false,
			// record.getDefaultSettings("inDebugMode", "showCalculate"),
			// record.getDefaultSettings("inDebugMode", "showTime"),
			record.getDefaultSettings("inDebugMode", "starsOn"),
			false
			// record.getDefaultSettings("inDebugMode", "trackRoutes")
			);
#ifdef STARS_DEBUG_INFO
		if (!byOpponent.list.empty() &&
			!MyShortList::inList(byOpponent.list, byOpponent.move))
			throw logic_error("suggestion not in safe list\n");
#endif
		// analyse->go(byOpponent.player, byOpponent.move);
		// record.push_back(byOpponent);
		// cout << "    input time used: " << byPlayer.time << "ms\n";
		// printf("    %c goes here %d\n", byOpponent.player, byOpponent.move);
		// if (isOver(byOpponent)) {
		// 	printf("Exit from self play mode ...\n");
		// 	return "quit";
		// }

		// recommend
		// byPlayer.hintOn	= record.getDefaultSettings("inDebugMode", "hintOn");
		// if (byPlayer.hintOn)
		// 	cout << "\nHere is hint provided for you\n";
		// byPlayer.suggestion = analyse->respond(byPlayer.player, byPlayer,
			// byPlayer.hintOn,
			// record.getDefaultSettings("inDebugMode", "showTime"),
			// record.getDefaultSettings("inDebugMode", "starsOn"),
			// false);
		// showComment(byPlayer);
		// if (byPlayer.hintOn && byPlayer.word != "bad")
		// 	printf("    %d is recommended\n", byPlayer.suggestion);

		// show
		printf("\n");
		if (record.getDefaultSettings("inDebugMode", "starrySky"))
			analyse->starShow();
		else
			analyse->show();
		printf("\n");

		// change the player
		std::swap(byPlayer.player, byOpponent.player);
		// std::swap(byPlayer.word, byOpponent.word);
		// std::swap(byPlayer.list, byOpponent.list);
		std::swap(byPlayer.move, byOpponent.move);
	}

	return "debug";
}

string BoardInterface::getHelp(string mode) {
	string enjoy  = "Enjoy!\n";
	string addon  = "";
	string end	  = "------------------------------------ The End ----------------------------------\n";
	string enterForMore =
		"------------------------- 'Enter' for more, 0 to quit -------------------------\n";
	string add = addon + "Type in player (X or 0) and move (1~8) to place a piece, for example, type in\n" +
		"X 3\nwill place a pice X in column 3, you can also type x-7 if you want. Just make\n" +
		"sure there's one and only one character between player and its move.\n" +
		"Hit 'Enter' to exit add mode.\n";
	string reverse = "Type in column number to reverse that action. Hit 'Enter' to exit\n";
	string moreAddOrReverse =
		addon + "If you don't know what's going on, why are you here. Why don't you exit from here\n" +
		"and see what the help information in debug mode have to say?\n" + enjoy + end;
	string normal =
		addon + "Type in numbers (1~8) to play, one who place four piece in a row first wins\n\n" +
		"0/exit ---------- exit from a certain mode\n" +
		"q/quit ---------- quit the whole game\n" +
		"C/custom -------- custom board height, width and win number (4 by default)\n" +
		"h/help ---------- show help message of the current mode\n" +
		"p/play ---------- into play mode\n" +
		"P/play back ----- into play back mode\n" +
		"S/show ---------- show the current board\n" +
		"s/settings ------ view and change the settings\n" +
		"st/show stars --- show debug analyse stars\n" +
		"sv/save --------- save the current game\n" +
		"sr/show routes -- show routes that the computer has examined\n" +
		"t/tips ---------- tips I wrote to help other player (you) to play the game\n" +
		"w/winn ---------- show win number (4 by default) in case you forgot\n" +
		"i/info ---------- information about the game\n\n" + enterForMore;
	vector<string> moreDebug = {
		addon +
			"If hintOn is true, then when the computer says word = good, then you'll win (if\n" +
			"starsOn is false) in a few steps if you chose to take the step within the list\n" +
			"that follows, and if there's no bugs :-)\n" + enterForMore,
		addon + "If word=free, list=[1, 5] but you can see that there are plenty of column that\n" +
			"is not full but out side of that [1, 5] list, it is recommended that you take\n" +
			"the step within the list for every Move outside the safe list is risky.\n"
			"Note that area that's covered by the stars cannot be accessed by the computer\n" +
			"easily, therefore might contain surprise or danger.\n"+ enterForMore,
		addon +
			"a/add      - into add mode: add some moves\n" +
			"c/change   - change (swap) the player\n" +
			"H/hint     - show hint for the previous step\n"
			"I/import   - import a new board from input, which must fit in the current board\n" +
			"m/move     - force the computer to take a move now\n" +
			"r/reverse  - into reverse mode: reverse some moves\n" +
			"\nKnowing that one can reverse an action might discourage one from thinking\n" +
			"carefully before one take a move. It's easy to get bored playing games like\n"
			"that.\n" + enjoy + end
		};
	if (mode == "debug") {
		cout << normal;
		char dis[16];
		int	 wrongInput = 0;
		int	 i			= 0;
		while (i < 3) {
			printf("> ");
			cin.getline(dis, 16);
			if (!strcmp(dis, "0") || !strcmp(dis, "exit"))
				break;
			if (!strcmp(dis, "quit") || !strcmp(dis, "q"))
				return "quit";
			else if (i < 3 && (!strlen(dis) || !strcmp(dis, "m") || !strcmp(dis, "more"))) {
				cout << moreDebug[i];
				++i;
			} else {
				cout << "Pardon?\n";
				++wrongInput;
			}
			if (wrongInput > 8) {
				cout << "There might be an overflow here, please don't enter more than 15 characters at "
					"a\ntime\n";
				cin.clear();
				wrongInput = 0;
			}
		}
		return addon;
	}
	if (mode == "add")
		return add + moreAddOrReverse;
	if (mode == "reverse")
		return reverse + moreAddOrReverse;
	if (mode == "settings") {
		string settings = addon + 
			"Each situation represent a situation where questions might be asked, you can\n" +
			"decide whether it will be asked, and dispite whether it IS asked, the default\n" +
			"answer would be. Notice that, for example, in situation gameIsOver, if\n" +
			"askToReverse is false but defaultDebug is true, then when game is over, we will\n" +
			"went into debug mode immediately.\n" +
			"If starsOn is true, then stars will fall down from the sky then make the\n" +
			"computer think fast and reckless.\n" +
			"Most of them is turned down by default to keep it simple.\n";
		return settings;
	}
	return reverse;
}

string BoardInterface::getInfo(string input) {
	string addon = "";
#ifdef STARS_VERSION_DEBUG
	string version = STARS_VERSION_DEBUG;
#else
#ifdef STARS_VERSION_RELEASE
	string version = STARS_VERSION_RELEASE;
#endif
#endif
	string enjoy = "Enjoy!\n";
	string end	 = "----------------------------------- The End -----------------------------------\n";
	string tips	 = addon + "Tips from CharmedPython:\n" +
		"So the trick is to build yourself as high as possible, but don't make hugh\n" +
		"chunks, leave some room between them. Here are some good examples:\n" +
		"1 2 3 4 5 6 7 8\n" + "| | | | | | | | |\n" + "| | | | | | | | |\n" +
		"| | | | | |0| | |\n" + "|0| | | | |X| | |\n" + "|X| |X|0| |X| | |\n" +
		"|0| |X|X| |X| | |\n" + "|0| |X|X| |0|X| |\n" + "|0|0|0|X|0|X|0| |\n" + "\nor\n" +
		"1 2 3 4 5 6 7 8\n" + "| | | | | | | | |\n" + "| |0| | | | | | |\n" +
		"| |0| | | | | | |\n" + "| |X| | |0| | | |\n" + "| |X| |0|X| | | |\n" +
		"| |X| |X|X| | | |\n" + "| |0| |X|X| | | |\n" + "|0|0| |X|0|X|0|0|\n" +
		"------------------------- 'Enter' for more, q to quit -------------------------\n";
	string moreTips = addon +
		"This type of layout will give you a lot of advantage. However, this program is\n" +
		"designed to prevent you from building yourself too high - it's always trying to\n" +
		"interrupt but build it anyway. Actually, trying to build any kind of three\n" +
		"piece in a row is helpful.\nGood luck!\n" + end;
	string info = addon +
		"\nA 1v1 & 8x8 command line based board game\n" +
#ifndef STARS_DEBUG_INFO
		"\n------------------------------- version " + version + " ---------------------------------\n" +
#endif
#ifdef STARS_DEBUG_INFO
		"\n---------------------------- version " + version + " -------------------------------\n" +
#endif
		"                                                                  by Duan Hanyu\n" +
		"                                                                      2020-8-25";
	vector<string> story = {
		addon +
		"Out of a few random try, you finally get yourself an Easter Egg! But I'm afraid\n" +
		"this is actually wrote for myself, so it's quite boring for other players. Not\n" +
		"too late to quite now.\n" +
		"------------------------- 'Enter' for more, q to quit --------------------------\n",
		addon +
		"Anyway, I have just finished an interview when I see my roommate\n" +
		"coding. He told me it was his homework, I got interested. Then I begin to work\n"
		"on this silly thing till I found that it's taking up all of my time and I'm not\n" +
		"even satisfied? Like it's a weird addictive activity that does even provide\n" +
		"satisfaction like another other decent addictive activity will. Watching Game of\n" +
		"Thrones all day might indicate that someone's life is decaying, but at least\n" +
		"that \"feels good\". Now I'm just coding for almost no reason at all, there's\n" +
		"not much technique here, doesn't require any brain work. Brick by brick, anyone\n" +
		"can build this. I mean, what? I didn't go to school so that I can sit here and\n" +
		"type all day. And there's always new features to develop and new bugs to fix.\n" +
		"Now I really get it when my cousin say you can never finish a project, it's a\n" + "disaster.\n" +
		"------------------------- 'Enter' for more, q to quit --------------------------\n",
		addon +
		"But when I say end a project instead of stop coding, I suddenly realized that it\n" +
		"IS my fault that the project never ends 'cause I never said WHEN it ends. When?\n" +
		"Do I stop once the computer can play at least five moves? Do I stop once I've\n" +
		"developed a recursive analyse function? Do I stop once play back feature is\n" +
		"developed? Maybe I should have set a goal, a point, where once it was met, I\n" +
		"stop. Maybe I should do that whenever I start to do anything?\n" + "So... where should I stop?\n" +
		"\n" + "Well, one have to admit, typing that IS refreshing. The developer is OK now.\n" +
		"Sorry you have to see all these irrelevant crap. Have a nice day!\n" + end
};
	if (input == "t" || input == "tips") {
		cout << tips;
		char dis[8];
		int	 wrongInput = 0;
		while (true) {
			printf("> ");
			cin.getline(dis, 8);
			if (!strcmp(dis, "0") || !strcmp(dis, "exit"))
				break;
			if (!strcmp(dis, "quit") || !strcmp(dis, "q"))
				return "quit";
			else if (!strlen(dis) || !strcmp(dis, "m") || !strcmp(dis, "more")) {
				cout << moreTips;
				break;
			}
			printf("Pardon?\n");
			++wrongInput;
			if (wrongInput > 8) {
				cout << "There might be an overflow here, please don't enter "
					 << "more than 15 characters at a\ntime\n";
				cin.clear();
				wrongInput = 0;
			}
		}
		return addon;
	}
	if (input == "story") {
		char  dis[16];
		int	  wrongInput = 0;
		short i			 = 1;
		cout << story[0] << "> ";
		cin.getline(dis, 16);
		if (!strcmp(dis, "q") || !strcmp(dis, "0") || !strcmp(dis, "quit") || !strcmp(dis, "exit"))
			return addon;
		cout << story[1] << "> ";
		cin.getline(dis, 16);
		if (!strcmp(dis, "q") || !strcmp(dis, "0") || !strcmp(dis, "quit") || !strcmp(dis, "exit"))
			return addon;
		cout << story[2];
		return addon;
	}
	if (input == "a song, please") {
		string aSong = addon +
		"\n    Oh my darling,\n" +
		"    oh my darling,\n" +
		"    oh my darling Clementine\n\n" +
		"    You are lost and\n" +
		"    gone forever,\n" +
		"    dreadful sorry Clementine\n\n" +
		"    How I missed her,\n" +
		"    how I missed her,\n" +
		"    how I missed my Clementine\n\n" +
		"    Until I kissed her\n" +
		"    little sister\n" +
		"    and forgot my Clementine";
		return aSong;
	}
	return info;
}

void BoardInterface::showComment(oneMove& move) {
	// comment
	if ((move.word == "good" && move.byComputer) || (move.word == "bad" && !move.byComputer))
		cout << "    not a promising future I'm afraid.\n";
	else if ((move.word == "bad" && move.byComputer) || (move.word == "good" && !move.byComputer))
		cout << "    this is going really well~\n";
	else {
		shortv non;
		analyse->state.nonFullColumn(non);
		if (move.list.size() == 1 && move.byComputer)
			cout << "    one move left, where can I go then?\n";
		else if (move.list.size() == 1 && !move.byComputer)
			cout << "    one move left, where can you go then?\n";
		else if (non.size() > 5 && move.list.size() <= non.size() / 2)
			cout << "    such broad space, yet so few of them is available.\n";
		else if (non.size() < 4)
			cout << "    not much space to chose form.\n";
	}
}

bool BoardInterface::askToReverse(bool yes) {
	char input[8];
	if (yes)
		printf("Care for reverse mode? (default yes) (Yes/no)> ");
	else
		printf("Care for reverse mode? (default no) (yes/No)> ");
	cin.getline(input, 8);
	if ((yes && !strlen(input)) || !strcmp(input, "Y") || !strcmp(input, "yes")
		|| !strcmp(input, "y")) {
		printf("As you wish\n");
		return true;
	}
	else
		printf("No it is.\n");
	return false;
}

void BoardInterface::askToSaveBoard(bool yes) {
	char input[8];
	if (yes)
		printf("Save the old game? (yes as default) (Yes/no)> ");
	else
		printf("Save the old game? (no as default) (yes/No)> ");
	cin.getline(input, 8);
	if ((yes && !strlen(input)) || !strcmp(input, "Y") || !strcmp(input, "yes")
		|| !strcmp(input, "y"))
		record.saveGame(analyse->state);
}

void BoardInterface::importNewBoard() {
	printf("Paste a board down below>\n");
	if (!getStateFromInput())
		return;
	record.clearHistoryMove();
}

bool BoardInterface::isOver(const oneMove& move) {
	if (analyse->gameIsOver() == move.player) {
		analyse->show();
		if (move.byComputer)
			printf("Game is over, stars are gone.\n");
		else
			printf("Congratulations, you win!\n");
		return true;
	}
	if (analyse->gameIsOver() == analyse->rPlayer(move.player)) {
		analyse->show();
		if (move.byComputer)
			printf("Congratulations, you win!\n");
		else
			printf("Game is over, stars are gone.\n");
		return true;
	}
	if (analyse->boardIsFull()) {
		printf("board is full, game is over, lura is gone.\n");
		return true;
	}
	return false;
}

#ifdef STARS_DEBUG_INFO
int BoardInterface::autoTestMode(int startMove) {
	int errCount = 0, stepCount = 1;
	char expectedWinner = '1';
	// show time
	oneMove byOpponent, byPlayer;
	byPlayer.player	  = 'X';
	byPlayer.move	  = startMove;
	byOpponent.player = '0';

	printf("We are in auto test mode now\n");

	// main loop
	while (true) {
		// player goes
		analyse->go(byPlayer.player, byPlayer.move);
		printf("%d:'%c' goes '%d'\t", stepCount, byPlayer.player, byPlayer.move);
		if (isOver(byPlayer)) {
			if (analyse->gameIsOver() != expectedWinner)
				cout << "What? why?\n";
			printf("Exit from auto test mode ...\n");
			return errCount;
		}

		// opp respond
		try {
			byOpponent.move = analyse->respond(byOpponent.player, byOpponent,
				false, false,
				record.getDefaultSettings("inDebugMode", "starsOn"),
				false);
			
			// see if expectedWinner is changed
			if (byOpponent.word == "good" && stepCount > 3) {
				if (expectedWinner != byOpponent.player && expectedWinner != '1') {
					++errCount;
					cout << "Expected winner changed from "
						<< expectedWinner << " to "
						<< byOpponent.player << ", why?\n";
					cout << "player " << byPlayer.player << " goes "
						 << byPlayer.move << ", then opp respond with the board:\n";
					analyse->starShow();
					printf("\n\n\n\n");
					expectedWinner = byOpponent.player;
				}
				else
					expectedWinner = byOpponent.player;
			}
			else if (byOpponent.word == "bad" && stepCount > 3) {
				if (expectedWinner != byPlayer.player && expectedWinner != '1') {
					++errCount;
					cout << "Expected winner changed from "
						<< expectedWinner << " to "
						<< byPlayer.player << ", why?\n";
					cout << "player " << byPlayer.player << " goes "
						 << byPlayer.move << ", then opp respond with the board:\n";
					analyse->starShow();
					printf("\n\n\n\n");
					expectedWinner = byPlayer.player;
					//cout << "Hit anything to continue ...";
					//cin.get();
				}
				else
					expectedWinner = byPlayer.player;
			}
		}
		catch(const std::exception& e) {
			errCount++;
			std::cerr << e.what() << '\n';
			cout << "Error occurred after player '" << byPlayer.player
				 << "' goes " << byPlayer.move
				 << "\nWhen another player is responding to the following board:\n";
			analyse->starShow();

			// if in Visual Studio
			cout << "Let's try again and let VS catch the error>\n";
			byOpponent.move = analyse->respond(byOpponent.player, byOpponent,
				false, false,
				record.getDefaultSettings("inDebugMode", "starsOn"), false);
		}
#ifdef STARS_DEBUG_INFO
		if (!byOpponent.list.empty() &&
			!MyShortList::inList(byOpponent.list, byOpponent.move))
			throw logic_error("suggestion not in safe list");
#endif // STARS_DEBUG_INFO

		// change the player
		std::swap(byPlayer.player, byOpponent.player);
		std::swap(byPlayer.move, byOpponent.move);
		++stepCount;
	}
	return errCount;
}
#endif // STARS_DEBUG_INFO
