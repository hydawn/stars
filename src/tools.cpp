/*-- encoding: UTF-8 --*/
#include <stdexcept>
#include <iostream>
#include <algorithm>
#include "boardState.h"
#include "tools.h"

namespace MyShortList {
bool inList(vector<int>& sl, int i) {
	return sl.end() != find(sl.begin(), sl.end(), i);
}

void shortIntersection(vector<int>& dst, vector<int>& src1, vector<int>& src2) {
	dst.clear();
	std::sort(src1.begin(), src1.end());
	std::sort(src2.begin(), src2.end());
	std::set_intersection(
		src1.begin(), src1.end(), src2.begin(), src2.end(),
		std::back_inserter(dst));
}
} // namespace MyShortList

namespace MyJson {
Json::Value toValue(const vector<int>& list) {
	Json::Value root;
	for (const int i : list)
		root.append(i);
	return std::move(root);
}

vector<int> toVectorInt(const Json::Value& root) {
	vector<int> list;
	for (auto i : root)
		list.push_back(i.asInt());
	return list;
}
} // namespace MyJson

namespace ToInt {
int myStoi(const string& word) {
	int num = std::stoi(word);
	if (!xtoiFit(word, num))
		throw std::invalid_argument("myStoi::invalid_argument");
	return num;
}

bool xtoiFit(const string& word, const int num) {
	return std::to_string(num).size() == word.size();
}
} // namespace ToInt

namespace MainArgsHandle {
vector<string> charListToVector(int argc, char* args[]) {
	vector<string> rax;
	for (int i = 0; i < argc; ++i)
		rax.push_back(args[i]);
	return rax;
}

bool inVector(const vector<string>& argv, const string& str) {
	return argv.end() != std::find(argv.begin(), argv.end(), str);
}
} // namespace MainArgsHandle

namespace Random {
int randomMove(const BoardState& state) {
	shortv list;
	state.nonFullColumn(list);
#ifdef STARS_DEBUG_INFO
	if (list.empty())
		throw logic_error("trying randomMove() in full board");
#endif // STARS_DEBUG_INFO
	return randomMove(list);
}

int randomMove(const shortv& list) {
#ifdef STARS_DEBUG_INFO
	if (list.empty())
		throw logic_error("trying randomMove(shortv& list) in an empty list");
#endif // STARS_DEBUG_INFO
	srand((unsigned)time(NULL));
	return list[rand() % list.size()];
}

int randomSuggestion(
	const BoardState& state, const char plr, shortv& list, const string& mode) {
	shortv opp2, opp1, plr2, plr1;
	char   opp = rPlayer(plr);
	srand((unsigned)time(NULL));
	const vector<string>& board = state.getBoard();
	const shortv&         top   = state.getTop();
	for (const int col : list) {
		if (state.colIsFull(col) || state.colIsEmpty(col))
			continue;
		const string& line = board[col - 1];
		const int     head = top[col - 1] - 1;
		if (line[head] == plr) {
			if (head > 0 && line[head - 1] == plr)
				plr2.push_back(col);
			else
				plr1.push_back(col);
		}
		else {
			if (head > 0 && line[head - 1] == opp)
				opp2.push_back(col);
			else
				opp1.push_back(col);
		}
	}
	int ran = rand() % 100;
	if (mode == "progressive") {
		if (!plr2.empty() && ran < 72)
			return randomMove(plr2);
		ran = rand() % 100;
		if (!opp2.empty() && ran < 95)
			return randomMove(opp2);
		ran = rand() % 100;
		if (!plr1.empty() && ran < 95)
			return randomMove(plr1);
		ran = rand() % 100;
		if (!opp1.empty() && ran < 57)
			return randomMove(opp1);
	}
	else if (mode == "defensive") {
		if (!opp2.empty() && ran < 95)
			return randomMove(opp2);
		ran = rand() % 100;
		if (!plr2.empty() && ran < 72)
			return randomMove(plr2);
		ran = rand() % 100;
		if (!opp1.empty() && ran < 77)
			return randomMove(opp1);
		ran = rand() % 100;
		if (!plr1.empty() && ran < 65)
			return randomMove(plr1);
	}
#ifdef STARS_DEBUG_INFO
	else
		throw logic_error("no such mode");
	if (list.empty())
		throw logic_error("call randomSuggestion with empty list");
#endif // STARS_DEBUG_INFO
	return randomMove(list);
}

int randomSuggestion(
	const BoardState& state, const char plr, shortv& list, shortv oppList,
	const string& mode) {
#ifdef STARS_DEBUG_INFO
	if (list.empty())
		throw logic_error("call randomSuggestion(4 args) with empty list");
#endif
	shortv intersectionList;
	BoardState board = state;
	// preference No.1: take what can bring me winn-1 in a row, and what can
	// interrupt opponent's three in a row
	// but some times it block itself which is rather stupid
	shortv plrTList = board.makeThreeCols(plr, list),
		   oppTList = board.makeThreeCols(rPlayer(plr), oppList);
	srand((unsigned)time(NULL));
	if (rand() % 100 < 85) {
		MyShortList::shortIntersection(intersectionList, plrTList, oppTList);
		if (intersectionList.empty()) {
			if (plrTList.empty()) {
				MyShortList::shortIntersection(
					intersectionList, list, oppTList);
				if (!intersectionList.empty())
					return randomSuggestion(state, plr, intersectionList, mode);
			}
			else
				return randomSuggestion(state, plr, plrTList, mode);
		}
		else
			return randomSuggestion(state, plr, intersectionList, mode);
	}

	// else if everything is empty
	// preference No.2: take the opponent's safe list
	MyShortList::shortIntersection(intersectionList, list, oppList);
	if (intersectionList.empty())
		return randomSuggestion(state, plr, list, mode);
	return randomSuggestion(state, plr, intersectionList, mode);
}

} // namespace Random
#ifdef STARS_LANG_CHINESE
string toChinese(const string& word) {
	if (word == "good")
		return "好";
	if (word == "bad")
		return "坏";
	if (word == "free")
		return "自由"; // 我免费了！
	if (word == "debug")
		return "人机";
	if (word == "play")
		return "双人";
	if (word == "board width")
		return "棋盘宽度";
	if (word == "board height")
		return "棋盘高度";
	if (word == "win number")
		return "获胜所需连排最小数";
	if (word == "changeBoard")
		return "更换棋盘";
	if (word == "askToSaveBoard")
		return "询问棋盘是否存档";
	if (word == "defaultSaveBoard")
		return "默认棋盘是否存档";
	if (word == "gameIsOver")
		return "游戏结束";
	if (word == "askToReverse")
		return "询问进入撤回模式";
	if (word == "defaultReverse")
		return "默认进入撤回模式";
	if (word == "inCustomMode")
		return "自定义模式";
	if (word == "inDebugMode")
		return "普通模式";
	if (word == "hintOn")
		return "打开提示";
	if (word == "showCalculate")
		return "显示程序计算过程";
	if (word == "showTime")
		return "显示用时";
	if (word == "starrySky")
		return "星空";
	if (word == "starsOn")
		return "晴天";
	if (word == "trackRoutes")
		return "追踪路径";
	if (word == "whenSaveGame")
		return "保存游戏";
	if (word == "askGiveName")
		return "询问命名存档";
	if (word == "defaultGiveName")
		return "默认命名存档";
	if (word == "maxcaltime")
		return "最大计算时间";
	std::cout << "Word = " << word << std::endl;
	throw logic_error("toChinese: wrong input");
}

string toChinese(const bool word) {
	if (word)
		return "真";
	return "假";
}
#endif // STARS_LANG_CHINESE
