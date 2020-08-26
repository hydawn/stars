#ifndef _SHORTLIST_H_GUARD_
#define _SHORTLIST_H_GUARD_ 1

#include "json/json.h"

#define SHORTV_LENGTH 32
using std::logic_error;

// some of this code can be improved - like short erase
class ShortList {
private:
	short data[SHORTV_LENGTH];
	short top;

public:
	ShortList() { top = 0; }
	ShortList(const Json::Value root) {
		for (unsigned short i = 0; i < root.size(); ++i)
			data[i] = root[i].asInt();
		top = root.size();
	}

	typedef short* iterator;

	void push_back(const short& ele) {
#ifdef STARS_DEBUG_INFO
		if (top == SHORTV_LENGTH)
			throw logic_error("the end of ShortList, can't be pushed back further more");
#endif
		data[top++] = ele;
	}
	bool  empty() const { return top == 0; }
	short size() const { return top; }

	void clear() { top = 0; }

	iterator begin() { return data; }
	iterator end() { return data + top; }
	iterator erase(iterator iter) {
		iterator saveIter = iter, lastIter = iter++;
		while (iter != end()) {
			*lastIter = *iter;
			++iter;
			++lastIter;
		}
		top -= 1;
		return saveIter;
	}
	void del(short iter) {
		for (short i = 0; i < top; ++i)
			if (data[i] == iter) {
				for (short j = i + 1; j < top; ++j)
					data[j - 1] = data[j];
				top -= 1;
				return;
			}
#ifdef STARS_DEBUG_INFO
		throw logic_error("trying to erase an item that does not exist");
#endif
	}

	// op
	const short& operator[](const short index) const { return data[index]; }
	short& operator[](const short index) { return data[index]; }
	operator Json::Value() {
	Json::Value root;
	for (short i = 0; i < top; ++i)
		root.append(data[i]);
	return root;
	}
};

namespace MyShortList {
	bool inList(ShortList& sl, short i);
	void shortIntersection(ShortList& dest, ShortList& sour1, ShortList& sour2);
	bool equal(ShortList& l1, ShortList& l2);
};

#endif
