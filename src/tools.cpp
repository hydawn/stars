/*-- encoding: UTF-8 --*/
#include <stdexcept>
#include <iostream>
#include <algorithm>
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

void copy(vector<int>& list, const Json::Value& root) {
	list.clear();
	for (int i = 0; i < root.size(); ++i)
		list.push_back(root[i].asInt());
}
} // namespace MyShortList

namespace MyJson {
Json::Value toValue(const vector<int>& list) {
	Json::Value root;
	for (int i : list)
		root.append(i);
	return std::move(root);
}

vector<int> toVectorInt(const Json::Value& root) {
	vector<int> list;
	for (int i = 0; i < root.size(); ++i)
		list.push_back(root[i].asInt());
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
