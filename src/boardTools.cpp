/*-- encoding: UTF-8 --*/
#include <stdexcept>
#include <iostream>
#include "boardTools.h"

namespace MyShortList {
bool inList(ShortList& sl, short i) {
	for (short j : sl)
		if (j == i)
			return true;
	return false;
}

void shortIntersection(ShortList& dest, ShortList& sour1, ShortList& sour2) {
	dest.clear();
	if (sour1.empty())
		return;
	for (short i : sour1) {
		if (inList(sour2, i))
			dest.push_back(i);
	}
}

bool equal(ShortList& l1, ShortList& l2) {
	if (l1.size() != l2.size())
		return false;
	for (short i = 0; i < l1.size(); ++i)
		if (l1[i] != l2[i])
			return false;
	return true;
}
}; // namespace MyShortList

namespace ToInt {
int myStoi(string word) {
	int num = std::stoi(word); // stupid stoi
	if (!xtoiFit(word, num))
		throw std::invalid_argument("myStoi::invalid_argument");
	return num;
}

bool xtoiFit(const string& word, int num) {
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

bool inCharList(int argc, char* argv[], const string& str) {
	for (int i = 0; i < argc; ++i)
		if (!strcmp(argv[i], str.c_str()))
			return true;
	return false;
}

bool inVector(vector<string>& argv, const string& str) {
	vector<string>::iterator iter = argv.begin();
	for (; iter != argv.end(); ++iter)
		if (*iter == str)
			return true;
	return false;
}
} // namespace MainArgsHandle

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
