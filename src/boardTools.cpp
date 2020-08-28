#include <stdexcept>
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
	for (int i = 0; i < argc; ++i) {
		rax.push_back(args[i]);
	}
	return rax;
}

bool inCharList(int argc, char* argv[], const string& str) {
	for (int i = 0; i < argc; ++i)
		if (!strcmp(argv[i], str.c_str()))
			return true;
	return false;
}

bool inVector(vector<string> argv, const string& str) {
	vector<string>::iterator iter = argv.begin();
	for (; iter != argv.end(); ++iter)
		if (*iter == str)
			return true;
	return false;
}
} // namespace MainArgsHandle
