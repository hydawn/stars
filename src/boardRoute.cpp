#include "boardRoute.h"

using std::logic_error;

int RouteTree::branches = 0;

void RouteNode::clone(const RouteNode& rn) {
	data  = rn.data;
	print = rn.print;
	if (rn.prev)
		prev = new RouteNode(*rn.prev);
	else
		prev = nullptr;
	for (RouteNode* i : rn.next) {
		RouteNode* temp = new RouteNode(*i);
		next.push_back(temp);
	}
}

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
#ifdef STARS_DEBUG_INFO
	if (iter == prev->next.end())
		throw logic_error("error RouteNode::hasNext");
#endif
	++iter;
	if (iter == prev->next.end())
		return false;
	for (vRi save = iter; save != prev->next.end(); ++save) {
		if ((*save)->print)
			return true;
	}
	return false;
}

// flag = 1, 0, -1, -2. 1 is for those that are not considered
void RouteNode::maskFlag(int flag) {
	if (flag == 1) {
		// mask those not considered
		if (!(data == goodNode || data == badNode || data == freeNode) &&
			next.empty()) {
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
		print       = false;
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

RouteTree::RouteTree(const RouteTree& rt) : crnt(rt.crnt) {
	head = new RouteNode(*rt.head);
}

void RouteTree::free(RouteNode* node) {
	if (node) {
		free(node->next);
		delete node;
	}
}

void RouteTree::free(vector<RouteNode*> list) {
	for (vRi iter = list.begin(); iter != list.end(); ++iter)
		free(*iter);
}

void RouteTree::generate() {
	head        = new RouteNode;
	head->data  = 0;
	head->print = true;
	head->prev  = nullptr;
	crnt        = head;
}

void RouteTree::clear() {
	free(head);
	generate();
}

void RouteTree::forward() {
#ifdef STARS_DEBUG_INFO
	if (crnt->next.size() != 1)
		throw logic_error("reached the end of the tree");
#endif
	crnt = crnt->next[0];
}

void RouteTree::forward(short data) {
#ifdef STARS_DEBUG_INFO
	if (crnt->next.empty())
		throw logic_error("reached the end of the tree");
#endif
	vector<RouteNode*>::iterator iter = crnt->next.begin();
	for (; iter != crnt->next.end(); ++iter)
		if ((*iter)->data == data) {
			crnt = *iter;
			return;
		}
#ifdef STARS_DEBUG_INFO
	throw logic_error("no such data in the next set of node");
#endif
}

void RouteTree::nextNode() {
#ifdef STARS_DEBUG_INFO
	if (!crnt->prev)
		throw logic_error("trying nextNode on head node");
	if (crnt->prev->next.empty())
		throw logic_error("reached the end of the tree");
#endif
	vRi iter = crnt->prev->next.begin();
	for (; iter != crnt->prev->next.end() && *iter != crnt; ++iter)
		;
#ifdef STARS_DEBUG_INFO
	if (iter == crnt->prev->next.end())
		throw logic_error("no such data, bugs occurred inside this class");
#endif
	++iter;
	if (iter == crnt->prev->next.end())
		return;
	crnt = *iter;
}

void RouteTree::backward() {
#ifdef STARS_DEBUG_INFO
	if (!crnt->prev)
		throw logic_error("reached the top of the tree");
#endif
	crnt = crnt->prev;
}

RouteNode* RouteTree::fastBackward(RouteNode* node, int num) {
#ifdef STARS_DEBUG_INFO
	if (num < 0)
		throw logic_error("invalid argument");
#endif
	for (int i = 0; i < num; ++i) {
#ifdef STARS_DEBUG_INFO
		if (!node->prev)
			throw logic_error("fastBackward \"overhead\"");
#endif
		node = node->prev;
	}
	return node;
}

void RouteTree::add(short data) {
	RouteNode* p = new RouteNode(crnt, data);
	crnt->next.push_back(p);
}

void RouteTree::add(vector<int>& list) {
	for (int i : list)
		add(i);
}

void RouteTree::showRoute(int flag) {
	if (flag == goodNode) { // good
		crnt->maskFlag(freeNode);
		crnt->maskFlag(badNode);
		crnt->maskFlag(1);
	}
	else if (flag == badNode) { // bad
		crnt->maskFlag(freeNode);
		crnt->maskFlag(goodNode);
		crnt->maskFlag(1);
	}
	else if (flag == freeNode) { // free
		crnt->maskFlag(goodNode);
		crnt->maskFlag(badNode);
		crnt->maskFlag(1);
	}
	showRoute();
	if (flag != 1)
		crnt->resetMask();
}

void RouteTree::showRoute() {
	for (RouteNode* i : crnt->next) {
		if (showRoute(i, 0))
			cout << endl;
	}
}

#ifdef STARS_PLATFORM_WINDOWS
string twoDot   = "-- ";
string oneDot   = "-";
string vertical = "|";
#elif STARS_PLATFORM_LINUX
string twoDot   = "\u2500\u2500 ";
string oneDot   = "\u2514";
string vertical = "\u2502";
#endif // STARS_PLATFORM
bool RouteTree::showRoute(RouteNode* node, int level) {
	if (!node->print)
		return false;
	if (node->next.empty()) {
		switch (node->data) {
			case badNode:
				cout << "bad";
				break;
			case goodNode:
				cout << "good";
				break;
			case freeNode:
				cout << "free";
				break;
			default:
				cout << node->data << twoDot << "not considered";
		}
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
			cout << twoDot;
			showRoute(*iter, level + 1);
			firstPrinted = true;
			continue;
		}
		for (int i = 0; i < level; ++i) {
			if (fastBackward(*iter, level - i)->hasNext())
				cout << vertical << "   ";
			else
				cout << ' ' << "   ";
		}
		if ((*iter)->hasNext())
			cout << vertical;
		else
			cout << oneDot;
		cout << twoDot;
		showRoute(*iter, level + 1);
	}
	return firstPrinted;
}

void RouteTree::show(RouteNode* node, int level) {
	cout << node->data;
	if (node->next.empty()) {
		cout << endl;
		return;
	}

	cout << ' ' << twoDot;
	show(node->next[0], level + 1);
	vRi iter = node->next.begin();
	for (++iter; iter != node->next.end(); ++iter) {
		for (int i = 0; i < level; ++i) {
			if (fastBackward(*iter, level - i)->hasNext())
				cout << vertical << "    ";
			else
				cout << ' ' << "    ";
		}
		cout << vertical << ' ' << twoDot;
		show(*iter, level + 1);
	}
}

int RouteTree::getBranches(int flag) {
	if (flag == goodNode) {
		crnt->maskFlag(freeNode);
		crnt->maskFlag(badNode);
	}
	else if (flag == badNode) {
		crnt->maskFlag(freeNode);
		crnt->maskFlag(goodNode);
	}
	else if (flag == freeNode) {
		crnt->maskFlag(goodNode);
		crnt->maskFlag(badNode);
	}
	branches = 0;
	branchCounter(crnt);
	if (flag != 1)
		crnt->resetMask();
	return branches;
}

void RouteTree::branchCounter(const RouteNode* node) {
	if (node->next.empty()) {
		if (node->print && node->prev && node->prev->next.size() == 1)
			++branches;
	}
	else
		for (const RouteNode* i : node->next)
			branchCounter(i);
}
