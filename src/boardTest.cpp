#include "boardTest.h"

char BoardTest::lastMove = 'c';

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

	// main loop
	while (true) {
		input = getInput();
		if (input == "c" || input == "change") {
			// change player
			std::swap(byPlayer.player, byOpponent.player);
			std::swap(byPlayer.word, byOpponent.word);
			std::swap(byPlayer.list, byOpponent.list);
			std::swap(byPlayer.suggestion, byOpponent.suggestion);
			continue;
		}
		else if (isOver(byPlayer)) {
			printf("Exit from test mode ...\n");
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
		printf("%d:'%c' goes '%d'\t", ++stepCount, byOpponent.player, byOpponent.move);
		// see if expectedWinner is changed
		if (byOpponent.word == "good" && stepCount > 3) {
			if (expectedWinner != byOpponent.player && expectedWinner != '1') {
				cout << "Expected winner changed from "
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
				cout << "Expected winner changed from "
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
		if (record.getDefaultSettings("inDebugMode", "showTime"))
			cout << "    input time used: " << byPlayer.time << "ms\n";
		if (isOver(byOpponent)) {
			printf("Exit from test mode ...\n");
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
		if (byPlayer.hintOn && byPlayer.word != "bad")
			printf("    %d is recommended\n", byPlayer.suggestion);
	}
#ifdef STARS_DEBUG_INFO
	throw logic_error("control flow into the end of debug mode\n");
#endif
}

void BoardTest::askToSaveBoard(bool yes) {
	if (yes)
		record.saveGame(analyse->state);
}

bool BoardTest::isOver(const oneMove& move) {
	if (analyse->gameIsOver() == move.player) {
		toWinn = move.player;
		printf("\n");
		analyse->starShow();
		if (move.byComputer)
			printf("Game is over, stars are gone.\n");
		else
			printf("Congratulations, you win!\n");
		return true;
	}
	if (analyse->gameIsOver() == analyse->rPlayer(move.player)) {
		toWinn = move.player;
		printf("\n");
		analyse->starShow();
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
