#include "boardTest.h"

char BoardTest::lastMove = 'c';

BoardTest::BoardTest(const string& option) : BoardInterface(), toWinn('N'),
	showCalculate(record.getDefaultSettings("inDebugMode", "showCalculate")),
	showTime(record.getDefaultSettings("inDebugMode", "showTime")),
	lessPrint(false), noPrint(false) {
	if (option == "no-hint") {
		showCalculate = false;
		showTime	  = false;
	}
	else if (option == "less-print") {
		showCalculate = false;
		showTime	  = false;
		lessPrint	  = true;
	}
	else if (option == "no-print") {
		showCalculate = false;
		showTime	  = false;
		lessPrint	  = true;
		noPrint		  = true;
	}
}

string BoardTest::getInput() {
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

string BoardTest::debugMode(oneMove& byPlayer) {
	// init
	oneMove byOpponent;
	string	input;
	byPlayer.mode		  = "test";
	byPlayer.byComputer	  = true;
	byOpponent.mode		  = "test";
	byOpponent.byComputer = true;
	byOpponent.player	  = analyse->rPlayer(byPlayer.player);
	char expectedWinner	  = '1';
	int	 stepCount		  = 0;
	if (!lessPrint) { // if no lessPrint
		printf("We are in debug mode now\n");
		analyse->show();
	}
	// main loop
	while (true) {
		input = getInput();
		if (input == "c" || input == "change") {
			// change player
			std::swap(byPlayer.player, byOpponent.player);
			std::swap(byPlayer.word, byOpponent.word);
			std::swap(byPlayer.list, byOpponent.list);
			std::swap(byPlayer.suggestion, byOpponent.suggestion);
			std::swap(byPlayer.move, byOpponent.move);
			continue;
		}
		else if (isOver(byPlayer)) {
			if(!lessPrint)
				printf("Exit from test mode ...\n");
			return "over";
		}

		// opp respond
		if (showCalculate && showTime)
			printf("Info for the computer:\n");
		byOpponent.move = analyse->respond(byOpponent.player, byOpponent,
			showCalculate, showTime,
			record.getDefaultSettings("inDebugMode", "starsOn"),
			record.getDefaultSettings("inDebugMode", "trackRoutes"));
#ifdef STARS_DEBUG_INFO
		if (!byOpponent.list.empty() &&
			!MyShortList::inList(byOpponent.list, byOpponent.move))
			throw logic_error("suggestion not in safe list");
#endif
		analyse->go(byOpponent.player, byOpponent.move);
		if (!noPrint)
			printf("%d:'%c' goes '%d'\t", ++stepCount, byOpponent.player, byOpponent.move);
		// see if expectedWinner is changed
		if (byOpponent.word == "good" && stepCount > 3) {
			if (expectedWinner != byOpponent.player && expectedWinner != '1') {
				cout << "\nExpected winner changed from "
					<< expectedWinner << " to "
					<< byOpponent.player << ", why?\n";
				cout << "player " << byPlayer.player << " goes "
					<< byPlayer.move << ", then opp respond with the board:\n";
				analyse->starShow();
				throw logic_error("expected winner changed");
			}
			else
				expectedWinner = byOpponent.player;
		}
		else if (byOpponent.word == "bad" && stepCount > 3) {
			if (expectedWinner != byPlayer.player && expectedWinner != '1') {
				cout << "\nExpected winner changed from "
					<< expectedWinner << " to "
					<< byPlayer.player << ", why?\n";
				cout << "player " << byPlayer.player << " goes "
					<< byPlayer.move << ", then opp respond with the board:\n";
				analyse->starShow();
				throw logic_error("expected winner changed");
			}
			else
				expectedWinner = byPlayer.player;
		}
		byOpponent.suggestion = byOpponent.move;
		record.push_back(byOpponent);
		if (isOver(byOpponent)) {
			if (!lessPrint)
				printf("Exit from test mode ...\n");
			return "over";
		}

		// recommend
		byPlayer.hintOn = showCalculate;
		if (showCalculate)
			cout << "\nHere is hint provided for you\n";
		byPlayer.suggestion = analyse->respond(byPlayer.player, byPlayer,
			showCalculate, showTime,
			record.getDefaultSettings("inDebugMode", "starsOn"),
			false);
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

void BoardTest::askToSaveBoard(bool yes) {
	if (yes)
		record.saveGame("auto test auto saved games", analyse->state);
}

bool BoardTest::isOver(const oneMove& move) {
	if (analyse->gameIsOver() == move.player) {
		toWinn = move.player;
		if(!noPrint) {
			cout << endl;
			analyse->starShow();
			if (move.byComputer)
				printf("Game is over, stars are gone.\n");
			else
				printf("Congratulations, you win!\n");
		}
		return true;
	}
	if (analyse->gameIsOver() == analyse->rPlayer(move.player)) {
		toWinn = move.player;
		if(!noPrint) {
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

bool BoardTest::controlMode() {
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
		throw runtime_error("too much unhandled advice");
	if (advice == "quit")
		return false;
	if (!lessPrint)
		cout << "Exit from controller\n";
	return true;
}

void autoTest(int n, string option) {
	int i, err = 0, XWinn = 0, ZeroWinn = 0;
	for (i = 0; i < n; ++i) {
		try {
			BoardTest test(option);
			try {
				test.controlMode();
				if (test.toWinn == 'X')
					++XWinn;
				else if (test.toWinn == '0')
					++ZeroWinn;
			}
			catch (const std::runtime_error& e) {
				cout << "runtime_error: ";
				std::cerr << e.what() << '\n';
				test.askToSaveBoard(true);
				++err;
			}
			catch (const std::logic_error& e) {
				cout << "logic_error: ";
				std::cerr << e.what() << '\n';
				test.askToSaveBoard(true);
				++err;
			}
		}
		catch (const std::logic_error& e) {
			std::cerr << e.what() << '\n';
			cout << "logic_error occurred when creating or destroying BoardTest\n";
		}
		catch (const std::runtime_error& e) {
			std::cerr << e.what() << '\n';
			cout << "runtime_error occurred when creating or destroying BoardTest\n";
		}
	}
	cout << "tested for\t\t" << i << " rounds" << endl;
	cout << "error occurred\t\t" << err << " times" << endl;
	cout << endl;
	cout << "Player 0 goes first\n";
	cout << "Player X win:\t\t" << XWinn << " times" << endl;
	cout << "Player 0 win:\t\t" << ZeroWinn << " times" << endl;
	cout << "Over, hit 'Enter' to close ...";
	cin.get();
}
