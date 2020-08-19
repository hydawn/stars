#include "boardRoute.h"

long long RouteTree::branches = 0;

bool RouteNode::listNextIs(vector<RouteNode*>& list, int data) {
	for (RouteNode* iter : list) {
		if (iter->next.size() != 1)
			return false;
		if (iter->next[0]->data != data)
			return false;
	}
	return true;
}

bool RouteNode::masked(vector<RouteNode*>& list) {
	if (list.empty())
		return false;
	for (RouteNode* i : list)
		if (i->print)
			return false;
	return true;
}

bool RouteNode::hasNext() {
	if (!prev)
		return false;
	vRi iter = prev->next.begin();
	for (; iter != prev->next.end() && *iter != this; ++iter)
		;
	if (iter == prev->next.end())
		throw runtime_error("error inside the code");
	++iter;
	if (iter == prev->next.end())
		return false;
	// bugs here
	for (vRi save = iter; save != prev->next.end();++save) {
		if ((*save)->print)
			return true;
	}
	return false;
}

// flag = 1, 0, -1, -2. 1 is for those that are not considered
void RouteNode::maskFlag(int flag) {
	if (flag == 1) { // mask those not considered
		if (!(data == 0 || data == -1 || data == -2) && next.empty()) {
			print = false;
			return;
		}
		for (RouteNode* i : next)
			i->maskFlag(flag);
		if (masked(next))
			print = false;
		return;
	}
	if (data == flag && prev && prev->next.size() == 1 && next.empty()) {
		prev->print = false;
		print = false;
		return;
	}

	if (next.empty())
		return;
	for (RouteNode* i : next)
		i->maskFlag(flag);

	if (!prev)
		return;
	if (masked(next))
		print = false;
}

void RouteNode::resetMask() {
	print = true;
	for (RouteNode* i : next)
		i->resetMask();
}

void RouteTree::free(RouteNode* node) {
	free(node->next);
	delete node;
}

void RouteTree::free(vector<RouteNode*> list) {
	for (vRi iter = list.begin(); iter != list.end(); ++iter)
		free(*iter);
}

void RouteTree::init() {
	head		= new RouteNode;
	head->data	= 0;
	head->print = true;
	head->prev	= nullptr;
	crnt		= head;
}

void RouteTree::clear() {
	free(head);
	init();
}

void RouteTree::forward() {
	if (crnt->next.size() != 1)
		throw runtime_error("reached the end of the tree!\n");
	crnt = crnt->next[0];
}

void RouteTree::forward(short data) {
	if (crnt->next.empty())
		throw runtime_error("reached the end of the tree!\n");
	// vRi iter = std::find_if(crnt->next.begin(), crnt->next.end(), tempComp);
	vector<RouteNode *>::iterator iter = crnt->next.begin();
	for (; iter != crnt->next.end();++iter)
		if ((*iter)->data == data) {
			crnt = *iter;
			return;
		}
	throw runtime_error("no such data in the next set of node\n");
}

void RouteTree::nextNode() {
	if (crnt->prev->next.empty())
		throw runtime_error("reached the end of the tree!\n");
	// vRi iter = std::find(crnt->prev->next.begin(), crnt->prev->next.end(), crnt);
	vRi iter = crnt->prev->next.begin();
	for (; iter != crnt->prev->next.end() && *iter != crnt;++iter)
		;
	if (iter == crnt->prev->next.end())
		throw runtime_error("no such data, bugs occurred inside this class\n");
	++iter;
	if (iter == crnt->prev->next.end())
		return;
	crnt = *iter;
}

void RouteTree::backward() {
	if (!crnt->prev)
		throw runtime_error("reached the top of the tree!\n");
	crnt = crnt->prev;
}

RouteNode* RouteTree::fastBackward(RouteNode* node, int num) {
	if (num < 0)
		throw runtime_error("invalid argument");
	for (int i = 0; i < num; ++i) {
		if (!node->prev)
			throw runtime_error("fastBackward \"overhead\"");
		node = node->prev;
	}
	return node;
}

void RouteTree::add(short data) {
	RouteNode* p = new RouteNode;
	p->data		 = data;
	p->prev		 = crnt;
	p->print	 = true;
	crnt->next.push_back(p);
}

void RouteTree::add(ShortList& list) {
	for (ShortList::iterator iter = list.begin(); iter != list.end(); ++iter)
		add(*iter);
}

void RouteTree::showRoute() {
	for (RouteNode* i : head->next) {
		if (showRoute(i, 0))
			cout << endl;
	}
}

void RouteTree::showRoute(int flag) {
	if (flag == -1) { // good
		head->maskFlag(-2);
		head->maskFlag(0);
		head->maskFlag(1);
	}
	else if (flag == 0) { // bad
		head->maskFlag(-2);
		head->maskFlag(-1);
		head->maskFlag(1);
	}
	else if (flag == -2) { // free
		head->maskFlag(-1);
		head->maskFlag(0);
		head->maskFlag(1);
	}
	showRoute();
	if (flag != 1)
		head->resetMask();
}

bool RouteTree::showRoute(RouteNode* node, int level) {
	if (!node->print)
		return false;
	if (node->next.empty()) {
		if (node->data == 0)
			cout << "bad";
		else if (node->data == -1)
			cout << "good";
		else if (node->data == -2)
			cout << "free";
		else
			cout << node->data
				//  << "\u2500\u2500 not considered";
				 << "-- not considered";
		cout << endl;
		return true;
	}
	else
		cout << node->data;

	bool firstPrinted = false;
	for (vRi iter = node->next.begin(); iter != node->next.end(); ++iter) {
		if (!(*iter)->print)
			continue;
		if (!firstPrinted) { // print the first element
			// cout << "\u2500\u2500 ";
			cout << "-- ";
			showRoute(*iter, level + 1);
			firstPrinted = true;
			continue;
		}
		for (int i = 0; i < level; ++i) {
			if (fastBackward(*iter, level - i)->hasNext())
				// cout << "\u2502" << "   ";
				cout << "|" << "   ";
			else
				cout << ' ' << "   ";
		}
		if ((*iter)->hasNext())
			// cout << "\u251C";
			cout << "|";
		else
			// cout << "\u2514";
			cout << "-";
		cout << "-- ";
		// cout << "\u2500\u2500 ";
		showRoute(*iter, level+1);
	}
	return firstPrinted;
}

void RouteTree::show(RouteNode* node, int level)  {
	cout << node->data;
	if (node->next.empty()) {
		cout << endl;
		return;
	}

	cout << " -- ";
	show(node->next[0], level+1);
	vRi iter = node->next.begin();
	for (++iter; iter != node->next.end(); ++iter) {
		for (int i = 0; i < level; ++i) {
			if (fastBackward(*iter, level - i)->hasNext())
				cout << '|' << "    ";
			else
				cout << ' ' << "    ";
		}
		cout << '|';
		cout << " -- ";
		show(*iter, level+1);
	}
}
/*
0    1    2    3
3 -- 5 -- 3 -- 6 -- 7
|    |         | -- 8
|    |         | -- 12
|    | -- 4 -- 3
|    |    | -- 8
|    | -- 9 -- 78
|    | -- 2 -- 65
|    |    | -- 1
|    | -- 0 -- 8
- -- 6
*/

int RouteTree::getBranches(int flag) {
	if (flag == -1) {
		head->maskFlag(-2);
		head->maskFlag(0);
	}
	else if (flag == 0) {
		head->maskFlag(-2);
		head->maskFlag(-1);
	}
	else if (flag == -2) {
		head->maskFlag(-1);
		head->maskFlag(0);
	}
	branches = 0;
	branchCounter(head);
	if (flag != 1)
		head->resetMask();
	return branches;
}

void RouteTree::branchCounter(RouteNode* node) {
	if (node->next.empty()) {
		if (node->print && node->prev && node->prev->next.size() == 1)
			++branches;
	}
	else
		for (RouteNode* i : node->next)
			branchCounter(i);
}
