#include "BoardInterface.h"

void BoardInterface::getStateFromInput() {
	short  row = analyse->state.row, column = analyse->state.column;
	char** input = new char*[row];
	short  minor = 0;
	/*******************************temp code**********************************/
	// don't really know why I need this eater
	char* inputEater = new char[column * 2 + 2];
	cin.getline(inputEater, column * 2 + 2);
	if (inputEater[0] == '\0')
		printf("inputEater=='\\0' and why?\n");
	else {
		// printf("no need for an eater? why?\n");
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
		for (short j = 0; j < row; ++j)
			temp[i][j] = input[row - j - 1][i * 2 + 1];
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
}

// mode = "reverse", "add"
string BoardInterface::getInput(const string mode) {
	double dis;
	// maybe we should put add mode and reverse mode here!
	if (mode == "normal" || mode == "debug")
		return getInput(mode, '0', dis);

	// main loop
	while (true) {
		if (mode == "reverse")
			printf("\nIn reverse mode\nEnter move> ");
		else if (mode == "add")
			printf("\nIn Add mode\nEnter player(blank space)move> ");
		char input[INTER_MAX_INPUT];
		cin.getline(input, INTER_MAX_INPUT);

		// excute
		if (input[0] == '\0' || !strcmp(input, "0") || !strcmp(input, "q") || !strcmp(input, "exit") ||
			!strcmp(input, "quit")) {
			cout << "Exit from " << mode << " mode.\n";
			input[0] = '\0';
			return input;
		} else if (!strcmp(input, "r") || !strcmp(input, "reverse")) {
			if (mode != "add")
				printf("We are already in reverse mode\n");
			else
				reverseMode();
		} else if (!strcmp(input, "a") || !strcmp(input, "add")) {
			if (mode != "reverse")
				printf("We are already in add mode\n");
			else
				addMode();
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
			if (num > 0 && num <= analyse->getColumn() && !analyse->colIsEmpty(num))
				return input;
			cout << "Invalid " << mode << " mode number input, let\'s try again\n";
		} else if (mode != "reverse") {
			char temp[INTER_MAX_INPUT];
			strcpy(temp, input + 2);
			int num = atoi(temp);
			if ((input[0] == 'X' || input[0] == 'x' || input[0] == '0') && num > 0 &&
				num <= analyse->getColumn() && !analyse->colIsFull(num))
				return input;
		}
	}
}

// mode = "normal", "debug"
string BoardInterface::getInput(const string mode, char plr, double& inputTime) {
	// invalid valid debug help info
	int						 counter = 0;
	system_clock::time_point start;
	system_clock::time_point end;
	while (true) {
		if (counter > 64)
			cin.clear();
		cout << "\nIn " << mode << " mode\nPlayer '" << plr << "' move> ";
		char input[INTER_MAX_INPUT];
		start = system_clock::now();
		cin.getline(input, INTER_MAX_INPUT);
		end			 = system_clock::now();
		auto elapsed = duration_cast<milliseconds>(end - start);
		inputTime	 = elapsed.count();

		int num = atoi(input);
		if (num > 0 && num <= analyse->getColumn() && !analyse->colIsFull(num))
			return input;
		// excute
		if (!strcmp(input, "0") || !strcmp(input, "q") || !strcmp(input, "exit") || !strcmp(input, "quit")) {
			input[0] = '\0';
			return input;
		} else if (input[0] == '\0') {
			++counter;
			cout << "Invalid empty input, let\'s try again\n";
		} else if (!strcmp(input, "C") || !strcmp(input, "custom"))
			customMode();
		else if (!strcmp(input, "h") || !strcmp(input, "help"))
			cout << getHelp(mode) << endl;
		else if (!strcmp(input, "i") || !strcmp(input, "info") || !strcmp(input, "story") ||
				 !strcmp(input, "t") || !strcmp(input, "tips") || !strcmp(input, "a song, please"))
			cout << getInfo(input) << endl;
		else if (!strcmp(input, "P") || !strcmp(input, "play back") || !strcmp(input, "playback"))
			playBackMode();
		else if (!strcmp(input, "p") || !strcmp(input, "play") || !strcmp(input, "play"))
			playMode();
		else if (!strcmp(input, "S") || !strcmp(input, "show"))
			analyse->show();
		else if (!strcmp(input, "save"))
			record.saveGame(analyse->state);
		else if (!strcmp(input, "s") || !strcmp(input, "setting") || !strcmp(input, "settings"))
			settingsMode();
		else if (mode != "normal") {
			if (!strcmp(input, "a") || !strcmp(input, "add"))
				addMode();
			else if (!strcmp(input, "d") || !strcmp(input, "debug"))
				printf("We are already in debug mode\n");
			else if (!strcmp(input, "H") || !strcmp(input, "hint"))
				return input;
			else if (!strcmp(input, "m") || !strcmp(input, "move") || !strcmp(input, "I") ||
					 !strcmp(input, "import") || !strcmp(input, "c") || !strcmp(input, "change"))
				return input;
			else if (!strcmp(input, "n") || !strcmp(input, "normal"))
				normalMode();
			else if (!strcmp(input, "r") || !strcmp(input, "reverse"))
				reverseMode();
		}
		// now mode == "normal"
		else if ((!strcmp(input, "d") || !strcmp(input, "debug")) && mode != "debug")
			return input;
		else if (!strcmp(input, "n") || !strcmp(input, "normal"))
			printf("We are already in normal mode\n");
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
		cout << item << " (1~" << SHORTV_LENGTH - 1 << ") = ";
		cin.getline(input, 16);
		if (!strcmp(input, "0") || !strcmp(input, "exit") || !strcmp(input, "q") || !strcmp(input, "quit"))
			return 0;
		customNumber = atoi(input);
		if (customNumber > 0 && customNumber < SHORTV_LENGTH - 1)
			return customNumber;
		cout << "Let's try again\n";
	}
}

void BoardInterface::addMode() {
	string	input;
	oneMove move;
	move.mode = "add";
	while (true) {
		input = getInput("add");
		if (input.empty())
			return;
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

void BoardInterface::reverseMode() {
	string	input;
	oneMove move;
	move.mode = "reverse";
	while (true) {
		input = getInput("reverse");
		if (input.empty())
			return;
		move.move = atoi(input.c_str());
		printf("remove %d as you like it:\n", move.move);
		analyse->reverse(move.move);
		record.push_back(move);
		analyse->show();
	}
}

void BoardInterface::debugMode(oneMove& byPlayer) {
	// init
	oneMove byOpponent;
	byPlayer.mode		  = "debug";
	byPlayer.byComputer	  = false;
	byOpponent.mode		  = "debug";
	byOpponent.byComputer = true;
	byOpponent.player	  = analyse->rPlayer(byPlayer.player);
	char		disposal[16];
	const short disNumber = 16;
	string		input;
	bool		defalutImport = record.getSettings("inToDebugMode", "defaultImport");
	if (record.getSettings("inToDebugMode", "askToImport")) {
		printf("Import new board (or use the current board)?\nhint: if");
		if (defalutImport)
			printf(" you don't know what's going on, choose no (Yes/no)> ");
		else
			printf(" you don't know what's going on, choose no (yes/No)> ");
		cin.getline(disposal, disNumber);
		if ((defalutImport && !strlen(disposal)) || !strcmp(disposal, "import") || !strcmp(disposal, "I") ||
			!strcmp(disposal, "yes") || !strcmp(disposal, "y"))	 // then import
			defalutImport = true;
		else
			defalutImport = false;
	}
	if (defalutImport) {
		if (record.getSettings("inToDebugMode", "askToSaveBoard"))	// if should ask
			askToSaveBoard(record.getSettings("inToDebugMode", "debugSaveBoard"));
		else if (record.getSettings("inToDebugMode", "debugSaveBoard"))	 // if save
			record.saveGame(analyse->state);
		// new board
		importNewBoard();
	}
	printf("We are in debug mode now\n");

	// main loop
	while (true) {
		// analyse->show();
		input = getInput("debug", byPlayer.player, byPlayer.time);
		if (input.empty())
			break;
		else if (input == "c" || input == "change") {
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
		} else if (input == "I" || input == "import") {
			if (record.getSettings("changeBoard", "askToSaveBoard"))
				askToSaveBoard(record.getSettings("changeBoard", "defaultSaveBoard"));
			else if (record.getSettings("changeBoard", "defaultSaveBoard"))
				record.saveGame(analyse->state);
			// new board
			importNewBoard();
			continue;
		} else if (input == "H" || input == "hint") {
			printf("this is the hint for the previous move:\n");
			cout << "word = " << byPlayer.word << "\nlist = [ ";
			for (short i : byPlayer.list)
				printf("%d ", i);
			printf("]\n");
			continue;
		} else if (input != "m" && input != "move") {
			// player goes
			byPlayer.move = atoi(input.c_str());
			analyse->go(byPlayer.player, byPlayer.move);
			record.push_back(byPlayer);	 // byPlayer end here
			if (isOver(byPlayer))
				break;
		} else if (isOver(byPlayer))
			break;

		// opp respond
		byOpponent.move = analyse->respond(byOpponent.player, byOpponent,
										   record.getSettings("inDebugMode", "showCalculate"),
										   record.getSettings("inDebugMode", "showTime"));
		analyse->go(byOpponent.player, byOpponent.move);
		byOpponent.suggestion = byOpponent.move;
		record.push_back(byOpponent);
		printf("\n%c goes here %d\n", byOpponent.player, byOpponent.move);
		cout << "Player time used: " << byPlayer.time << "ms\n\n";
		if (isOver(byOpponent))
			break;

		// recommend
		byPlayer.hintOn		= record.getSettings("inDebugMode", "hintOn");
		byPlayer.suggestion = analyse->respond(byPlayer.player, byPlayer, byPlayer.hintOn,
											   record.getSettings("inDebugMode", "showTime"));
		showComment(byPlayer);
		if (byPlayer.hintOn)
			printf("this %d is recommended\n", byPlayer.suggestion);

		// show
		analyse->show();
		printf("\n");
	}
	printf("Exit from debug mode...\n");
}

void BoardInterface::normalMode() {
	// init
	string	input;
	oneMove byPlayer, byOpponent;
	byPlayer.player		  = 'X';
	byPlayer.mode		  = "normal";
	byPlayer.byComputer	  = false;
	byOpponent.player	  = '0';
	byOpponent.mode		  = "normal";
	byOpponent.byComputer = true;
	analyse->show();
	while (true) {
		// get input
		input = getInput("normal", byPlayer.player, byPlayer.time);
		if (input.empty())
			break;
		else if (input == "c") {
			byOpponent.player = byPlayer.player;
			byPlayer.player	  = analyse->rPlayer(byOpponent.player);
			byPlayer.word	  = byOpponent.word;
			byPlayer.list	  = byOpponent.list;
			continue;
		} else if (input == "d" || input == "debug") {
			debugMode(byPlayer);
			// maybe in debug mode, the player changes:
			byOpponent.player = analyse->rPlayer(byPlayer.player);
			continue;
		}

		// player goes
		byPlayer.move = atoi(input.c_str());
		analyse->go(byPlayer.player, byPlayer.move);
		record.push_back(byPlayer);
		if (isOver(byPlayer))
			break;

		// opp respond
		byOpponent.move = analyse->respond(byOpponent.player, byOpponent,
										   record.getSettings("inNormalMode", "showCalculate"),
										   record.getSettings("inNormalMode", "showTime"));
		analyse->go(byOpponent.player, byOpponent.move);
		printf("\n%c goes here %d\n", byOpponent.player, byOpponent.move);
		showComment(byOpponent);

		// show
		cout << "Player time used: " << byPlayer.time << "ms\n\n";
		analyse->show();

		byOpponent.suggestion = byOpponent.move;
		record.push_back(byOpponent);
		if (isOver(byOpponent))
			break;

		// player recommend - not shown
		byPlayer.suggestion = analyse->respond(byPlayer.player, byPlayer, false,
											   record.getSettings("inNormalMode", "showTime"));
		printf("\n");
	}
	if (input.empty()) {
		// here is exit from game
		if (record.getSettings("exitNormal", "askToDebug") &&
			askToDebug(record.getSettings("exitNormal", "defaultDebug")))
			debugMode(byPlayer);
		else if (record.getSettings("exitNormal", "defaultDebug"))
			debugMode(byPlayer);
		if (record.getSettings("exitNormal", "askToSaveBoard"))
			askToSaveBoard(record.getSettings("exitNormal", "defaultSaveBoard"));
		else if (record.getSettings("exitNormal", "defaultSaveBoard"))
			record.saveGame(analyse->state);
		printf("Exit from normal mode...\n");
		return;
	}
	// below is game is over
	if (record.getSettings("gameIsOver", "askToDebug") &&
		askToDebug(record.getSettings("gameIsOver", "defaultDebug")))
		debugMode(byPlayer);
	else if (record.getSettings("gameIsOver", "defaultDebug"))
		debugMode(byPlayer);

	if (record.getSettings("gameIsOver", "askToSaveBoard"))
		askToSaveBoard(record.getSettings("gameIsOver", "defaultSaveBoard"));
	else if (record.getSettings("gameIsOver", "defaultSaveBoard"))
		record.saveGame(analyse->state);

	printf("Exit from normal mode...\n");
}

void BoardInterface::settingsMode() {
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
		if (input[0] == '\0' || !strcmp(input, "0") || !strcmp(input, "q") || !strcmp(input, "exit") ||
			!strcmp(input, "quit") || !strcmp(input, "no"))
			break;
		else if (!strcmp(input, "h") || !strcmp(input, "help"))
			cout << getHelp("settings");
		else if (!strcmp(input, "S") || !strcmp(input, "show"))
			record.showSettingsWithTags();
		else if (strlen(input) == 2) {
			int tag1 = input[0] - 'a', tag2 = input[1] - 'a';
			if (!(tag1 < 26 && tag1 > -1 && tag2 < 26 && tag2 > -1 &&
				  record.changeSettingsUsingTags(tag1, tag2))) {
				cout << "Invalid input, let's try again.\n";
				cout << "And if you don't know what's going on, you can always type help, happy to help as "
						"always.\n";
			}
		} else {
			cout << "Invalid input, let's try again.\n";
			cout << "And if you don't know what's going on, you can always type help, happy to help as "
					"always.\n";
		}
	}
	record.saveSettings();
	printf("Exit from settings mode.\n");
}

void BoardInterface::playMode() {
	if (!record.getNumberOfSavedBoard()) {
		printf(
			"No games has been recorded yet. If you want to try this mode, save a game first\nTill next "
			"time.\n");
		printf("Exit from play mode...\n");
		return;
	}
	printf("We are here in play mode.\n");
	printf("You can choose from %d saved games.\n", record.getNumberOfSavedBoard());
	printf("Hit 'Enter' to see more, type in index number to went into\n");
	printf("one of those games. Or type c to continue to play the board that is currently\ndisplayed. ");
	printf("Use 0 to exit.\n");
	printf("Explore!\n");
	Json::Value* game = record.showSavedGames();
	if (!game) {
		printf("Exit from play mode...\n");
		return;
	}

	// set up a new game
	BoardAnalyse   analyser((*game)["state"]);
	BoardInterface interface(analyser);
	BoardRecord	   newRecord;
	newRecord.refreshHistoryMove((*game)["historyMove"]);
	interface.refreshRecord(newRecord);
	printf("Into a new normal mode:\n");
	printf("Use 1~8 to place a piece, h for help, 0 to exit.\n");
	printf("Game is on->\n");
	interface.normalMode();

	printf("Exit from play mode...\n");
}

void BoardInterface::playBackMode() {
	if (!record.getNumberOfSavedBoard()) {
		printf(
			"No games has been recorded yet. If you want to try this mode, save a game first\nTill next "
			"time.\n");
		printf("Exit from play back mode...\n");
		return;
	}
	printf("We are here in play back mode.\n");
	printf("You can choose from %d saved games.\n", record.getNumberOfSavedBoard());
	printf("Hit 'Enter' to see more, type in index number to see one of the games.\n");
	printf("or type c to see the play back of the board that is currently displayed.\n");
	printf("Use 0 to exit.\n");
	printf("Explore!\n");
	Json::Value* game = record.showSavedGames();
	if (!game) {
		printf("Exit from play back mode...\n");
		return;
	}

	// set up a new game
	BoardAnalyse analyser((*game)["state"]["row"].asInt(), (*game)["state"]["column"].asInt(),
						  (*game)["state"]["winn"].asInt());
	BoardRecord	 newRecord, tempRecord;
	newRecord.refreshHistoryMove((*game)["historyMove"]);
	printf("Here's the play back of an old game:\n");
	printf("Use c to cut in and play, h for help, b to go back to the previous step, 0\nto exit.\n");
	char					  input[16];
	bool					  wentBack		= false;
	char					  reversePlayer = ' ';
	vector<oneMove>::iterator iter			= newRecord.historyMove.begin();
	while (iter != newRecord.historyMove.end()) {
		// show
		printf("This is step %lu/%zu\n", tempRecord.historyMove.size() + 1, newRecord.historyMove.size());
		if (wentBack)
			wentBack = false;
		else {
			cout << *iter;
			tempRecord.push_back(*iter);
			if (iter->mode != "reverse")
				analyser.go(iter->player, iter->move);
			else {
				reversePlayer = analyser.state.board[iter->move][analyser.state.top[iter->move - 1] - 1];
				analyser.reverse(iter->move);
			}
		}
		analyser.show();

		// get input
		printf("> ");
		cin.getline(input, 16);
		if (!strcmp(input, "0") || !strcmp(input, "q") || !strcmp(input, "quit") || !strcmp(input, "exit"))
			break;
		else if (!strcmp(input, "c") || !strcmp(input, "cut") || !strcmp(input, "cut in")) {
			BoardAnalyse   tempAnalyser = analyser;
			BoardInterface interface(tempAnalyser);
			interface.refreshRecord(tempRecord);
			printf("Go into a new normal mode:\n");
			interface.normalMode();
			printf("We're back in play back mode again\n");
		} else if (!strcmp(input, "b") || !strcmp(input, "back") || !strcmp(input, "go back")) {
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
		++iter;
	}
	printf("Exit from play back mode...\n");
}

void BoardInterface::customMode() {
	printf("In custom mode ...\n");
	if (record.getSettings("inCustomMode", "askToSaveBoard"))
		askToSaveBoard(record.getSettings("inCustomMode", "defaultSaveBoard"));
	else if (record.getSettings("inCustomMode", "defaultSaveBoard"))
		record.saveGame(analyse->state);
	short column, row, winn;
	column = getCustomInput("board width");
	if (!column)
		return;
	row = getCustomInput("board height");
	if (!row)
		return;
	winn = getCustomInput("win number");
	if (!winn)
		return;
	analyse->customBoard(column, row, winn);
	record.clearHistoryMove();
	printf("Here's the board we're gonna use:\n");
	analyse->show();
	printf("Exit from custom mode ...\n");
}

string BoardInterface::getHelp(string mode) {
	string enjoy  = "Enjoy!\n";
	string addon  = "";
	string theend = "------------------------------- The end, q to quit -----------------------------\n";
	string end	  = "------------------------------------ The End -----------------------------------\n";
	string enterForMore =
		"------------------------- 'Enter' for more, q to quit --------------------------\n";
	string add = addon + "Type in player (X or 0) and move (1~8) to place a piece, for example, type in\n" +
				 "0 3\nwill place a pice X in column 3, you can also type x-7 if you want. Just make\n" +
				 "sure there's one and only one character between player and its move.\n" +
				 "Hit 'Enter' to exit.\n" + enterForMore;
	string reverse = "Type in column number to reverse that action. Hit 'Enter' to exit\n" + enterForMore;
	string moreAddOrReverse =
		addon + "If you don't know what's going on, why are you here. Why don't you exit from here\n" +
		"and see what the help information in debug mode have to say?\n" + enjoy + end;
	string normal =
		addon + "Type in numbers (1~8) to play, one who place four piece in a row first wins\n\n" +
		"0/q/quit/exit -- exit from a certain mode or exit the game\n" +
		"C/custom ------- custom board height, width and win number (4 by default)\n"
		"d/debug -------- into debug mode\n" +
		"h/help --------- show help message of the current mode\n" +
		"p/play --------- into play mode\n" +
		"P/play back ---- into play back mode\n" +
		"save ----------- save the current game\n" +
		"S/show --------- show the current board\n" +
		"s/settings ----- view and change the settings\n" +
		"t/tips --------- tips I wrote to help other player (you) to play the game\n" +
		"w/winn --------- show win number (4 by default)\n"
		"i/info --------- information about the game\n\n" +
		enjoy + end;
	string debug = addon +
		"You are in debug mode now, It's quite the same with normal mode, just that you\n" +
		"can get some hint from the computer from time to time. If the computer says\n" +
		"word=good, then you'll win in a few steps, just take the step within the list\n" +
		"that follows, then you will win - if there's no bugs :-)\n" + enterForMore;
	vector<string> moreDebug = {
		addon + "If word=free, list=[1, 5] but you can see that there are plenty of column that\n" +
			"is not full but out side of that [1, 5] list, it is recommended that you take\n" +
			"the step within the list for every Move outside the safe list is risky. But\n" +
			"surely one can choose wherever column one want, but it seems neccesary to warn.\n" +
			"Notice: maybe you do as I told and there are bugs here and you failed anyway\n" +
			"well, that is my fault then.\n" + enterForMore,
		addon + "no more \"information\"~~\n" + enterForMore,
		addon + "well, there are some \"disinformation\" that doesn't count as \"information\"\n" +
			"Some funtions in this program that ordinary players cannot harness. One might\n" +
			"argue that these ancient artifacts might cause harm to the experience. Do you\n" +
			"wish to continue?\n" + enterForMore,
		addon +
			"a/add         - into add mode: add some moves\n" +
			"c/change      - change (swap) the player\n" +
			"H/hint        - show hint for the previous step\n"
			"I/import      - import a new board from input\n" +
			"m/move        - force the computer to take a move now\n" +
			"r/reverse     - into reverse mode: reverse some moves\n" +
			"\nthis looks like cheating, but trust me, I actually developed it for debug\n" +
			"purposes - I got to test different kinds of board - how the program react to\n" +
			"them, is the performance satisfying, etc. And here we are~\n" +
			"\nAbout that \"cause harm\" thing, knowing that you can reverse an action might\n" +
			"discourage you from thinking carefully before you play, plus that \"hint\" you\n" +
			"get from the computer, it's easy to get bored playing this game. Now that you\n" +
			"know it, and you can't unknow it, just relax and try to enjoy, there's plenty of\n" +
			"games out there. Just don't get too addicted at finding such meaningless \"short\n" +
			"cuts\".\n" + enjoy + theend,
		addon + "What kind of person will hit more after seeing \"The end\"? it must be my fault\n" +
			"I have lied before with that silly {no more \"information\"~~} information.\n" +
			"The developer have lost his credibility.\n" +
			"Now the developer is thinking about how to regain his credibility by making\n" +
			"statements such as The end is not the end, The End is the true end.\n" + end,
		addon + "One mustn't trust those who have lied and those who have never lied.\n",
		addon + "That is not true\n"};
	if (mode == "normal")
		return normal;
	if (mode == "debug") {
		cout << debug;
		char dis[16];
		int	 wrongInput = 0;
		int	 i			= 0;
		while (i < 6) {
			printf("> ");
			cin.getline(dis, 16);
			if (!strcmp(dis, "q") || !strcmp(dis, "0") || !strcmp(dis, "quit") || !strcmp(dis, "exit"))
				break;
			else if (i < 4 && (!strlen(dis) || !strcmp(dis, "m") || !strcmp(dis, "more"))) {
				cout << moreDebug[i];
				++i;
			} else if (i >= 4) {
				if ((!strcmp(dis, "m") || !strcmp(dis, "more"))) {
					cout << moreDebug[4];
					break;
				} else if (i == 4) {
					break;
				}
			} else {
				cout << "Pardon?\n";
				++wrongInput;
			}
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
	if (mode == "add" || mode == "reverse") {
		if (mode == "add")
			cout << add;
		else
			cout << reverse;
		char dis[16];
		int	 wrongInput = 0;
		while (true) {
			printf("> ");
			cin.getline(dis, 16);
			if (!strcmp(dis, "q") || !strcmp(dis, "0") || !strcmp(dis, "quit") || !strcmp(dis, "exit"))
				break;
			else if (!strlen(dis)) {
				cout << moreAddOrReverse;
				break;
			} else {
				cout << "Pardon?\n";
				++wrongInput;
			}
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
	if (mode == "settings") {
		string settings = addon + 
			"Each situation represent a situation where questions might be asked, you can\n" +
			"decide whether it will be asked, and dispite whether it IS asked, the default\n" +
			"answer would be. Notice that, for example, in situation gameIsOver, if\n" +
			"askToDebug is false but defaultDebug is true, then when game is over, we will\n" +
			"went into debug mode immediately.\n" +
			"Personally, I only let exitNormal:askToSaveBoard and whenSaveGame:askGiveName\n" +
			"to be true, don't know why I wrote all those settings...\n";
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
		"\n------------------------------ version 1.0 - Beta ------------------------------\n" +
		"                                                                    by DuanHanyu\n" +
		"                                                                        2020-8-2";
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
			if (!strcmp(dis, "q") || !strcmp(dis, "0") || !strcmp(dis, "quit") || !strcmp(dis, "exit"))
				break;
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
		cout << "Not a promising future I'm afraid.\n";
	else if ((move.word == "bad" && move.byComputer) || (move.word == "good" && !move.byComputer))
		cout << "This is going really well~\n";
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
	getStateFromInput();
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
			printf("Game is over, good luck next time!\n");
		else
			printf("Congratulations, you win!\n");
		return true;
	}
	if (analyse->boardIsFull()) {
		printf("board is full, game is over, lura is gone.\n");
		return true;
	}
	return false;
}
