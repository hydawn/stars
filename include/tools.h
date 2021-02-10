#ifndef _MY_TOOLS_H_
#define _MY_TOOLS_H_ 1
#include <string>
#include <vector>
#include "json/json.h"


using std::invalid_argument;
using std::string;
using std::vector;


namespace MyShortList {
bool inList(vector<int>&, int);
void shortIntersection(vector<int>& dst, vector<int>& src1, vector<int>& src2);
void copy(vector<int>&, const Json::Value&);
} // namespace MyShortList


namespace MyJson {
Json::Value toValue(const vector<int>&);
vector<int> toVectorInt(const Json::Value&);
} // namespace MyJson


namespace ToInt {
int  myStoi(const string& word);
bool xtoiFit(const string& word, const int num);
} // namespace ToInt


namespace MainArgsHandle {
bool           inVector(const vector<string>& argv, const string& str);
vector<string> charListToVector(int argc, char* args[]);
} // namespace MainArgsHandle


string toChinese(const string& word);
string toChinese(const bool word);


#endif // _MY_TOOLS_H_