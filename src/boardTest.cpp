#include "boardTest.h"

char BoardTest::lastMove = 'c';

BoardTest::BoardTest(const vector<string>& args)
	: BoardInterface(),
	  toWinn('N'),
	  showCalculate(record.getDefaultSettings("inDebugMode", "showCalculate")),
	  showTime(record.getDefaultSettings("inDebugMode", "showTime")),
	  askToReverseBool(record.getDefaultSettings("gameIsOver", "askToReverse")),
	  askToSaveBoardBool(
		  record.getDefaultSettings("gameIsOver", "askToSaveBoard")),
	  lessPrint(false),
	  noPrint(false) {
	if (MainArgsHandle::inVector(args, "--no-hint")) {
		showCalculate = false;
		showTime      = false;
	}
	if (MainArgsHandle::inVector(args, "--no-ask")) {
		askToReverseBool   = false;
		askToSaveBoardBool = false;
	}
	if (MainArgsHandle::inVector(args, "--less-print")) {
		askToReverseBool   = false;
		askToSaveBoardBool = false;
		showCalculate      = false;
		showTime           = false;
		lessPrint          = true;
	}
	if (MainArgsHandle::inVector(args, "--no-print")) {
		askToReverseBool   = false;
		askToSaveBoardBool = false;
		showCalculate      = false;
		showTime           = false;
		lessPrint          = true;
		noPrint            = true;
	}
}

string BoardTest::getInput(const string& mode, const short& suggestion) {
	if (mode == "play") {
		return std::to_string(suggestion);
	}
	// generate an input here
	if (lastMove == 'c') {
		lastMove = 'm';
		return "m";
	}
	else if (lastMove == 'm') {
		lastMove = 'c';
		return "c";
	}
	throw logic_error("lastMove failure");
	return string();
}

string BoardTest::debugMode(const string& mode) {
	// init
	if (mode == "debug") {
		byOpponent.byComputer = true;
	}
	else {
		if (!noPrint)
			printf("We are in play mode.\n");
		byOpponent.byComputer = true;
	}
	string input;
	byPlayer.mode       = "test-" + mode;
	byPlayer.byComputer = true;
	byOpponent.mode     = "test-" + mode;
	byOpponent.player   = rPlayer(byPlayer.player);
	char expectedWinner = '1';
	int  stepCount      = 0;
	if (!lessPrint) { // if no lessPrint
		printf("We are in debug mode now\n");
		analyse->show();
	}
	// main loop
	while (true) {
		input = getInput(mode, byPlayer.suggestion);
		if (input == "c" || input == "change") {
			// change player
			std::swap(byPlayer.player, byOpponent.player);
			std::swap(byPlayer.word, byOpponent.word);
			std::swap(byPlayer.list, byOpponent.list);
			std::swap(byPlayer.suggestion, byOpponent.suggestion);
			std::swap(byPlayer.move, byOpponent.move);
			continue;
		}
		else if (input == "m" || input == "move") {
			if (mode == "play") {
				if (!lessPrint)
					cout << "You can't force your friend to play right?\n";
				continue;
			}
			else if (isOver(byPlayer)) {
				if (expectedWinner != byPlayer.player) {
					throw logic_error(
						"BoardTest::debugMode:expected winner changed in the "
						"end");
				}
				if (expectedWinner == '1') {
					throw logic_error(
						"BoardTest::debugMode: player suddenly wins");
				}
				if (!lessPrint)
					printf("Exit from test mode ...\n");
				return "over";
			}
		}
		else if (mode == "play") {
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
			if (!lessPrint)
				printf(
					"    %d:'%c' goes '%d'\n", ++stepCount, byPlayer.player,
					byPlayer.move);
			else if (!noPrint)
				printf(
					"%d:'%c' goes '%d'\t", ++stepCount, byPlayer.player,
					byPlayer.move);
			record.push_back(byPlayer); // byPlayer end here
			if (isOver(byPlayer)) {
				if (!noPrint)
					cout << "Exit from " << mode << " mode ...\n";
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
		else {
			// opp respond
			byOpponent.suggestion = respond();
			// opp move
			byOpponent.move = byOpponent.suggestion;
			analyse->go(byOpponent.player, byOpponent.move);
			if (!lessPrint)
				printf(
					"    %d:'%c' goes '%d'\n", ++stepCount, byOpponent.player,
					byOpponent.move);
			else if (!noPrint)
				printf(
					"%d:'%c' goes '%d'\t", ++stepCount, byOpponent.player,
					byOpponent.move);
			record.push_back(byOpponent);
			if (record.getDefaultSettings("inDebugMode", "showTime") && showTime)
				cout << "    input time used: " << byPlayer.time << " ms\n";
			if (isOver(byOpponent)) {
				if (!noPrint)
					cout << "Exit from " << mode << " mode ...\n";
				return "over";
			}
		}

		// print hint
		if (showCalculate) {
			byPlayer.hintOn =
				record.getDefaultSettings("inDebugMode", "hintOn");
			if (byPlayer.hintOn)
				cout << "\nHere is hint provided for player " << byPlayer.player
					 << ":\n";
		}

		// see if expectedWinner is changed
		if (byOpponent.word == "good" && stepCount > 3) {
			if (expectedWinner != byOpponent.player && expectedWinner != '1') {
				throw logic_error(
					"BoardTest::debugMode: expected winner changed in good");
			}
			else
				expectedWinner = byOpponent.player;
		}
		else if (byOpponent.word == "bad" && stepCount > 3) {
			if (expectedWinner != byPlayer.player && expectedWinner != '1') {
				throw logic_error(
					"BoardTest::debugMode: expected winner changed in bad");
			}
			else
				expectedWinner = byPlayer.player;
		}
		if (isOver(byOpponent)) {
			if (expectedWinner != byOpponent.player) {
				throw logic_error(
					"BoardTest::debugMode: expected winner changed in the end");
			}
			if (expectedWinner == '1') {
				throw logic_error("BoardTest::debugMode: player suddenly wins");
			}
			if (!lessPrint)
				printf("Exit from test mode ...\n");
			return "over";
		}

		// recommend
		byPlayer.suggestion = analyse->respond(
			byPlayer.player, byPlayer, showCalculate, showTime,
			record.getDefaultSettings("inDebugMode", "starsOn"), false);
		if (showCalculate && byPlayer.word != "bad")
			printf("    %d is recommended\n", byPlayer.suggestion);

		// show
		if (!lessPrint) {
			printf("\n");
			if (record.getDefaultSettings("inDebugMode", "starrySky"))
				analyse->starShow();
			else
				analyse->show();
			printf("\n");
		}
	}
#ifdef STARS_DEBUG_INFO
	throw logic_error("control flow into the end of debug mode");
#endif
}

short BoardTest::respond() {
	if (showCalculate && !lessPrint)
		printf("Info for player %c:\n", byOpponent.player);
	byOpponent.suggestion = analyse->respond(
		byOpponent.player, byOpponent, showCalculate, showTime,
		record.getDefaultSettings("inDebugMode", "starsOn"),
		record.getDefaultSettings("inDebugMode", "trackRoutes"));
#ifdef STARS_DEBUG_INFO
	if (!byOpponent.list.empty() &&
		!MyShortList::inList(byOpponent.list, byOpponent.suggestion))
		throw logic_error("suggestion not in safe list");
#endif // STARS_DEBUG_INFO
	return byOpponent.suggestion;
}

void BoardTest::askToSaveBoard(bool yes) {
	if (yes)
		record.saveGame("auto test auto saved games", analyse->state);
}

bool BoardTest::isOver(const oneMove& move) {
	if (analyse->gameIsOver() == move.player) {
		toWinn = move.player;
		if (!noPrint) {
			cout << endl;
			analyse->starShow();
			if (move.byComputer)
				printf("Game is over, stars are gone.\n");
			else
				printf("Congratulations, you win!\n");
		}
		return true;
	}
	if (analyse->gameIsOver() == rPlayer(move.player)) {
		toWinn = move.player;
		if (!noPrint) {
			cout << endl;
			analyse->starShow();
			if (move.byComputer)
				printf("Congratulations, you win!\n");
			else
				printf("Game is over, stars are gone.\n");
		}
		return true;
	}
	if (analyse->boardIsFull()) {
		if (!noPrint)
			printf("board is full, game is over, lura is gone.\n");
		return true;
	}
	return false;
}

bool BoardTest::controlMode(const string& firstMode) {
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
			if (askToSaveBoardBool)
				askToSaveBoard(record.getDefaultSettings(
					"gameIsOver", "defaultSaveBoard"));
			else if (record.getDefaultSettings(
						 "gameIsOver", "defaultSaveBoard"))
				record.saveGame("test mode auto save", analyse->state);
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
	if (!lessPrint)
		cout << "Exit from controller\n";
	return true;
}

void autoTest(int n, const vector<string>& args) {
	int    i, err = 0, XWinn = 0, ZeroWinn = 0;
	string mode = "play";
	if (!MainArgsHandle::inVector(args, "--play"))
		mode = "debug";
	for (i = 0; i < n; ++i) {
		try {
			BoardTest test(args);
			try {
				test.byPlayer.suggestion =
					i % 8 + 1; // set play mode's first step
				test.controlMode(mode);
				if (test.toWinn == 'X')
					++XWinn;
				else if (test.toWinn == '0')
					++ZeroWinn;
			}
			catch (const std::runtime_error& e) {
				cout << "runtime_error: ";
				std::cerr << e.what() << '\n';
				string autoTest = "autoTest-error:";
				test.record.saveGame(
					autoTest + e.what() + "starsOn:" +
						std::to_string(test.record.getDefaultSettings(
							"inDebugMode", "starsOn")),
					test.analyse->state);
				++err;
			}
			catch (const std::logic_error& e) {
				cout << "logic_error: ";
				std::cerr << e.what() << '\n';
				string autoTest = "autoTest-error:";
				test.record.saveGame(
					autoTest + e.what() + "starsOn:" +
						std::to_string(test.record.getDefaultSettings(
							"inDebugMode", "starsOn")),
					test.analyse->state);
				++err;
			}
		}
		catch (const std::logic_error& e) {
			std::cerr << e.what() << '\n';
			cout << "logic_error occurred when creating or destroying "
					"BoardTest\n";
		}
		catch (const std::runtime_error& e) {
			std::cerr << e.what() << '\n';
			cout << "runtime_error occurred when creating or destroying "
					"BoardTest\n";
		}
	}
	cout << endl
		 << "Tested for\t\t" << i << " rounds" << endl
		 << "Error occurred\t\t" << err << " times" << endl
		 << endl
		 << "Player 0 goes first\n"
		 << "Player X win:\t\t" << XWinn << " times" << endl
		 << "Player 0 win:\t\t" << ZeroWinn << " times" << endl
		 << "Over.";
}
