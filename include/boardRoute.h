#ifndef _BOARDROUTE_H_
#define _BOARDROUTE_H_ 1
#include <vector>
#include <stdexcept>
#include <iostream>
#include "shortList.h"

using std::cout;
using std::endl;
using std::runtime_error;
using std::vector;

class RouteNode {
public:
	short data;
	bool print;
	RouteNode* prev;
	vector<RouteNode*> next;

	RouteNode() : data(0), print(true), prev(nullptr), next(vector<RouteNode*>()) {}
	RouteNode(const RouteNode& rn) { clone(rn); }

	void clone(const RouteNode& rn);

	bool	   listNextIs(vector<RouteNode*>& list, int data);
	bool	   masked(vector<RouteNode*>& list);  // if all of list isn't printable
	bool	   hasNext();
	void	   maskFlag(int flag); // mask those marked if match the flag
	void	   resetMask();
};

typedef vector<RouteNode*>::iterator vRi;

class RouteTree {
public:
	RouteNode* head;
	RouteNode* crnt;
	int	badNode;
	int	goodNode;
	int	freeNode;
	static long long branches;
	RouteTree() : badNode(0), goodNode(-1), freeNode(-2) { generate(); }
	RouteTree(const RouteTree& rt);
	~RouteTree() { free(head); }

	// check
	bool match() { return head == crnt; }

	// handle
	void clear();
	void generate();
	void free(RouteNode* node);
	void free(vector<RouteNode*> list);

	// move
	void forward();
	void forward(short data);
    // next node in the same level
	void nextNode();
	void backward();
    RouteNode* fastBackward(RouteNode* node, int num);

    // change
	void add(short data);
	void add(ShortList& list);

    // related to show
	void showRoute();	  // for boardAnalyse
	void showRoute(int flag);	  // for boardAnalyse
	bool showRoute(RouteNode* node, int level);
	void show(RouteNode* node, int level);
	void showAll() { show(head, 0); }
	void showCurrent() { show(crnt, 0); }

	// counter
	int getBranches(int flag=1);
	void branchCounter(RouteNode* node);
};

#endif
