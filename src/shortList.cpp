#include "shortList.h"

namespace MyShortList {
	bool inList(ShortList& sl, short i) {
		for (short j : sl)
			if (j == i)
				return true;
		return false;
	}

	void shortIntersection(ShortList& dest, ShortList& sour1, ShortList& sour2) {
		dest.clear();
		if(sour1.empty())
			return;
		for (short i : sour1) {
			if (inList(sour2, i))
				dest.push_back(i);
		}
	}

	bool equal(ShortList& l1, ShortList& l2) {
		if (l1.size()!=l2.size())
			return false;
		for (short i = 0; i < l1.size();++i)
			if (l1[i]!=l2[i])
				return false;
		return true;
	}
};
