/*-- encoding: UTF-8 --*/
#include "boardInterface.h"

BoardInterface::BoardInterface()
	: gamesFilename("Stars_games.json"),
	  settingsFilename("Stars_settings.json"),
	  record(gamesFilename, settingsFilename) {
	generate(8, 8, 4);
	analyse->maxcaltime = record.getOtherSettings("maxcaltime").asInt();
}

BoardInterface::BoardInterface(BoardAnalyse& hb)
	: gamesFilename("Stars_games.json"),
	  settingsFilename("Stars_settings.json"),
	  record(gamesFilename, settingsFilename) {
	analyse             = new BoardAnalyse(hb);
	analyse->maxcaltime = record.getOtherSettings("maxcaltime").asInt();
}

BoardInterface::~BoardInterface() {
	record.changeOtherSettings("maxcaltime", analyse->maxcaltime);		
	delete analyse;
}

bool BoardInterface::getStateFromInput() {
	short          rows = analyse->getRows(), cols = analyse->getCols();
	vector<string> input;
	string         inputEater;
	analyse->state.printHead();

	for (int i = 0; i < rows; ++i) {
		getline(cin, inputEater);
		if (inputEater == "e" || inputEater == "exit")
			return false;
		if (inputEater.size() != cols * 2 + 1) {
#ifndef STARS_LANG_CHINESE
			cout << "Something wrong going on with the input board\n";
#else
			cout << "输入的棋盘出了点问题\n";
#endif
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

bool BoardInterface::transformInput(
	char** dest, vector<string>& src, const int cols, const int rows) {
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

// mode = "reverse"
string BoardInterface::getInput() {
	while (true) {
#ifndef STARS_LANG_CHINESE
		printf("\nIn reverse mode\nEnter move> ");
#else
		printf("\n撤回模式\n输入行数> ");
#endif // STARS_LANG_CHINESE
		char input[INTER_MAX_INPUT];
		cin.getline(input, INTER_MAX_INPUT);

		if (input[0] == '\0' || !strcmp(input, "e") || !strcmp(input, "exit"))
			return "exit";
		else if (!strcmp(input, "quit") || !strcmp(input, "q"))
			return "quit";
		else if (!strcmp(input, "S") || !strcmp(input, "show"))
			analyse->show();
		else if (!strcmp(input, "h") || !strcmp(input, "help"))
			cout << getHelp("reverse") << endl;
		else if (
			!strcmp(input, "i") || !strcmp(input, "info") ||
			!strcmp(input, "story") || !strcmp(input, "t") ||
			!strcmp(input, "tips"))
			cout << getInfo(input) << endl;
		else if (!strcmp(input, "m") || !strcmp(input, "move"))
#ifndef STARS_LANG_CHINESE
			printf(
				"Why don't we try this in debug mode to see what it does?\n");
#else
			printf("你可以在人机模式里输入这个指令看看会发生什么\n");
#endif // STARS_LANG_CHINESE
		else {
			if (reverseStringConvert(input))
				return input;
#ifndef STARS_LANG_CHINESE
			cout << "Invalid reverse mode number input, let\'s try again\n";
#else
			cout << "这个输入有点小问题\n";
#endif
		}
	}
}

// mode = "debug" or "play"
string
BoardInterface::getInput(char plr, double& inputTime, const string& mode) {
	int                      counter = 0;
	system_clock::time_point start;
	system_clock::time_point end;
	string                   getter;
	while (true) {
		if (counter > 32)
			cin.clear();
#ifndef STARS_LANG_CHINESE
		cout << "\nIn " << mode << " mode\nPlayer '" << plr << "' move> ";
#else
		cout << "\n" << toChinese(mode) << "模式\n玩家 '" << plr << "' > ";
#endif // STARS_LANG_CHINESE
		char input[INTER_MAX_INPUT];
		start = system_clock::now();
		cin.getline(input, INTER_MAX_INPUT);
		end          = system_clock::now();
		auto elapsed = duration_cast<milliseconds>(end - start);
		inputTime    = elapsed.count();
		int num      = -1;
		try {
			num = ToInt::myStoi(input);
		}
		catch (const std::invalid_argument&) {
		}
		if (!strcmp(input, "e") || !strcmp(input, "exit"))
			return "exit";
		else if (analyse->state.colCanAdd(num) && input[0] != '0')
			return input;
		// excute
		else if (input[0] == '\0') {
			++counter;
#ifndef STARS_LANG_CHINESE
			cout << "Invalid empty input, let\'s try again\n";
#else
			cout << "输入无效，请重试\n";
#endif // STARS_LANG_CHINESE
		}
		else if (!strcmp(input, "C") || !strcmp(input, "custom"))
			return "custom";
		else if (!strcmp(input, "d") || !strcmp(input, "debug"))
			return "debug";
		else if (!strcmp(input, "H") || !strcmp(input, "hint"))
			return input;
		else if (!strcmp(input, "h") || !strcmp(input, "help")) {
			getter = getHelp("debug");
			if (getter != "quit")
				cout << getter << endl;
			else
				return getter;
		}
		else if (
			!strcmp(input, "i") || !strcmp(input, "info") ||
			!strcmp(input, "story") || !strcmp(input, "t") ||
			!strcmp(input, "tips") || !strcmp(input, "a song, please")) {
			getter = getInfo(input);
			if (getter != "quit")
				cout << getter << endl;
			else
				return getter;
		}
		else if (
			!strcmp(input, "m") || !strcmp(input, "move") ||
			!strcmp(input, "I") || !strcmp(input, "import") ||
			!strcmp(input, "c") || !strcmp(input, "change"))
			return input;
		else if (
			!strcmp(input, "P") || !strcmp(input, "playback") ||
			!strcmp(input, "playback"))
			return "playback";
		else if (
			!strcmp(input, "p") || !strcmp(input, "play") ||
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
		else if (
			!strcmp(input, "s") || !strcmp(input, "setting") ||
			!strcmp(input, "settings"))
			return "settings";
		else if (
			!strcmp(input, "sa") || !strcmp(input, "st") ||
			!strcmp(input, "show stars"))
			analyse->starShow();
		else if (!strcmp(input, "sv") || !strcmp(input, "save"))
			record.saveGame(analyse->state);
		else if (!strcmp(input, "sr") || !strcmp(input, "show routes")) {
			analyse->routes.crnt = analyse->routes.head;
			if (showRoutesMode() == "quit")
				return "quit";
		}
		else if (!strcmp(input, "w") || !strcmp(input, "winn"))
			cout << "winn = " << analyse->state.winn << endl;
		else if (addStringConvert(input)) {
			add(input);
		}
		else if (reverseStringConvert(input)) {
			reverse(input);
		}
		else
#ifndef STARS_LANG_CHINESE
			cout << "No worries, Let\'s try again?\n";
#else
			cout << "来咱再试一次\n";
#endif
	}
}

short BoardInterface::getCustomInput(const string item) {
	char  input[16];
	short customNumber;
	while (true) {
#ifndef STARS_LANG_CHINESE
		cout << item << " (2~" << SHORTV_LENGTH - 1 << ") = ";
#else
		cout << toChinese(item) << " (2~" << SHORTV_LENGTH - 1 << ") = ";
#endif // STARS_LANG_CHINESE
		cin.getline(input, 16);
		if (!strcmp(input, "e") || !strcmp(input, "exit"))
			return 'e';
		if (!strcmp(input, "q") || !strcmp(input, "quit"))
			return 'q';
		try {
			customNumber = ToInt::myStoi(input);
		}
		catch (const std::invalid_argument&) {
#ifndef STARS_LANG_CHINESE
			cout << "Let's try again\n";
#else
			cout << "来咱再试一次\n";
#endif
			continue;
		}

		if (customNumber > 1 && customNumber < SHORTV_LENGTH - 1)
			return customNumber;
#ifndef STARS_LANG_CHINESE
		cout << "Let's try again\n";
#else
		cout << "来咱再试一次\n";
#endif
	}
}

void BoardInterface::add(string input) {
	oneMove move;
	move.mode = "add";
	if (!addStringConvert(input, move))
		throw logic_error("add: input is in function, yet still invalid");
	analyse->go(move.player, move.move);
	analyse->show();
	record.push_back(move);
#ifndef STARS_LANG_CHINESE
	printf(
		"Player '%c' is added to '%d' as you like it:\n", move.player,
		move.move);
#else
	printf("棋子 '%c' 已被如愿添加到 '%d' 列:\n", move.player, move.move);
#endif // STARS_LANG_CHINESE
}

// return false if wrong input
bool BoardInterface::addStringConvert(string input) {
	oneMove move;
	return addStringConvert(input, move);
}

// return false if wrong input
bool BoardInterface::addStringConvert(string input, oneMove& move) {
	// "012" or "aX13"
	if (input.size() < 2)
		return false;
	char header = input[0];
	if (header == 'x')
		header = 'X';
	if (input[1] == 'x')
		input[1] = 'X';
	if (header == 'a' && (input[1] == 'X' || input[1] == '0')) {
		header = input[1];
		input.erase(input.begin());
	}
	else if (header != 'X' && header != '0')
		return false;
	move.player = header;
	input.erase(input.begin());
	try {
		move.move = ToInt::myStoi(input);
	}
	catch (const std::invalid_argument&) {
		return false;
	}
	if (analyse->state.colCanAdd(move.move))
		return true;
	return false;
}

string BoardInterface::reverseMode() {
	string input;
	while (true) {
		input = getInput();
		if (input == "exit")
			return "last";
		if (input == "quit")
			return input;
		reverse(input);
	}
}

void BoardInterface::reverse(string input) {
	oneMove move;
	move.mode = "reverse";
	if (!reverseStringConvert(input, move))
		throw logic_error("reverse: input is in function, yet still invalid");
	move.player = analyse->state.getTopPiece(move.move);
	analyse->reverse(move.move);
#ifndef STARS_LANG_CHINESE
	printf("Remove %d as you like it:\n", move.move);
#else
	printf("%d 列被撤回:\n", move.move);
#endif // STARS_LANG_CHINESE
	record.push_back(move);
	analyse->show();
}

// return false if wrong input
bool BoardInterface::reverseStringConvert(string input) {
	oneMove dis;
	return reverseStringConvert(input, dis);
}

// return false if wrong input
bool BoardInterface::reverseStringConvert(string input, oneMove& move) {
	// "12" or "r12"
	if (input.empty())
		return false;
	if (input[0] == 'r') {
		input.erase(input.begin());
	}
	try {
		move.move = ToInt::myStoi(input);
	}
	catch (const std::invalid_argument&) {
		return false;
	}
	if (analyse->state.colCanRemove(move.move))
		return true;
	return false;
}

string BoardInterface::debugMode(const string& mode) {
	// init
	string input;
	byPlayer.mode       = mode;
	byPlayer.byComputer = false;
	byOpponent.mode     = mode;
	byOpponent.player   = analyse->rPlayer(byPlayer.player);
	analyse->show();
#ifndef STARS_LANG_CHINESE
	if (mode == "debug") {
		printf("We are in debug mode\n");
		byOpponent.byComputer = !byPlayer.byComputer;
	}
	else {
		printf("We are in play mode\n");
		byOpponent.byComputer = false;
	}
#else
	if (mode == "debug") {
		byOpponent.byComputer = !byPlayer.byComputer;
	}
	else {
		byOpponent.byComputer = false;
	}
#endif // STARS_LANG_CHINESE

	// main loop
	// get input -> process input -> plr move -> opp cal respond -> opp move
	while (true) {
		input = getInput(byPlayer.player, byPlayer.time, mode);
		// process input
		if (input == "exit" || input == "quit") {
#ifdef STARS_DEBUG_INFO
#	ifndef STARS_LANG_CHINESE
			cout << "Exit from " << mode << " mode ...\n";
#	else
			cout << "退出" << toChinese(mode) << "模式 ...\n";
#	endif // STARS_LANG_CHINESE
#endif     // STARS_DEBUG_INFO
			return input;
		}
		if (input == "reverse" || input == "settings" || input == "custom" ||
			input == "play" || input == "playback" || input == "debug")
			return input;
		if (input == "c" || input == "change") {
			// change player
			std::swap(byPlayer.player, byOpponent.player);
			std::swap(byPlayer.word, byOpponent.word);
			std::swap(byPlayer.list, byOpponent.list);
			std::swap(byPlayer.suggestion, byOpponent.suggestion);

			// print hint
			if (byPlayer.mode == "debug" &&
				record.getDefaultSettings("inDebugMode", "hintOn")) {
#ifndef STARS_LANG_CHINESE
				printf(
					"Here was %c's word, list, and suggestion:\n",
					byPlayer.player);
				cout << "word = " << byPlayer.word << "\nlist = [ ";
#else
				printf(
					"另一个玩家 %c 的状态，建议列表和建议如下:\n",
					byPlayer.player);
				cout << "玩家状态 = " << toChinese(byPlayer.word)
					 << "\n建议列表 = [ ";
#endif // STARS_LANG_CHINESE
				for (short move : byPlayer.list)
					printf("%d ", move);
				printf("]\n");
			}
			continue;
		}
		else if (input == "I" || input == "import") {
			if (record.getDefaultSettings("changeBoard", "askToSaveBoard"))
				askToSaveBoard(record.getDefaultSettings(
					"changeBoard", "defaultSaveBoard"));
			else if (record.getDefaultSettings(
						 "changeBoard", "defaultSaveBoard"))
				record.saveGame(analyse->state);
			// new board
			importNewBoard();
			continue;
		}
		else if (input == "H" || input == "hint") {
#ifndef STARS_LANG_CHINESE
			if (byPlayer.mode != "debug" || byPlayer.mode != "play") {
				printf(
					"The last move is not in debug or play mode, therefore "
					"won't display hint\n");
				continue;
			}
			printf("this is the hint for the previous move:\n");
			cout << "word = " << byPlayer.word << "\nlist = [ ";
#else
			if (byPlayer.mode != "debug" || byPlayer.mode != "play") {
				printf("上一步不在人机或双人模式，所以提示不显示\n");
				continue;
			}
			printf("上一步的提示：\n");
			cout << "玩家状态 = " << toChinese(byPlayer.word)
				 << "\n建议列表 = [ ";
#endif // STARS_LANG_CHINESE
			for (short i : byPlayer.list)
				printf("%d ", i);
			printf("]\n");
			continue;
		}
		else if (input == "m" || input == "move") {
			if (mode == "play") {
				cout << "You can't force your friend to play right?\n";
				continue;
			}
			if (isOver(byPlayer, mode)) {
#ifdef STARS_DEBUG_INFO
#	ifndef STARS_LANG_CHINESE
				cout << "Exit from " << mode << " mode ...\n";
#	else
				cout << "退出" << toChinese(mode) << "模式 ...\n";
#	endif // STARS_LANG_CHINESE
#endif     // STARS_DEBUG_INFO
				return "over";
			}
		}
		else {
			// player goes
#ifndef STARS_DEBUG_INFO
			byPlayer.move = ToInt::myStoi(input);
#else
			try {
				byPlayer.move = ToInt::myStoi(input);
			}
			catch (const std::invalid_argument& e) {
				std::cerr << e.what() << '\n';
				throw logic_error("in-fuction invalid argument!?");
			}
#endif // STARS_DEBUG_INFO
			analyse->go(byPlayer.player, byPlayer.move);
			record.push_back(byPlayer); // byPlayer end here
			if (isOver(byPlayer, mode)) {
#ifdef STARS_DEBUG_INFO
#	ifndef STARS_LANG_CHINESE
				cout << "Exit from " << mode << " mode ...\n";
#	else
				cout << "退出" << toChinese(mode) << "模式 ...\n";
#	endif // STARS_LANG_CHINESE
#endif     // STARS_DEBUG_INFO
				return "over";
			}
		}

		if (mode == "play") {
			// don't move and change the player
			std::swap(byPlayer.player, byOpponent.player);
			std::swap(byPlayer.word, byOpponent.word);
			std::swap(byPlayer.list, byOpponent.list);
			std::swap(byPlayer.suggestion, byOpponent.suggestion);
		}
#ifndef STARS_LANG_CHINESE
		else {
			// opp respond
			byOpponent.suggestion = respond();
			// opp move
			byOpponent.move = byOpponent.suggestion;
			analyse->go(byOpponent.player, byOpponent.move);
			record.push_back(byOpponent);
			if (record.getDefaultSettings("inDebugMode", "showTime"))
				cout << "    input time used: " << byPlayer.time << " ms\n";
			printf("    %c goes here %d\n", byOpponent.player, byOpponent.move);
			if (isOver(byOpponent, mode)) {
#	ifdef STARS_DEBUG_INFO
				cout << "Exit from " << mode << " mode ...\n";
#	endif // STARS_DEBUG_INFO
				return "over";
			}
		}

		// print hint
		byPlayer.hintOn = record.getDefaultSettings("inDebugMode", "hintOn");
		if (byPlayer.hintOn)
			cout << "\nHint provided for player " << byPlayer.player << ":\n";
#else
		else {
			// opp respond
			byOpponent.suggestion = respond();
			// opp move
			byOpponent.move = byOpponent.suggestion;
			analyse->go(byOpponent.player, byOpponent.move);
			record.push_back(byOpponent);
			if (record.getDefaultSettings("inDebugMode", "showTime"))
				cout << "    输入用时：" << byPlayer.time << " 毫秒\n";
			printf(
				"    %c 走了这一步：%d\n", byOpponent.player, byOpponent.move);
			if (isOver(byOpponent, mode)) {
#	ifdef STARS_DEBUG_INFO
				cout << "退出" << toChinese(mode) << "模式 ...\n";
#	endif // STARS_DEBUG_INFO
				return "over";
			}
		}

		byPlayer.hintOn = record.getDefaultSettings("inDebugMode", "hintOn");
		if (byPlayer.hintOn)
			cout << "\n给玩家" << byPlayer.player << "的提示信息:\n";
#endif     // STARS_LANG_CHINESE

		// recommend
		byPlayer.suggestion = analyse->respond(
			byPlayer.player, byPlayer, byPlayer.hintOn,
			record.getDefaultSettings("inDebugMode", "showTime"),
			record.getDefaultSettings("inDebugMode", "starsOn"), false);
		if (mode == "debug")
			showComment(byPlayer);
		if (byPlayer.hintOn && byPlayer.word != "bad")
#ifndef STARS_LANG_CHINESE
			printf(
				"    %d is recommended for player %c\n", byPlayer.suggestion,
				byPlayer.player);
#else
			printf(
				"    推荐 %c 走这一步：%d\n", byPlayer.player,
				byPlayer.suggestion);
#endif // STARS_LANG_CHINESE

		// show
		printf("\n");
		if (record.getDefaultSettings("inDebugMode", "starrySky"))
			analyse->starShow();
		else
			analyse->show();
		printf("\n");
	}
#ifdef STARS_DEBUG_INFO
	throw logic_error("control flow into the end of debug mode");
#endif
}

short BoardInterface::respond() {
#ifndef STARS_LANG_CHINESE
	printf("Info for player %c:\n", byOpponent.player);
#else
	printf("玩家 %c 信息：\n", byOpponent.player);
#endif // STARS_LANG_CHINESE
	byOpponent.suggestion = analyse->respond(
		byOpponent.player, byOpponent,
		record.getDefaultSettings("inDebugMode", "showCalculate"),
		record.getDefaultSettings("inDebugMode", "showTime"),
		record.getDefaultSettings("inDebugMode", "starsOn"),
		record.getDefaultSettings("inDebugMode", "trackRoutes"));
#ifdef STARS_DEBUG_INFO
	if (!byOpponent.list.empty() &&
		!MyShortList::inList(byOpponent.list, byOpponent.suggestion))
		throw logic_error("suggestion not in safe list");
#endif // STARS_DEBUG_INFO
	return byOpponent.suggestion;
}

string BoardInterface::defaultSettings() {
#ifndef STARS_LANG_CHINESE
	cout << "We have " << record.getDefaultSettingsItemNum()
		 << " situations , inPlayMode is the same with inDebugMode. Within "
			"each of\n"
		 << "them we have multiple items each marked with tags below, type in "
			"tags to change\n"
		 << "these settings, e to exit, S/show "
		 << "to show current settings, h/help for help\n";
	// show
	record.showSettingsWithTags();
	cout << "\nCare to change one of them?\n";
#else
	cout << "一共有" << record.getDefaultSettingsItemNum() << "种情况，其中"
		 << "每个情况有多个项目，各个项目均有两个字母为标记，输入标记\n"
		 << "来改变相应的设定值，输入e退出，输入S或show显示当前设置，输入h获得帮"
			"助\n";
	cout << "双人模式和普通模式设定相同\n";
	// show
	record.showSettingsWithTags();
#endif // STARS_LANG_CHINESE
	while (true) {
		printf("> ");
		char input[8];
		cin.getline(input, 8);
		if (input[0] == '\0' || !strcmp(input, "e") || !strcmp(input, "exit") ||
			!strcmp(input, "no")) {
			record.saveSettings();
			return "last";
		}
		if (!strcmp(input, "q") || !strcmp(input, "quit")) {
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
		}
		else {
			cout << "Invalid input, let's try again.\n";
			cout << "And if you don't know what's going on, you can always "
					"type help, happy to help as always.\n";
		}
	}
#ifdef STARS_DEBUG_INFO
	throw logic_error("control flow into the end of settings mode");
	return "quit";
#endif
}

string BoardInterface::otherSettings() {
	if (analyse->maxcaltime < 0)
		printf("analyse->maxcaltime went nuts, better fix it\n");
#ifndef STARS_LANG_CHINESE
	cout << "Here you can change the program's max calculation time settings, "
			"the\n"
		 << "current cal time is " << analyse->maxcaltime << "ms\n"
		 << "Enter nothing or e to exit, q to quit\n";
#else
	cout << "你可以在此处改变程序的最大计算时间，当前最大计算时间是"
		 << analyse->maxcaltime << "毫秒\n直接回车或输入e返回，q退出游戏\n";
#endif // STARS_LANG_CHINESE
	char in[16];
	int  trans = 0;
	do {
		printf("(10ms ~ 9999999ms)> ");
		cin.getline(in, 16);
		if (in[0] == '\0' || !strcmp(in, "e") || !strcmp(in, "exit"))
			return "last";
		if (!strcmp(in, "q") || !strcmp(in, "quit"))
			return "quit";
		try {
			trans = ToInt::myStoi(in);
		}
		catch (const invalid_argument&) {
			cout << "let's try again\n";
			continue;
		}
		if (trans > 9 && trans < 9999999) {
			analyse->maxcaltime = trans;
			return "last";
		}
		printf("let's try again\n");
	} while (true);
}

string BoardInterface::settingsMode() {
#ifndef STARS_LANG_CHINESE
	printf("We are in settings mode now:\n");
	char input[8];
	printf("1. default settings\n2. other settings\n");
	do {
		printf("(Enter number)> ");
#else
	printf("设置模式\n");
	char input[8];
	printf("1. 默认设置\n2. 其他设置\n");
	do {
		printf("(输入数字)> ");
#endif // STARS_LANG_CHINESE
		cin.getline(input, 8);
		if (input[0] == '\0' || !strcmp(input, "e") || !strcmp(input, "exit"))
			return "last";
		if (!strcmp(input, "q") || !strcmp(input, "quit"))
			return "quit";
		if (!strcmp(input, "1"))
			return defaultSettings();
		if (!strcmp(input, "2"))
			return otherSettings();
		else
#ifndef STARS_LANG_CHINESE
			printf("let's try again\n");
	} while (true);
#else
			printf("我们来再试一次\n");
	} while (true);
#endif // STARS_LANG_CHINESE
}

string BoardInterface::playbackMode() {
#ifndef STARS_LANG_CHINESE
	if (!record.getNumberOfSavedBoard()) {
		printf("No games has been recorded yet. If you want to try this mode,");
		cout << " save a game first\nTill next time.\n";
#	ifdef STARS_DEBUG_INFO
		printf("Exit from playback mode ...\n");
#	endif // STARS_DEBUG_INFO
		return "last";
	}
	printf("We are here in playback mode.\n");
	printf(
		"You can choose from %d saved games.\n",
		record.getNumberOfSavedBoard());
	printf(
		"Hit 'Enter' to see more, type in index number to went into one of "
		"those games.\n");
	printf(
		"Or type c to continue to play the board that is currently "
		"displayed.\n");
	printf("Use d/rm/delete/remove to delete the current board, e to exit.\n");
	printf("Explore!\n");
	Json::Value game;
	string      result = record.showSavedGames(game);
	if (result == "exit") {
		printf("Exit from playback mode ...\n");
		return "last";
	}
	if (result == "quit") {
		printf("Exit from playback mode ...\n");
		return "quit";
	}
#else
	if (!record.getNumberOfSavedBoard()) {
		printf("还没有任何存档，如果你想使用此模式，请先保存游戏");
#	ifdef STARS_DEBUG_INFO
		printf("退出读档回放模式 ...\n");
#	endif // STARS_DEBUG_INFO
		return "last";
	}
	printf("读档回放模式\n");
	printf("共 %d 份存档\n", record.getNumberOfSavedBoard());
	printf(
		"回车浏览下一个，输入索引号直接读取该存档，输入c读取当前显示的存档\n");
	printf("使用 d/rm/delete/remove 删除当前存档，e退出\n");
	printf("探索愉快!\n");
	Json::Value game;
	string result = record.showSavedGames(game);
	if (result == "exit") {
#	ifdef STARS_DEBUG_INFO
		printf("退出读档回放模式 ...\n");
#	endif // STARS_DEBUG_INFO
		return "last";
	}
	if (result == "quit") {
#	ifdef STARS_DEBUG_INFO
		printf("退出读档回放模式 ...\n");
#	endif // STARS_DEBUG_INFO
		return "quit";
	}
#endif     // STARS_LANG_CHINESE

	// set up a new game and playback the moves
	BoardState  state = game["state"];
	BoardRecord newRecord, tempRecord;
	newRecord.refreshHistoryMove(game["historyMove"]);
	// return to the initial state
	state.retInit(newRecord.historyMove);
	BoardAnalyse analyser(state);
#ifndef STARS_LANG_CHINESE
	printf("Here's the playback of an old game:\n");
	printf(
		"Use c to cut in and play, h for help, b to go back to the previous "
		"step, e\nto exit.\n");
#else
	printf("这是存档游戏的回放：\n");
	printf("使用c打断回放并进入游戏模式，b回到上一步，h帮助，e退出\n");
#endif // STARS_LANG_CHINESE
	char                      input[16];
	bool                      wentBack      = false;
	char                      reversePlayer = ' ';
	vector<oneMove>::iterator iter          = newRecord.historyMove.begin();
	while (iter != newRecord.historyMove.end()) {
		// show
#ifndef STARS_LANG_CHINESE
		printf(
			"This is step %zu/%zu\n", tempRecord.historyMove.size() + 1,
			newRecord.historyMove.size());
#else
		printf(
			"此步/总步数 %zu/%zu\n", tempRecord.historyMove.size() + 1,
			newRecord.historyMove.size());
#endif // STARS_LANG_CHINESE
		if (wentBack)
			wentBack = false;
		else {
			cout << *iter;
			tempRecord.push_back(*iter);
			if (iter->mode != "reverse")
				analyser.go(iter->player, iter->move);
			else {
				reversePlayer = analyser.state.getTopPiece(iter->move);
				analyser.reverse(iter->move);
			}
		}
		analyser.show();

		// get input
		printf("> ");
		cin.getline(input, 16);
		if (!strcmp(input, "e") || !strcmp(input, "exit"))
			break;
		if (!strcmp(input, "q") || !strcmp(input, "quit"))
			return "quit";
		else if (
			!strcmp(input, "c") || !strcmp(input, "cut") ||
			!strcmp(input, "cut in")) {
			BoardAnalyse   tempAnalyser = analyser;
			BoardInterface interface(tempAnalyser);
			interface.refreshRecord(tempRecord);
#ifndef STARS_LANG_CHINESE
			printf("Go into a new control mode:\n");
			if (!interface.controlMode())
				return "quit";
			printf("We're back in playback mode again\n");
#else
			printf("进入新的游戏模式\n");
			if (!interface.controlMode())
				return "quit";
			printf("回到读档回放模式\n");
#endif // STARS_LANG_CHINESE
		}
		else if (
			!strcmp(input, "b") || !strcmp(input, "back") ||
			!strcmp(input, "go back")) {
			if (tempRecord.historyMove.empty())
#ifndef STARS_LANG_CHINESE
				printf("This is the beginning, there is no going back.\n");
#else
				printf("已经回到开头，没有上一步了\n");
#endif // STARS_LANG_CHINESE
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
		else if (!strcmp(input, "h") || !strcmp(input, "help")) {
#ifndef STARS_LANG_CHINESE
			printf(
				"b to go back to the previous move, c to cut in and play, q to "
				"quit\n");
			printf("the whole game, e to exit this mode\n");
#else
			printf("使用c打断回放并进入游戏模式，b回到上一步，h帮助，e退出\n");
#endif // STARS_LANG_CHINESE
		}
		++iter;
	}
#ifdef STARS_DEBUG_INFO
#	ifndef STARS_LANG_CHINESE
	printf("Exit from playback mode ...\n");
#	else
	printf("退出读档回放模式 ...\n");
#	endif // STARS_LANG_CHINESE
#endif     // STARS_DEBUG_INFO
	return "last";
}

string BoardInterface::customMode() {
#ifndef STARS_LANG_CHINESE
	printf("In custom mode ...\n");
	printf(
		"Note that the more width you set, the clumsier the program will be\n");
	printf("It is recommended to set the width lesser than 16\n");
#else
	printf("自定义模式 ...\n");
	printf(
		"注意设置的宽度越大，程序的计算量就越大，可能会导致程序变慢，不推荐\n");
	printf("将宽度设置到16以上\n");
#endif // STARS_LANG_CHINESE
	if (record.getDefaultSettings("inCustomMode", "askToSaveBoard"))
		askToSaveBoard(
			record.getDefaultSettings("inCustomMode", "defaultSaveBoard"));
	else if (record.getDefaultSettings("inCustomMode", "defaultSaveBoard"))
		record.saveGame(analyse->state);
	short column, row, winn;
	column = getCustomInput("board width");
	if (column == 'e')
		return "last";
	if (column == 'q')
		return "quit";
	row = getCustomInput("board height");
	if (row == 'e')
		return "last";
	if (row == 'q')
		return "quit";
	winn = getCustomInput("win number");
	if (winn == 'e')
		return "last";
	if (winn == 'q')
		return "quit";
	analyse->customBoard(column, row, winn);
	record.clearHistoryMove();
#ifdef STARS_DEBUG_INFO
#	ifndef STARS_LANG_CHINESE
	cout << "Exit from custom mode ...\n";
#	else
	cout << "退出自定义模式 ...\n";
#	endif // STARS_LANG_CHINESE
#endif     // STARS_DEBUG_INFO
	return "last";
}

bool BoardInterface::controlMode(const string& firstMode) {
	byPlayer.player = 'X';
	string lastPlayerMode, advice = firstMode;
	if (advice.empty())
		advice = "debug";
	if (advice == "debug" || advice == "play")
		lastPlayerMode = advice;
	else
		lastPlayerMode = "debug";
	int i = 0;
	while (i < 100) {
		if (advice == "quit" || advice == "exit")
			break;
		else if (advice == "reverse")
			advice = reverseMode();
		else if (advice == "debug" || advice == "play")
			advice = debugMode(advice);
		else if (advice == "settings")
			advice = settingsMode();
		else if (advice == "playback")
			advice = playbackMode();
		else if (advice == "custom")
			advice = customMode();
		else if (advice == "over") {
			if (record.getDefaultSettings("gameIsOver", "askToReverse") &&
				askToReverse(record.getDefaultSettings(
					"gameIsOver", "defaultReverse"))) {
				advice = reverseMode();
				continue;
			}
			else if (record.getDefaultSettings(
						 "gameIsOver", "defaultReverse")) {
				advice = reverseMode();
				continue;
			}
			if (record.getDefaultSettings("gameIsOver", "askToSaveBoard"))
				askToSaveBoard(record.getDefaultSettings(
					"gameIsOver", "defaultSaveBoard"));
			else if (record.getDefaultSettings(
						 "gameIsOver", "defaultSaveBoard"))
				record.saveGame(analyse->state);
			break;
		}

		// set last
		if (advice == "last") {
			advice = lastPlayerMode;
		}
		else if (advice == "debug" || advice == "play")
			lastPlayerMode = advice;
		++i;
	}
	if (!record.match())
		throw runtime_error(
			"current settings var in BoardRecord doesn't match with the "
			"default settings");
	if (i == 100)
		throw runtime_error("too much unhandled advice");
	if (advice == "quit")
		return false;
#ifdef STARS_DEBUG_INFO
#	ifndef STARS_LANG_CHINESE
	cout << "Exit from controller ...\n";
#	else
	cout << "退出控制模式 ...\n";
#	endif // STARS_LANG_CHINESE
#endif     // STARS_DEBUG_INFO
	return true;
}

string BoardInterface::showRoutesMode() {
	if (!record.getDefaultSettings("inDebugMode", "trackRoutes")) {
#ifndef STARS_LANG_CHINESE
		cout << "trackRoutes is currently off, turn it on in settings mode to "
				"track and show\n"
			 << "routes\n";
#else
		cout << "追踪路线已关闭，可以在设置模式里将其打开\n";
#endif // STARS_LANG_CHINESE
		return "last";
	}
	RouteTree&          routes        = analyse->routes;
	vector<RouteNode*>& next          = routes.crnt->next;
	bool                nextFlag      = false;
	long long           routeBranches = routes.getBranches();
#ifndef STARS_LANG_CHINESE
	cout << "The program examined " << routeBranches
		 << " possibilities in this node\n"
		 << routes.getBranches(-2) << " of them is free, "
		 << routes.getBranches(-1) << " of them is good and "
		 << routes.getBranches(0) << " of them is bad for the\nprogram\n";
	cout << "We'll be showing:\na. free routes\nb. good routes\nc. bad routes\n"
		 << "d. all routes\n";
	if (next.size() == 1 && next[0]->next.empty() &&
		(next[0]->data == routes.goodNode || next[0]->data == routes.badNode ||
		 next[0]->data == routes.freeNode))
		nextFlag = true; // next is a flag
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
	string tryAgain = "No such move, let's try again";
#else
	cout << "程序为自己检验了 " << routeBranches << "种路径，其中有 "
		 << routes.getBranches(-2) << " 个自由 " << routes.getBranches(-1)
		 << " 个好和 " << routes.getBranches(0) << " 个坏\n";
	cout << "通过输入相应字母，本程序将展示：\na. 自由的路径\nb. 好的路径\nc. "
		 << "坏的路径\nd. 所有路径\n";
	if (next.size() == 1 && next[0]->next.empty() &&
		(next[0]->data == routes.goodNode || next[0]->data == routes.badNode ||
		 next[0]->data == routes.freeNode))
		nextFlag = true; // next is a flag
	else {
		cout << "或者输入相应数字选择进入如下路径，查看该路径下的种种可能\n[ ";
		vRi iter = next.begin();
		for (; iter != next.end(); ++iter)
			cout << (*iter)->data << " ";
		cout << "]\n";
	}
	if (routes.crnt->prev)
		cout << "输入B或back返回上一个路径\n";
	string in;
	string tryAgain = "没有这个选项，我们再试一次";
#endif // STARS_LANG_CHINESE
	int num = 0;
	do {
		cout << "> ";
		getline(cin, in);
		if (in == "q" || in == "quit")
			return "quit";
		if (in == "e" || in == "exit" || in.empty())
			return "last";
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
#ifndef STARS_LANG_CHINESE
			cout << "There's no going back, let's try again.\n";
#else
			cout << "没有回头路了，我们再试一次\n";
#endif // STARS_LANG_CHINESE
			continue;
		}
		else if (!nextFlag) {
			try {
				num = ToInt::myStoi(in);
			}
			catch (const invalid_argument& e) {
				cout << "Invalid_argument: " << e.what() << endl;
				cout << tryAgain << endl;
				continue;
			}
			// find_if by me
			vRi iter = next.begin();
			for (; iter != next.end() && (*iter)->data != num; ++iter)
				;
			if (iter == next.end()) {
				cout << tryAgain << endl;
				continue;
			}
			routes.forward(num);
			// 老递归怪了
			return showRoutesMode();
		}
		else
			cout << tryAgain << endl;
	} while (true);


	// print
	routeBranches = routes.getBranches(num - 3);
	if (routeBranches > 1024) {
#ifndef STARS_LANG_CHINESE
		cout << "There are " << routeBranches
			 << " branches, each branch will have it's own line, sure\nyou "
				"want to print them all?\n"
			 << "(Yes/no)> ";
#else
		cout << "一共有 " << routeBranches << " 种路径，每个路径占有一行，确定要全部输出？\n"
			 << "(Yes/no)> ";
#endif // STARS_LANG_CHINESE
		getline(cin, in);
		if (in != "no" && in != "n")
			routes.showRoute(num - 3);
	}
	else
		routes.showRoute(num - 3);
	return "last";
}

string BoardInterface::getHelp(string mode) {
	string head = "";
#ifndef STARS_LANG_CHINESE
	string enjoy = "Enjoy!\n";
	string end =
		"----------------------------------------------------------------------"
		"---------\n";
	string enterForMore =
		"------------------------- 'Enter' for more, e to exit "
		"------------------------\n";
	string reverse =
		head +
		"\nType in column number to reverse that action. Hit 'Enter' to "
		"exit\n" +
		"\nIf you don't know what's going on, why are you here. Why don't you "
		"exit from here\n" +
		"and see what the help information in debug mode have to say?\n" +
		enjoy + end;
	vector<string> debug = {
		head +
			"\nType in number to play, the program will automaticaly respond. "
			"One who place\n" +
			"four piece in a row first wins.\n" +
			"\nUse <piece><column number> to place a piece in a column without "
			"auto-respond.\n"
			"\nUse r<column number> to reverse an action\n\n" +
			"Options\n" + "  e  / exit             exit from a certain mode\n" +
			"  q  / quit             quit the whole game\n" +
			"  C  / custom           custom board height, width and win number (4 "
			"by default)\n" +
			"  h  / help             show help message of the current mode\n" +
			"  p  / play             play mode - play with others\n" +
			"  P  / playback         playback mode - playback saved games\n" +
			"  S  / show             show the current board\n" +
			"  s  / settings         view and change the settings\n" +
			"  st / show stars       show stars\n" +
			"  sv / save             save the current game in file "+gamesFilename+"\n" +
			"  sr / show routes      show routes that the program has examined\n" +
			"  t  / tips             tips I wrote to help other player (you) to "
			"play the game\n" +
			"  w  / winn             show win number (4 by default) in case you "
			"forgot\n" +
			"  i  / info             information about the game\n\n" +
			enterForMore,
		head +
			"If hintOn is true, then when the program says your word = good, "
			"you'll win in a\n" +
			"few steps if you chose to take the step within the list that "
			"follows. However,\n" +
			"if starsOn is true, the program will be faster but might make "
			"mistakes. Use\n" +
			"settings to turn on/off hint\n" + enterForMore,
		head +
			"If word=free, list=[1, 5], it is recommended that you take the "
			"step within the\n" +
			"list.\n" +
			"\nNote that area that's covered by the stars cannot be accessed by "
			"the program,\n" +
			"therefore might contain surprises.\n" + "\nOther options\n" +
			"  c / change     change the player\n" +
			"  H / hint       show hint for the previous step\n"
			"  I / import     import a new board from input, the new board must "
			"fit in with\n" +
			"                 the current board\n" +
			"  m / move       force the program to take a move now\n" +
			"  r / reverse    into reverse mode - reverse some moves\n" + enjoy +
			end};
#else
	string enjoy = "玩的开心!\n";
	string end =
		"----------------------------------------------------------"
		"---------------------\n";
	string enterForMore =
		"-------------------------- '回车'显示余下信息，e返"
		"回 ------------------------";
	string reverse =
		head + "\n输入列数撤回该列最顶端棋子，按回车退出\n\n" +
		"如果你不知道发生了什么，最好回到人机模式看那里的帮助文档\n" + enjoy +
		end;
	vector<string> debug = {
		head +
			"\n输入数字下棋，程序会自动回应，四子连成一条线则获胜，注意棋子只能"
			"从下往上叠起\n\n" +
			"选项               效果\n" +
			"  e  / exit             退出所在模式\n" +
			"  q  / quit             退出游戏\n" +
			"  C  / custom           "
			"自定义棋盘长宽和获胜所需最小成排棋子数（默认4）\n" +
			"  h  / help             显示当前模式帮助文档\n" +
			"  p  / play             双人模式\n" +
			"  P  / playback         读档回放模式 - 回放已有存档\n" +
			"  S  / show             显示当前棋盘\n" +
			"  s  / settings         进入设置模式\n" +
			"  st / show stars       显示星星\n" +
			"  sv / save             将游戏文件存档在" + gamesFilename + "里\n" +
			"  sr / show routes      显示程序计算过的路径\n" +
			"  t  / tips             显示游戏玩法提示\n" +
			"  w  / winn             显示获胜所需最小成排棋子数\n" +
			"  i  / info             显示游戏信息\n\n" + enterForMore,
		head +
			"\n如果打开提示（hintOn）为真，那么当程序显示玩家状态为好时，如果玩"
			"家选择遵循推荐\n" +
			"列表中的下法，那么玩家将在几步之内获胜，反之如果显示玩家状态为坏则"
			"程序将在几步\n" +
			"之内获胜\n\n" +
			"不过如果设置中的晴天（starsOn）为真，那么程序有大概15%"
			"概率计算错误 ;-)\n" +
			enterForMore,
		head +
			"\n当玩家状态显示为自由时，推荐列表中的下法一般可以随便下，推荐列表"
			"之外最好不要下\n" +
			"注意有星星的地方程序是看不到的，所以不会推荐，但这并不说明那儿是好"
			"或者坏只能说\n" +
			"明那儿远\n\n" + "其他选项\n" +
			"  c / change     人机玩家身份互换\n" +
			"  H / hint       显示上一步的提示\n" +
			"  I / import     导入新棋盘，注意新旧棋盘的大小必须相同\n" +
			"  m / move       使程序立刻走一步（不等待玩家输入）\n" +
			"  r / reverse    进入撤回模式\n" + enjoy + end};
#endif // STARS_LANG_CHINESE
	if (mode == "debug") {
		string                   input;
		vector<string>::iterator iter = debug.begin();
		do {
			if (input.empty())
				cout << *iter;
			else if (input == "e" || input == "exit")
				break;
			else if (input == "quit" || input == "q")
				return "quit";
			else {
#ifndef STARS_LANG_CHINESE
				cout << "Pardon?\n> ";
#else
				cout << "嗯?\n> ";
#endif // STARS_LANG_CHINESE
				getline(cin, input);
				continue;
			}
			++iter;
			if (iter == debug.end())
				break;
			getline(cin, input);
		} while (iter != debug.end());
		return head;
	}
	if (mode == "reverse")
		return reverse;
#ifndef STARS_LANG_CHINESE
	if (mode == "settings") {
		string settings =
			head + "\nDefault settings:\n" +
			"Customize the program's behaviour in certain situations\n" +
			"Each situation represent a situation where questions might be "
			"asked, you can\n" +
			"decide whether it will be asked, and dispite whether it IS asked, "
			"the default\n" +
			"answer would be. Notice that, for example, in situation "
			"gameIsOver, if\n" +
			"askToReverse is false but defaultReverse is true, then when game "
			"is over, we will\n" +
			"went into reverse mode immediately.\n" +
			"If starsOn is true, then stars will fall down from the sky then "
			"make sure the\n" +
			"program think fast and reckless.\n"
			"A " +
			settingsFilename +
			" file will be generated if the settings are "
			"changed\n";
		return settings;
	}
#else
	if (mode == "settings") {
		string settings =
			head + "\n默认设定：\n" +
			"设定程序在特定情况下的行为，比如情况：游戏结束 "
			"下的项目：询问撤回如果为真，\n" +
			"程序就会在游戏结束时询问是否进入撤回模式，注意如果询问撤回为假而项"
			"目：默认\n" +
			"撤回为真，则程序会跳过询问直接进入撤回模式\n\n" +
			"如果晴天为真，星星就会落到棋盘上，进而导致程序的计算迅速而鲁莽\n"
			"自定义设置将被储存在" +
			settingsFilename + "里";
		return settings;
	}
#endif // STARS_LANG_CHINESE
	return reverse;
}

string BoardInterface::getInfo(string input) {
	string head = "";
#ifdef STARS_VERSION_DEBUG
	string version = STARS_VERSION_DEBUG;
#else
#	ifdef STARS_VERSION_RELEASE
	string version = STARS_VERSION_RELEASE;
#	endif // STARS_VERSION_RELEASE
#endif     // STARS_VERSION_DEBUG
#ifndef STARS_LANG_CHINESE
	string enjoy = "Enjoy!\n";
	string end =
		"----------------------------------------------------------------------"
		"---------\n";
	string tips =
		head + "\nTips from CharmedPython:\n" +
		"So the trick is to build yourself as high as possible, but don't make "
		"hugh\n" +
		"chunks, leave some room between them. Here are some good examples:\n" +
		" 1 2 3 4 5 6 7 8\n" + "| | | | | | | | |\n" + "| | | | | | | | |\n" +
		"| | | | | |0| | |\n" + "|0| | | | |X| | |\n" + "|X| |X|0| |X| | |\n" +
		"|0| |X|X| |X| | |\n" + "|0| |X|X| |0|X| |\n" + "|0|0|0|X|0|X|0| |\n" +
		"\nor\n" + " 1 2 3 4 5 6 7 8\n" + "| | | | | | | | |\n" +
		"| |0| | | | | | |\n" + "| |0| | | | | | |\n" + "| |X| | |0| | | |\n" +
		"| |X| |0|X| | | |\n" + "| |X| |X|X| | | |\n" + "| |0| |X|X| | | |\n" +
		"|0|0| |X|0|X|0|0|\n" +
		"This type of layout will give you a lot of advantage. Trying to build "
		"any kind\n" +
		"of three piece in a row is helpful.\nGood luck!\n" + end;
	string info = head + "\nA 1v1 & 8x8 command line based board game\n" +
#	ifndef STARS_DEBUG_INFO
				  "\n-------------------------------- version " + version +
				  " --------------------------------\n" +
#	endif
#	ifdef STARS_DEBUG_INFO
				  "\n------------------------------ version " + version +
				  " -----------------------------\n" +
#	endif
				  "                                                            "
				  "      by Duan Hanyu\n" +
				  "                                                            "
				  "          2020-8-28";
#else
	string enjoy = "玩的开心！\n";
	string end =
		"----------------------------------------------------------------------"
		"---------\n";
	string tips =
		head + "从上一个python版游戏复制来的游戏:\n" +
		"这个要诀就是尽量造出连续的，对方不能下的点，可以从盖连续高楼开始但中间"
		"留点空，\n" +
		"别弄成一大堆\n" + " 1 2 3 4 5 6 7 8\n" + "| | | | | | | | |\n" +
		"| | | | | | | | |\n" + "| | | | | |0| | |\n" + "|0| | | | |X| | |\n" +
		"|X| |X|0| |X| | |\n" + "|0| |X|X| |X| | |\n" + "|0| |X|X| |0|X| |\n" +
		"|0|0|0|X|0|X|0| |\n" + "\nor\n" + " 1 2 3 4 5 6 7 8\n" +
		"| | | | | | | | |\n" + "| |0| | | | | | |\n" + "| |0| | | | | | |\n" +
		"| |X| | |0| | | |\n" + "| |X| |0|X| | | |\n" + "| |X| |X|X| | | |\n" +
		"| |0| |X|X| | | |\n" + "|0|0| |X|0|X|0|0|\n" +
		"这样就能提升赢的概率。实际上，所有形式的三个一行都能达到提高获胜概率的"
		"效果" +
		"\n祝好运!\n" + end;
	string info = head + "\n一个一对一，8x8 基于命令行的四子棋游戏\n" +
#	ifndef STARS_DEBUG_INFO
				  "\n------------------------------- version " + version +
				  " ---------------------------------\n" +
#	endif
#	ifdef STARS_DEBUG_INFO
				  "\n---------------------------- version " + version +
				  " -------------------------------\n" +
#	endif
				  "                                                            "
				  "          by 段晗宇\n" +
				  "                                                            "
				  "          2020-8-28";
#endif // STARS_LANG_CHINESE
	vector<string> story = {
		head +
			"Out of a few random try, you finally get yourself an Easter Egg! "
			"But I'm afraid\n" +
			"this is actually wrote for myself, so it's quite boring for other "
			"players. Not\n" +
			"too late to quite now.\n" +
			"------------------------- 'Enter' for more, q to quit "
			"--------------------------\n",
		head +
			"Anyway, I have just finished an interview when I see my "
			"roommate\n" +
			"coding. He told me it was his homework, I got interested. Then I "
			"begin to work\n"
			"on this silly thing till I found that it's taking up all of my "
			"time and I'm not\n" +
			"even satisfied? Like it's a weird addictive activity that does "
			"even provide\n" +
			"satisfaction like another other decent addictive activity will. "
			"Watching Game of\n" +
			"Thrones all day might indicate that someone's life is decaying, "
			"but at least\n" +
			"that \"feels good\". Now I'm just coding for almost no reason at "
			"all, there's\n" +
			"not much technique here, doesn't require any brain work. Brick by "
			"brick, anyone\n" +
			"can build this. I mean, what? I didn't go to school so that I can "
			"sit here and\n" +
			"type all day. And there's always new features to develop and new "
			"bugs to fix.\n" +
			"Now I really get it when my cousin say you can never finish a "
			"project, it's a\n" +
			"disaster.\n" +
			"------------------------- 'Enter' for more, q to quit "
			"--------------------------\n",
		head +
			"But when I say end a project instead of stop coding, I suddenly "
			"realized that it\n" +
			"IS my fault that the project never ends 'cause I never said WHEN "
			"it ends. When?\n" +
			"Do I stop once the program can play at least five moves? Do I "
			"stop once I've\n" +
			"developed a recursive analyse function? Do I stop once playback "
			"feature is\n" +
			"developed? Maybe I should have set a goal, a point, where once it "
			"was met, I\n" +
			"stop. Maybe I should do that whenever I start to do anything?\n" +
			"So... where should I stop?\n" + "\n" +
			"Well, one have to admit, typing that IS refreshing. The developer "
			"is OK now.\n" +
			"Sorry you have to see all these irrelevant crap. Have a nice "
			"day!\n" +
			end};
	if (input == "t" || input == "tips") {
		return tips;
	}
	if (input == "story") {
		return head;
		char  dis[16];
		int   wrongInput = 0;
		short i          = 1;
		cout << story[0] << "> ";
		cin.getline(dis, 16);
		if (!strcmp(dis, "q") || !strcmp(dis, "e") || !strcmp(dis, "quit") ||
			!strcmp(dis, "exit"))
			return head;
		cout << story[1] << "> ";
		cin.getline(dis, 16);
		if (!strcmp(dis, "q") || !strcmp(dis, "e") || !strcmp(dis, "quit") ||
			!strcmp(dis, "exit"))
			return head;
		cout << story[2];
		return head;
	}
	if (input == "a song, please") {
		string aSong =
			head + "\n    Oh my darling,\n" + "    oh my darling,\n" +
			"    oh my darling Clementine\n\n" + "    You are lost and\n" +
			"    gone forever,\n" + "    dreadful sorry Clementine\n\n" +
			"    How I missed her,\n" + "    how I missed her,\n" +
			"    how I missed my Clementine\n\n" + "    Until I kissed her\n" +
			"    little sister\n" + "    and forgot my Clementine";
		return aSong;
	}
	return info;
}

void BoardInterface::showComment(oneMove& move) {
#ifndef STARS_LANG_CHINESE
	// comment
	if ((move.word == "good" && move.byComputer) ||
		(move.word == "bad" && !move.byComputer))
		cout << "    not a promising future I'm afraid.\n";
	else if (
		(move.word == "bad" && move.byComputer) ||
		(move.word == "good" && !move.byComputer))
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
#else
	if ((move.word == "good" && move.byComputer) ||
		(move.word == "bad" && !move.byComputer))
		cout << "    似乎不妙\n";
	else if (
		(move.word == "bad" && move.byComputer) ||
		(move.word == "good" && !move.byComputer))
		cout << "    妙呀~\n";
	else {
		shortv non;
		analyse->state.nonFullColumn(non);
		if (move.list.size() == 1 && move.byComputer)
			cout << "    只剩一步，能走哪儿呢？\n";
		else if (move.list.size() == 1 && !move.byComputer)
			cout << "    只剩一步，能走哪儿呢？\n";
		else if (non.size() < 4)
			cout << "    剩下的空间不多了\n";
	}
#endif // STARS_LANG_CHINESE
}

bool BoardInterface::askToReverse(bool yes) {
	char input[8];
#ifndef STARS_LANG_CHINESE
	if (yes)
		printf("Care for a reverse mode? (default yes) (Yes/no)> ");
	else
		printf("Care for a reverse mode? (default no) (yes/No)> ");
#else
	if (yes)
		printf("进入撤回模式? (默认 yes) (Yes/no)> ");
	else
		printf("进入撤回模式? (默认 no) (yes/No)> ");
#endif // STARS_LANG_CHINESE
	cin.getline(input, 8);
	if ((yes && !strlen(input)) || !strcmp(input, "Y") ||
		!strcmp(input, "yes") || !strcmp(input, "y")) {
		return true;
	}
	return false;
}

void BoardInterface::askToSaveBoard(bool yes) {
	char input[8];
#ifndef STARS_LANG_CHINESE
	if (yes)
		printf("Save the old game? (yes as default) (Yes/no)> ");
	else
		printf("Save the old game? (no as default) (yes/No)> ");
#else
	if (yes)
		printf("将游戏存档? (默认 yes) (Yes/no)> ");
	else
		printf("将游戏存档? (默认 no) (yes/No)> ");
#endif // STARS_LANG_CHINESE
	cin.getline(input, 8);
	if ((yes && !strlen(input)) || !strcmp(input, "Y") ||
		!strcmp(input, "yes") || !strcmp(input, "y"))
		record.saveGame(analyse->state);
}

void BoardInterface::importNewBoard() {
#ifndef STARS_LANG_CHINESE
	printf("Paste a board down below>\n");
#else
	printf("粘贴一个棋盘>\n");
#endif // STARS_LANG_CHINESE
	if (!getStateFromInput())
		return;
	record.clearHistoryMove();
}

bool BoardInterface::isOver(const oneMove& move, const string& mode) {
#ifndef STARS_LANG_CHINESE
	string gameOver        = "Game is over, stars are gone.\n";
	string congratulations = "Congratulations, you win!\n";
	string boardFull       = "board is full, game is over, lura is gone.\n";
#else
	string gameOver = "游戏结束\n";
	string congratulations = "玩家获胜！\n";
	string boardFull = "棋盘满了\n";
#endif // STARS_LANG_CHINESE
	if (analyse->gameIsOver() == move.player) {
		analyse->show();
		if (move.byComputer)
			cout << gameOver;
		else {
			if (mode == "debug")
				cout << congratulations;
			else if (mode == "play")
#ifndef STARS_LANG_CHINESE
				cout << "Player " << move.player << " wins.\n";
#else
				cout << "玩家" << move.player << "获胜\n";
#endif // STARS_LANG_CHINESE
		}
		return true;
	}
	if (analyse->gameIsOver() == analyse->rPlayer(move.player)) {
		analyse->show();
		if (move.byComputer) {
			if (mode == "debug")
				cout << congratulations;
			else if (mode == "play")
#ifndef STARS_LANG_CHINESE
				cout << "Player " << analyse->rPlayer(move.player)
					 << " wins.\n";
#else
				cout << "玩家" << analyse->rPlayer(move.player) << "获胜\n";
#endif // STARS_LANG_CHINESE
		}
		else
			cout << gameOver;
		return true;
	}
	if (analyse->boardIsFull()) {
		cout << boardFull;
		return true;
	}
	return false;
}
