#ifndef _BOARDROUTE_H_
#define _BOARDROUTE_H_ 1
#include <vector>
#include <string>
#include <stdexcept>
#include <iostream>

using std::cout;
using std::endl;
using std::runtime_error;
using std::string;
using std::vector;

const int goodNode = -1;
const int badNode  = 0;
const int freeNode = -2;

class RouteTree;
class BoardInterface;

class RouteNode {
	friend class RouteTree;
	friend class BoardInterface;

private:
	RouteNode*         prev = nullptr;
	vector<RouteNode*> next;
	int                data  = 0;
	bool               print = true;

public:
	RouteNode() {}
	RouteNode(RouteNode* prev_, const int data_)
		: prev(prev_), next(vector<RouteNode*>()), data(data_), print(true) {}
	RouteNode(const RouteNode& rn) { clone(rn); }

	void clone(const RouteNode& rn);

	bool listNextIs(const vector<RouteNode*>& list, const int data) const;
	bool masked(const vector<RouteNode*>& list) const; // if all of list isn't printable
	bool hasNext() const;
	void maskFlag(const int flag); // mask those marked if match the flag
	void resetMask();
};


class RouteTree {
	friend class BoardInterface;

private:
	RouteNode* head;
	RouteNode* crnt;
	static int branches;

public:
	RouteTree() { generate(); }
	RouteTree(const RouteTree& rt);
	~RouteTree() { free(head); }

#ifdef STARS_DEBUG_INFO
	// check
	bool match() const { return head == crnt; }
#endif

	// handle
	void clear();
	void generate();
	void free(RouteNode* node);
	void free(vector<RouteNode*> list);

	// move
	void       forward();
	void       forward(const int data);
	void       nextNode(); // next node in the same level
	void       backward();
	RouteNode* fastBackward(RouteNode* node, const int num);

	// change
	void add(const int data);
	void add(const vector<int>& list);

	// related to show
	void showRoute();               // for boardAnalyse
	void showRoute(const int flag); // for boardAnalyse
	bool showRoute(RouteNode* node, const int level);
	void show(RouteNode* node, const int level);
	void showAll() { show(head, 0); }
	void showCurrent() { show(crnt, 0); }

	// counter
	int  getBranches(const int flag = 1);
	void branchCounter(const RouteNode* node) const;
};

#endif // _BOARDROUTE_H_
