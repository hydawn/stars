#ifndef _MAINARGSHANDLE_H_
#define _MAINARGSHANDLE_H_ 1
#include <string>
#include <vector>
#include "shortList.h"

using std::invalid_argument;
using std::string;
using std::vector;

namespace MyShortList {
bool inList(ShortList& sl, short i);
void shortIntersection(ShortList& dest, ShortList& sour1, ShortList& sour2);
bool equal(ShortList& l1, ShortList& l2);
}; // namespace MyShortList

namespace ToInt {
int  myStoi(string word);
bool xtoiFit(const string& word, int num);
} // namespace ToInt

namespace MainArgsHandle {
bool           inVector(vector<string> argv, const string& str);
vector<string> charListToVector(int argc, char* args[]);
bool           inCharList(int argc, char* argv[], const string& str);
} // namespace MainArgsHandle

string toChinese(const string& word);
string toChinese(const bool word);

#endif // _MAINARGSHANDLE_H_
