#include "BoardInterface.h"

bool BoardInterface::getStateFromInput() {
	short  row = analyse->getRows(), column = analyse->getCols();
	short  minor = 0;
	/*******************************temp code**********************************/
	// don't really know why I need this eater
	char* inputEater = new char[SHORTV_LENGTH * 2 + 2];
	cin.getline(inputEater, SHORTV_LENGTH * 2 + 2);
	if (strlen(inputEater) != column * 2 + 1) {
		cout << "Note that the imported board must fit in with the current board size.\n";
		delete[] inputEater;
		return false;
	}
	char** input = new char*[row];
	if (inputEater[0] == '\0')
		printf("inputEater=='\\0' and why?\n");
	else {
		input[0] = new char[column * 2 + 2];
		strcpy(input[0], inputEater);
		minor = 1;
	}
	delete[] inputEater;
	/*******************************temp code**********************************/
	/***************************************************************************
	this will be much faster if I let BoardInterface change char** board
	directly
	for (short i = 0; i < row; ++i)
	{
		char *temp = new char[column * 2 + 2];
		cin.getline(temp, column * 2 + 2);
		for (short j = 0; j < column; ++j)
			analyse->board.board[j][row-i-1] = temp[j * 2 + 1];
		delete[] temp;
	}
	***************************************************************************/
	for (short i = 0 + minor; i < row; ++i) {
		input[i] = new char[column * 2 + 2];
		cin.getline(input[i], column * 2 + 2);
	}

	char** temp = new char*[column];
	for (short i = 0; i < column; ++i) {
		temp[i] = new char[row];
		for (short j = 0; j < row; ++j){
			if (input[row - j - 1][i * 2 + 1] == '+')
				input[row - j - 1][i * 2 + 1] = ' ';
			temp[i][j] = input[row - j - 1][i * 2 + 1];
		}
	}

	analyse->state.refreshBoard(temp);
	analyse->state.refreshTop();
	if (row == column)
		for (short i = 0; i < row; ++i) {
			delete[] input[i];
			delete[] temp[i];
		}
	else {
		for (short i = 0; i < row; ++i)
			delete[] input[i];
		for (short i = 0; i < column; ++i)
			delete[] temp[i];
	}

	delete[] input;
	delete[] temp;
	return true;
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

		// excute
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
		else if (!strcmp(input, "i") || !strcmp(input, "info") || !strcmp(input, "story") ||
				 !strcmp(input, "t") || !strcmp(input, "tips"))
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
	// invalid valid debug help info
	int						 counter = 0;
	system_clock::time_point start;
	system_clock::time_point end;
	string					 getter;
	while (true) {
		if (counter > 64)
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
		else if (!strcmp(input, "q") || !strcmp(input, "quit"))
			return "quit";
		else if (input[0] == '\0') {
			++counter;
			cout << "Invalid empty input, let\'s try again\n";
		}
		else if (!strcmp(input, "C") || !strcmp(input, "custom"))
			return "custom";
		else if (!strcmp(input, "h") || !strcmp(input, "help")) {
			getter = getHelp("debug");
			if (getter != "quit")
				cout << getter << endl;
			else
				return getter;
		}
		else if (!strcmp(input, "i") || !strcmp(input, "info") || !strcmp(input, "story") ||
				!strcmp(input, "t") || !strcmp(input, "tips") || !strcmp(input, "a song, please")) {
			getter = getInfo("debug");
			if (getter != "quit")
				cout << getter << endl;
			else
				return getter;
		}
		else if (!strcmp(input, "P") || !strcmp(input, "play back") || !strcmp(input, "playback"))
			return "playBack";
		else if (!strcmp(input, "p") || !strcmp(input, "play") || !strcmp(input, "play"))
			return "play";
		else if (!strcmp(input, "S") || !strcmp(input, "show")) {
			if (record.getSettings("inDebugMode", "starrySky"))
				analyse->starShow();
			else
				analyse->show();
		}
		else if (!strcmp(input, "sv") || !strcmp(input, "save"))
			record.saveGame(analyse->state);
		else if (!strcmp(input, "sa") || !strcmp(input, "st") || !strcmp(input, "show stars"))
			analyse->starShow();
		else if (!strcmp(input, "s") || !strcmp(input, "setting") || !strcmp(input, "settings"))
			return "settings";
		else if (!strcmp(input, "a") || !strcmp(input, "add"))
			return "add";
		else if (!strcmp(input, "d") || !strcmp(input, "debug"))
			printf("We are already in debug mode\n");
		else if (!strcmp(input, "H") || !strcmp(input, "hint"))
			return input;
		else if (!strcmp(input, "m") || !strcmp(input, "move") || !strcmp(input, "I") ||
					!strcmp(input, "import") || !strcmp(input, "c") || !strcmp(input, "change"))
			return input;
		else if (!strcmp(input, "r") || !strcmp(input, "reverse"))
			return "reverse";
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
		printf("player '%c' goes move '%d' as you like it:\n", move.player, move.move);
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
		printf("remove %d as you like it:\n", move.move);
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
		// analyse->show();
		input = getInput(byPlayer.player, byPlayer.time);
		if (input == "exit" || input == "quit") {
			printf("Exit from debug mode\n");
			return input;
		}
		if (input == "add" || input == "reverse" || input == "settings" || input == "custom" ||
			input == "play" || input == "playBack")
			return input;
		if (input == "c" || input == "change") {
			// change player
			std::swap(byPlayer.player, byOpponent.player);
			std::swap(byPlayer.word, byOpponent.word);
			std::swap(byPlayer.list, byOpponent.list);
			std::swap(byPlayer.suggestion, byOpponent.suggestion);

			// print hint
			if (record.getSettings("inDebugMode", "hintOn")) {
				printf("Here was %c's word, list, and suggestion:\n", byPlayer.player);
				cout << "word = " << byPlayer.word << "\nlist = [ ";
				for (short move : byPlayer.list)
					printf("%d ", move);
				printf("]\n");
			}
			continue;
		}
		else if (input == "I" || input == "import") {
			if (record.getSettings("changeBoard", "askToSaveBoard"))
				askToSaveBoard(record.getSettings("changeBoard", "defaultSaveBoard"));
			else if (record.getSettings("changeBoard", "defaultSaveBoard"))
				record.saveGame(analyse->state);
			// new board
			importNewBoard();
			continue;
		}
		else if (input == "H" || input == "hint") {
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
				printf("Exit from debug mode\n");
				return "over";
			}
		}
		else if (isOver(byPlayer)) {
			printf("Exit from debug mode\n");
			return "over";
		}

		// opp respond
		printf("info for the computer:\n");
		byOpponent.move = analyse->respond(byOpponent.player, byOpponent,
			record.getSettings("inDebugMode", "showCalculate"),
			record.getSettings("inDebugMode", "showTime"),
			record.getSettings("inDebugMode", "starsOn"));
		analyse->go(byOpponent.player, byOpponent.move);
		byOpponent.suggestion = byOpponent.move;
		record.push_back(byOpponent);
		cout << "\tinput time used: " << byPlayer.time << "ms\n";
		printf("\t%c goes here %d\n", byOpponent.player, byOpponent.move);
		if (isOver(byOpponent)) {
			printf("Exit from debug mode\n");
			return "over";
		}

		// recommend
		byPlayer.hintOn	= record.getSettings("inDebugMode", "hintOn");
		if (byPlayer.hintOn)
			cout << "\nHere is hint provided for you\n";
		byPlayer.suggestion = analyse->respond(byPlayer.player, byPlayer, byPlayer.hintOn,
			record.getSettings("inDebugMode", "showTime"),
			record.getSettings("inDebugMode", "starsOn"));
		showComment(byPlayer);
		if (byPlayer.hintOn)
			printf("\tthis %d is recommended\n", byPlayer.suggestion);

		// show
		if (record.getSettings("inDebugMode", "starrySky"))
			analyse->starShow();
		else
			analyse->show();
		printf("\n");
	}
	throw runtime_error("control flow into the end of debug mode\n");
	return "quit";
}

string BoardInterface::settingsMode() {
	printf("We are in settings mode now:\n");
	cout << "we have " << record.getSettingsItemNumber() << " situations and within them we have ";
	cout << "multiple items each marked with tags\nbelow, type in tags to change ";
	cout << "these settings, 0 to exit, S/show to show current\nsettings, h/help for help\n";
	// show
	record.showSettingsWithTags();
	cout << "\ncare to change one of them?\n";
	while (true) {
		printf("> ");
		char input[8];
		cin.getline(input, 8);
		if (input[0] == '\0' || !strcmp(input, "0") || !strcmp(input, "exit") || !strcmp(input, "no")) {
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
				cout << "And if you don't know what's going on, you can always "
						"type help, happy to help as always.\n";
			}
		} else {
			cout << "Invalid input, let's try again.\n";
			cout << "And if you don't know what's going on, you can always "
					"type help, happy to help as always.\n";
		}
	}
	throw runtime_error("control flow into the end of settings mode\n");
	return "quit";
}

string BoardInterface::playMode() {
	if (!record.getNumberOfSavedBoard()) {
		cout << "No games has been recorded yet. If you want to try this mode, "
				"save a game first\nTill next time.\nExit from play mode...\n";
		return "debug";
	}
	printf("We are here in play mode.\n");
	printf("You can choose from %d saved games.\n", record.getNumberOfSavedBoard());
	printf("Hit 'Enter' to see more, type in index number to went into\n");
	printf("one of those games. Or type c to continue to play the board that is currently\ndisplayed. ");
	printf("Use 0 to exit.\n");
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
	printf("Hit 'Enter' to see more, type in index number to see one of the games.\n");
	printf("or type c to see the play back of the board that is currently displayed.\n");
	printf("Use 0 to exit.\n");
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
	if (record.getSettings("inCustomMode", "askToSaveBoard"))
		askToSaveBoard(record.getSettings("inCustomMode", "defaultSaveBoard"));
	else if (record.getSettings("inCustomMode", "defaultSaveBoard"))
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
			if (record.getSettings("gameIsOver", "askToDebug") &&
				askToDebug(record.getSettings("gameIsOver", "defaultDebug"))) {
				advice = debugMode(byPlayer);
				continue;
			}
			else if (record.getSettings("gameIsOver", "defaultDebug")) {
				advice = debugMode(byPlayer);
				continue;
			}
			if (record.getSettings("gameIsOver", "askToSaveBoard"))
				askToSaveBoard(record.getSettings("gameIsOver", "defaultSaveBoard"));
			else if (record.getSettings("gameIsOver", "defaultSaveBoard"))
				record.saveGame(analyse->state);
			break;
		}
		++i;
	}
	if (i == 100)
		throw runtime_error("too much unhandled advice\n");
	if (advice == "quit")
		return false;
	// else if (advice == "exit")
	// 	;
	cout << "Exit from controller\n";
	return true;
}

string BoardInterface::getHelp(string mode) {
	string enjoy  = "Enjoy!\n";
	string addon  = "";
	string end	  = "------------------------------------ The End -----------------------------------\n";
	string enterForMore =
		"------------------------- 'Enter' for more, 0 to quit --------------------------\n";
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
			"Knowing that one can reverse an action might discourage one from thinking\n" +
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
			"askToDebug is false but defaultDebug is true, then when game is over, we will\n" +
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
	string enjoy = "Enjoy!\n";
	string end	 = "----------------------------------- The End ------------------------------------\n";
	string tips	 = addon + "Tips from CharmedPython:\n" +
		"So the trick is to build yourself as high as possible, but don't make hugh\n" +
		"chunks, leave some room between them. Here are some good examples:\n" +
		"1 2 3 4 5 6 7 8\n" + "| | | | | | | | |\n" + "| | | | | | | | |\n" +
		"| | | | | |0| | |\n" + "|0| | | | |X| | |\n" + "|X| |X|0| |X| | |\n" +
		"|0| |X|X| |X| | |\n" + "|0| |X|X| |0|X| |\n" + "|0|0|0|X|0|X|0| |\n" + "\nor\n" +
		"1 2 3 4 5 6 7 8\n" + "| | | | | | | | |\n" + "| |0| | | | | | |\n" +
		"| |0| | | | | | |\n" + "| |X| | |0| | | |\n" + "| |X| |0|X| | | |\n" +
		"| |X| |X|X| | | |\n" + "| |0| |X|X| | | |\n" + "|0|0| |X|0|X|0|0|\n" +
		"------------------------- 'Enter' for more, q to quit --------------------------\n";
	string moreTips = addon +
		"This type of layout will give you a lot of advantage. However, this program is\n" +
		"designed to prevent you from building yourself too high - it's always trying to\n" +
		"interrupt but don't worry, build it anyway. It's quite easy to win if you go\n" +
		"first, which gives you lots of advantage. And you always have debug mode to\n" +
		"reverse an action, get performance detail from the computer, etc. But that's for\n" +
		"debugging originally. Em, I wonder what other functions it can perform for other\n" +
		"players. Difficult to answer without actual practice wouldn't you say?\n" + enjoy +
		"Good luck\n" + end;
	string info = addon +
		"A 1v1 & 8x8 command line based board game\n" +
		"\n------------------------------ version 1.1 - Beta ------------------------------\n" +
		"                                                                    by DuanHanyu\n" +
		"                                                                        2020-8-5";
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
		"Sorry you have to see all these irrelevant crap. Have a nice day\n" + end
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
				printf(
					"There might be an overflow here, please don't enter more than 15 characters at "
					"a\ntime\n");
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
		"\n    Oh my darling, oh my darling, oh my darling Clementine\n" +
		"    You are lost and gone forever, dreadful sorry Clementine\n" +
		"    How I missed her, how I missed her, how I missed my Clementine\n" +
		"    Until I kissed her little sister and forgot my Clementine";
		return aSong;
	}
	return info;
}

void BoardInterface::showComment(oneMove& move) {
	// comment
	if ((move.word == "good" && move.byComputer) || (move.word == "bad" && !move.byComputer))
		cout << "\tNot a promising future I'm afraid.\n";
	else if ((move.word == "bad" && move.byComputer) || (move.word == "good" && !move.byComputer))
		cout << "\tThis is going really well~\n";
	else {
		shortv non;
		analyse->state.nonFullColumn(non);
		if (move.list.size() == 1 && move.byComputer)
			cout << "One move left, where can I go then?\n";
		else if (move.list.size() == 1 && !move.byComputer)
			cout << "One move left, where can you go then?\n";
		else if (non.size() > 5 && move.list.size() <= non.size() / 2)
			cout << "Such broad space, yet so few of them is available.\n";
		else if (non.size() < 4)
			cout << "Not much space to chose form.\n";
	}
}

bool BoardInterface::askToDebug(bool yes) {
	char msg[8];
	if (yes)
		printf("care for a debug mode? (default yes) (Yes/no)> ");
	else
		printf("care for a debug mode? (default no) (yes/No)> ");
	cin.getline(msg, 8);
	if ((yes && !strlen(msg)) || !strcmp(msg, "Y") || !strcmp(msg, "yes") || !strcmp(msg, "y")) {
		printf("As you wish\n");
		return true;
	} else
		printf("no it is.\n");
	return false;
}

void BoardInterface::askToSaveBoard(bool yes) {
	char msg[8];
	if (yes)
		printf("Save the old game? (yes as default) (Yes/no)> ");
	else
		printf("Save the old game? (no as default) (yes/No)> ");
	cin.getline(msg, 8);
	if ((yes && !strlen(msg)) || !strcmp(msg, "Y") || !strcmp(msg, "yes") || !strcmp(msg, "y"))
		record.saveGame(analyse->state);
}

void BoardInterface::importNewBoard() {
	printf("import a new board>\n");
	if (!getStateFromInput())
		return;
	record.clearHistoryMove();
	printf("Here's the imported board we are gonna use:\n");
	analyse->show();
	printf("hit 'Enter' to continue> ");
	cin.clear();
	char disposal[8];
	cin.getline(disposal, 8);
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
