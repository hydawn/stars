#pragma once
#include <vector>
#include <stdexcept>
#include <iostream>
#include <string>
#include "shortList.h"

using std::cout;
using std::endl;
using std::runtime_error;
using std::string;
using std::vector;

class RouteNode {
public:
	short data;
	bool print;
	RouteNode* prev;
	vector<RouteNode*> next;

	RouteNode() : data(0), print(true), prev(nullptr), next(vector<RouteNode*>()){}

	bool	   listNextIs(vector<RouteNode*>& list, int data);
	bool	   masked(vector<RouteNode*>& list);  // if all of list isn't printable
	bool	   hasNext();
	void	   maskFlag(int flag); // mask those marked if match the flag
	void	   resetMask();
};

typedef vector<RouteNode*>::iterator vRi;

class RouteTree {
private:
public:
	RouteNode* head;
	RouteNode* crnt;
	static long long branches;
	RouteTree() { init(); }
	~RouteTree() { free(head); }

    // check
	bool match() { return head == crnt; }

	// init
	void	   free(RouteNode* node);
	void	   free(vector<RouteNode*> list);
	void	   clear();
	void	   init();

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
	RouteNode* erase() { crnt = erase(crnt); }
	RouteNode* erase(RouteNode* node);
        // if node == crnt, return crnt
        // if has next node, return next node
        // else if has prev, return prev
        // else return nullptr
	// void add(RouteNode& node);

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

	// org
	// void organize();
	// void organize(RouteNode* node);
};
