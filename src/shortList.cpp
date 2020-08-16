#include "shortList.h"

namespace MyShortList {
	bool inList(ShortList& sl, short i) {
		for (short j : sl)
			if (j == i)
				return true;
		return false;
	}
	// void sortList(ShortList& sl);
	// void cpyList(ShortList& destination, ShortList& source);
	// void mergeList(ShortList& destination, ShortList& source1, ShortList& source2);
	void shortIntersection(ShortList& dest, ShortList& sour1, ShortList& sour2) {
		dest.clear();
		if(sour1.empty())
			return;
		for (short i : sour1) {
			if (inList(sour2, i))
				dest.push_back(i);
		}
	}
};
