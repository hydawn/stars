/*-- encoding: UTF-8 --*/
#include <numeric>
#include "boardAnalyse.h"

using std::logic_error;

void BoardAnalyse::go(const char plr, const short move) {
	if (!state.colCanAdd(move))
		throw logic_error("Error: trying to add in a wrong place");
	state.add(plr, move);
}

void BoardAnalyse::reverse(const int column) {
	if (!state.colCanRemove(column))
		throw logic_error("Error: tying to reverse in a wrong place");
	state.remove(column);
}

shortv BoardAnalyse::firstPoint(const char plr, shortv& nonFullList) {
	shortv firstPointList;
	for (int col : nonFullList) {
		state.add(plr, col);
		if (plr == state.isOver())
			firstPointList.push_back(col);
		state.remove(col);
	}
	return firstPointList;
}

// check if game is over before call this
// won't return free if list is empty, relax
string BoardAnalyse::analyse(const char plr, shortv& list) {
	// if going to change code below, note that should always check the
	// firstPoint of those who is going to go first
#ifdef STARS_DEBUG_INFO
	if (state.isOver() != 'N') {
		throw logic_error("game is over, yet analyse is called");
	}
#endif
	shortv nonFull, temp1;

	state.nonFullColumn(nonFull);
	if (nonFull.empty())
		return "bad";
	list = firstPoint(plr, nonFull);
	if (!list.empty())
		return "good";

	char opp = state.rPlayer(plr);
	list     = firstPoint(opp, nonFull);
	if (list.size() > 1)
		return "bad";
	if (list.size() == 1) {
		int frontPos = list.front();
		state.add(plr, frontPos);
		state.sweepFullColumn(nonFull, frontPos);
		temp1 = firstPoint(opp, nonFull);
		state.remove(frontPos);
		if (!temp1.empty())
			return "bad";
		return "free";
	}

	// I'll go
	shortv list1, goodList;
	list = nonFull;
	for (auto col = list.begin(); col != list.end();) {
		state.add(plr, *col);
		state.nonFullColumn(nonFull);
		list1 = firstPoint(opp, nonFull);
		// it's not my turn to move, so check opp first
		if (!list1.empty()) { // bad
			state.remove(*col);
			col = list.erase(col);
			continue;
		}
		list1 = firstPoint(plr, nonFull);
		if (list1.size() > 1) // good
			goodList.push_back(*col);
		else if (list1.size() == 1) {
			shortv list2;
			state.add(opp, list1[0]);
			state.sweepFullColumn(nonFull, list1[0]);
			list2 = firstPoint(plr, nonFull);
			// now it's my turn, check plr first
			if (!list2.empty()) {
				goodList.push_back(*col);
				state.remove(list1[0], *col);
				++col;
				continue;
			}
			list2 = firstPoint(opp, nonFull);
			if (list2.size() > 1) {
				state.remove(list1[0], *col);
				col = list.erase(col);
				continue;
			}
			if (list2.size() == 1) {
				shortv list3;
				state.add(plr, list2[0]);
				state.sweepFullColumn(nonFull, list2[0]);
				list3 = firstPoint(opp, nonFull);
				// opponent's turn, check opp first
				if (!list3.empty()) {
					state.remove(list2[0], list1[0], *col);
					col = list.erase(col);
					continue;
				}
				list3 = firstPoint(plr, nonFull);
				// this is new
				if (list3.size() > 1)
					goodList.push_back(*col);
				state.remove(list2[0]);
			}
			state.remove(list1[0]);
		}
		state.remove(*col);
		++col;
	}

	if (!goodList.empty()) {
		list = goodList;
		return "good";
	}
	if (list.empty())
		return "bad";
	return "free";
}

string BoardAnalyse::oneMoveAnalyse(
	const char plr, const short col, const short depth, const short maxDepth) {
	// only analyse one Move
	char   opp = state.rPlayer(plr);
	string word;
	shortv list;

	state.add(plr, col);
	word = analyse(opp, list);
	if (word == "free" && list.size() == 1 && depth < maxDepth)
		word = oneMoveAnalyse(opp, list.front(), depth + 1, maxDepth);
	if (word == "good")
		word = "bad";
	else
		word = "good";
	state.remove(col);
	return word;
}

string BoardAnalyse::oneMoveAnalyseTrackRoute(
	const char plr, const short col, short goodNode, short badNode,
	const short depth, const short maxDepth) {
	// only analyse one Move
	char   opp = state.rPlayer(plr);
	shortv list;

	routes.add(col);
	routes.forward(col);

	state.add(plr, col);
	string word = analyse(opp, list);
	if (word == "good") {
		routes.add(badNode);
		word = "bad";
	}
	else if (word == "bad") {
		routes.add(goodNode);
		word = "good";
	}
	else if (list.size() == 1 && depth < maxDepth) {
		word = oneMoveAnalyseTrackRoute(
			opp, list.front(), badNode, goodNode, depth + 1, maxDepth);
		if (word == "good")
			word = "bad";
		else if (word == "bad")
			word = "good";
	}
	else
		routes.add(freeNode);
	state.remove(col);
	routes.backward();
	return word;
}

string BoardAnalyse::recursiveSituationTrackRoute(
	const char plr, shortv& list, int recDepth, int recCount, bool firstRound) {
	/*
	 * record which position is good in the first round
	 * in other rounds, return immediately when detected a good point
	 * */
	// ? how do I do, if I want one function to track route, but don't want the
	// ? other call to do almost the same thing but don't track it

	// checking
	if (list.empty()) {
		throw logic_error("recursiveSituation: given list is empty");
		return "end";
	}
	if (state.isOver() != 'N')
		throw logic_error("game is over, yet recursiveSituation is called");
	if (state.boardIsFull())
		throw logic_error("board is full, yet recursiveSituation is called");
	// checking done

	// if the situation is quite clear, just return
	string word = analyse(plr, list);
	if (word == "good" || word == "bad" /* || list.size() == 1 */)
		return word;

	// if must return, but:
	if (recDepth <= recCount) {
		if (list.size() == 1)
			return oneMoveAnalyseTrackRoute(
				plr, list.front(), goodNode, badNode);
		routes.add(freeNode);
		return "free";
	}
	++recCount;

	char   opp      = state.rPlayer(plr);
	int    badCount = 0;
	shortv nextList, goodList;

	if (firstRound)
		routes.clear();
	routes.add(list);
	routes.forward(list.front());
	for (const int col : list) {
		state.add(plr, col);
		word = analyse(opp, nextList);
		if (word == "good") {
			routes.add(badNode);
			++badCount;
		}
		else if (word == "bad") {
			routes.add(goodNode);
			if (!firstRound) {
				state.remove(col);
				routes.backward();
				return "good";
			}
			goodList.push_back(col);
		}
		else {
			word = recursiveSituationTrackRoute(opp, nextList, recDepth,
				recCount, false);
			if (word == "good")
				++badCount;
			else if (word == "bad") {
				if (!firstRound) {
					state.remove(col);
					routes.backward();
					return "good";
				}
				goodList.push_back(col);
			}
		}

		state.remove(col);
		routes.nextNode();
	}
	routes.backward();

	if (badCount == list.size())
		return "bad";
	if (false == goodList.empty()) {
		list = goodList;
		return "good";
	}
	return "free";
}

string BoardAnalyse::recursiveSituation(
	const char plr, shortv& list, int recDepth, int recCount, bool firstRound) {
	// checking
	if (list.empty()) {
		throw logic_error("recursiveSituation: given list is empty");
		return "end";
	}
	if (state.isOver() != 'N')
		throw logic_error("game is over, yet recursiveSituation is called");
	if (state.boardIsFull())
		throw logic_error("board is full, yet recursiveSituation is called");
	// checking done

	// if the situation is quite clear, just return
	string word = analyse(plr, list);
	if (word == "good" || word == "bad" /* || list.size() == 1 */)
		return word;

	// if must return, but:
	if (recDepth <= recCount) {
		if (list.size() == 1)
			return oneMoveAnalyse(plr, list.front());
		return "free";
	}
	++recCount;

	char   opp      = state.rPlayer(plr);
	int    badCount = 0;
	shortv nextList, goodList;

	for (const int col : list) {
		state.add(plr, col);
		word = analyse(opp, nextList);
		if (word == "free")
			word = recursiveSituation(opp, nextList, recDepth, recCount, false);
		state.remove(col);

		if (word == "good")
			++badCount;
		else if (word == "bad") {
			if (!firstRound)
				return "good";
			goodList.push_back(col);
		}
	}

	if (badCount == list.size())
		return "bad";
	if (false == goodList.empty()) {
		list = goodList;
		return "good";
	}
	return "free";
}

int BoardAnalyse::respond(
	const char plr, oneMove& thisMove, bool showCal, bool showTime,
	bool starsOn, bool trackRoute) {
	// list for myself
	shortv    plrList(state.cols), oppList(state.cols), nonFullList;
	long long timeUsed = 0;
	short     recDepth = 2;
	string    word;
	std::iota(plrList.begin(), plrList.end(), 1);
	std::iota(oppList.begin(), oppList.end(), 1);

	// pre-test
	state.nonFullColumn(nonFullList);
#ifdef STARS_DEBUG_INFO
	if (nonFullList.empty())
		throw logic_error("call respond with full board");
	if (state.isOver() == plr || state.isOver() == state.rPlayer(plr))
		throw logic_error("call respond with ended game");
#endif // STARS_DEBUG_INFO

	// analyse
	// && state.pieceCount() < cols*rows * 0.75 ?
	if (starsOn && nonFullList.size() > 4)
		state.areaTopTransform();
	do {
		timeUsed = returnTime(plr, plrList, ++recDepth, word, trackRoute);
	} while (word == "free" && timeUsed < maxcaltime && recDepth < 10);
	// this opp list is for the random suggestion functions
	if (recDepth > 2)
		recursiveSituation(state.rPlayer(plr), oppList, recDepth - 1);
	else
		recursiveSituation(state.rPlayer(plr), oppList, recDepth);
	state.areaTopRestore();

	// in case something unpleasent happens:
	if (starsOn && word != "free" && recDepth > 5 && timeUsed < maxcaltime &&
		nonFullList.size() < 12) {
		recDepth = 2;
		do {
			timeUsed = returnTime(plr, plrList, ++recDepth, word, trackRoute);
		} while (word == "free" && timeUsed < maxcaltime && recDepth < 10);
#ifndef STARS_LANG_CHINESE
		if (showCal)
			cout << "    calculation depth without stars = " << recDepth - 1
				 << endl;
	}
	else if (showCal) {
		if (starsOn)
			cout << "    calculation depth with stars = " << recDepth - 1
				 << endl;
		else
			cout << "    calculation depth without stars = " << recDepth - 1
				 << endl;
	}

	// show info if needed
	if (showCal) {
		cout << "    word = " << word << ", list = [ ";
		for (short c : plrList)
			cout << c << " ";
		printf("]\n");
	}
	if (showTime)
		cout << "    calculate time used: " << timeUsed << " ms\n";
#else
		if (showCal)
			cout << "    没有星星的计算深度 = " << recDepth - 1 << endl;
	}
	else if (showCal) {
		if (starsOn)
			cout << "    有星星的计算深度 = " << recDepth - 1 << endl;
		else
			cout << "    没有星星的计算深度 = " << recDepth - 1 << endl;
	}

	// show info if needed
	if (showCal) {
		cout << "    玩家状态 = " << toChinese(word) << ", 推荐列表 = [ ";
		for (short c : plrList)
			cout << c << " ";
		printf("]\n");
	}
	if (showTime)
		cout << "    程序计算用时 " << timeUsed << " 毫秒\n";
#endif // STARS_LANG_CHINESE

	// record, suggest and return
	thisMove.word = word;
	thisMove.list = plrList;
	thisMove.time = timeUsed;
	if (word != "bad")
		return state.randomSuggestion(plr, plrList, oppList);
	else if (word == "bad") {
		word = analyse(plr, plrList);
		if (word == "bad") {
			oppList = firstPoint(rPlayer(plr), nonFullList);
			if (oppList.empty())
				return state.randomMove();
			return state.randomMove(oppList);
		}
		else
			return state.randomSuggestion(plr, plrList, oppList);
	}
	else
		throw logic_error("wrong word returned in respond");
	return 0;
}

long long BoardAnalyse::returnTime(
	const char plr, shortv& list, const int recDepth, string& word,
	const bool track) {
	clearMatch();
	auto start = system_clock::now();
	if (track)
		word = recursiveSituationTrackRoute(plr, list, recDepth);
	else
		word = recursiveSituation(plr, list, recDepth);
	auto end   = system_clock::now();
	checkMatch();
	return duration_cast<milliseconds>(end - start).count();
}

void BoardAnalyse::checkMatch() {
	static int called = 0;
	++called;
	if (!state.match()) {
		cout << "add for " << state.addNumber << " times\n"
			 << "remove for " << state.removeNumber << "times\n"
			 << "called " << called << " times" << endl;
		throw logic_error("add and remove doesn't match");
	}
	else {
		state.addNumber    = 0;
		state.removeNumber = 0;
		// cout << "called " << called << " times, match" << endl;
	}
}