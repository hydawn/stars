#ifndef _MAINARGSHANDLE_H_
#define _MAINARGSHANDLE_H_ 1
#include <string>
#include <vector>
#include "json/json.h"


using std::invalid_argument;
using std::string;
using std::vector;


namespace MyShortList {
bool inList(vector<int>&, int);
void shortIntersection(vector<int>& dest, vector<int>& sour1, vector<int>& sour2);
void copy(vector<int>&, const Json::Value&);
} // namespace MyShortList



namespace MyJson{
Json::Value trans(const vector<int>&);
} // namespace MyJson


namespace ToInt {
int  myStoi(const string word);
bool xtoiFit(const string& word, int num);
} // namespace ToInt


namespace MainArgsHandle {
bool           inVector(const vector<string>& argv, const string& str);
vector<string> charListToVector(int argc, char* args[]);
} // namespace MainArgsHandle


string toChinese(const string& word);
string toChinese(const bool word);


#endif // _MAINARGSHANDLE_H_