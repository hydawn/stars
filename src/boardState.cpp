#include "boardState.h"

#ifdef STARS_DEBUG_INFO
int BoardState::removeNumber;
int BoardState::addNumber;
#endif

BoardState::BoardState(const Json::Value& root)
	: rows(root["row"].asInt()), cols(root["column"].asInt()), winn(root["winn"].asInt()) {
	generate();
	for (short i = 0; i < cols; ++i) {
		strcpy(board[i], root["board"][i].asCString());
		top[i] = root["top"][i].asInt();
	}
}

void BoardState::generate() {
	board	 = new char*[cols];
	top		 = new short[cols];
	starArea = new short[cols];
	for (short i = 0; i < cols; ++i) {
		top[i]		= 0;
		starArea[i] = rows;
		board[i]	= new char[rows + 1];
		for (short j = 0; j < rows; ++j)
			board[i][j] = ' ';
	}
}

void BoardState::generate(char** b, const short* t) {
	board	 = new char*[cols];
	top		 = new short[cols];
	starArea = new short[cols];
	for (short i = 0; i < cols; ++i) {
		board[i]	= new char[rows + 1];
		top[i]		= t[i];
		starArea[i] = rows;
	}
	refreshBoard(b);
}

void BoardState::free() {
	freeBoard(board, cols);
	delete[] top;
	delete[] starArea;
}

void BoardState::freeBoard(char** b, int length) {
	for (short i = 0; i < length; ++i)
		delete[] b[i];
	delete[] b;
}

BoardState& BoardState::operator=(const BoardState& bh) {
	if (this == &bh)
		return *this;
	free();
	cols = bh.cols;
	rows = bh.rows;
	winn = bh.winn;
	generate(bh.board, bh.top);
	return *this;
}

Json::Value BoardState::boardToJson() {
	Json::Value root;
	for (short i = 0; i < cols; ++i) {
		board[i][rows] = '\0';
		root.append(board[i]);
	}
	return root;
}

Json::Value BoardState::topToJson() { return arraryToJson(top, cols); }

template <typename T>
Json::Value BoardState::arraryToJson(T a[], int n) {
	Json::Value root;
	for (int i = 0; i < n; ++i)
		root.append(a[i]);
	return root;
}

void BoardState::show() {
	short i = 1;
	printHead();
	for (short i = rows - 1; i > -1; --i) {
		for (short j = 0; j < cols; ++j)
			printf("|%c", board[j][i]);
		printf("|\n");
	}
}

void BoardState::printHead() {
	for (int i = 1; i <= cols; ++i) {
		if (i <= 10)
			printf(" %d", i);
		else
			printf("%d", i);
	}
	printf("\n");
}

bool BoardState::boardIsFull() {
	for (short i = 1; i <= cols; ++i)
		if (!colIsFull(i))
			return false;
	return true;
}

char BoardState::isOver() {
	for (short i = 0; i < cols; ++i)
		for (short j = 0; j < top[i]; ++j)
			if (winPieceNearBy(i, j))
				return board[i][j];
	return 'N';
}

void BoardState::nonFullColumn(shortv& nonFull) {
	nonFull.clear();
	for (short i = 0; i < cols; ++i)
		if (top[i] != starArea[i])
			nonFull.push_back(i + 1);
}

void BoardState::sweepFullColumn(shortv& nonFull, short col) {
	if (top[col - 1] == starArea[col - 1])
		nonFull.del(col);
	shortv realNonFull;
	nonFullColumn(realNonFull);
#ifdef STARS_DEBUG_INFO
	if (!MyShortList::equal(nonFull, realNonFull))
		throw logic_error("this non-full and the real non-full didn't match\n");
#endif
}

char BoardState::rPlayer(const char plr) {
	if (plr == 'X')
		return '0';
	if (plr == '0')
		return 'X';
	throw runtime_error("No such player exist!\n");
}

short BoardState::randomMove() {
	shortv list;
	nonFullColumn(list);
#ifdef STARS_DEBUG_INFO
	if (list.empty())
		throw logic_error("trying randomMove() in an empty list\n");
#endif	// STARS_DEBUG_INFO
	return randomMove(list);
}

short BoardState::randomMove(shortv& list) {
#ifdef STARS_DEBUG_INFO
	if (list.empty())
		throw logic_error("trying randomMove(shortv& list) in an empty list\n");
#endif	// STARS_DEBUG_INFO
	srand((unsigned)time(NULL));
	return list[rand() % list.size()];
}

short BoardState::randomSuggestion(const char plr, shortv& list, const string& mode) {
	shortv opp2, opp1, plr2, plr1;
	char   opp = rPlayer(plr);
	srand((unsigned)time(NULL));
	for (vIter col = list.begin(); col != list.end(); ++col) {
		if (colIsFull(*col) || colIsEmpty(*col))
			;
		else if (board[*col - 1][top[*col - 1] - 1] == plr) {
			if (top[*col - 1] - 1 > 0 && board[*col - 1][top[*col - 1] - 2] == plr)
				plr2.push_back(*col);
			else
				plr1.push_back(*col);
		} else {
			if (top[*col - 1] - 1 > 0 && board[*col - 1][top[*col - 1] - 2] == opp)
				opp2.push_back(*col);
			else
				opp1.push_back(*col);
		}
	}
	short ran = rand() % 100;
	if (mode == "progressive") {
		if (!plr2.empty() && ran < 72)
			return randomMove(plr2);
		ran = rand() % 100;
		if (!opp2.empty() && ran < 95)
			return randomMove(opp2);
		ran = rand() % 100;
		if (!plr1.empty() && ran < 95)
			return randomMove(plr1);
		ran = rand() % 100;
		if (!opp1.empty() && ran < 57)
			return randomMove(opp1);
	}
	else if (mode == "defensive") {
		if (!opp2.empty() && ran < 95)
			return randomMove(opp2);
		ran = rand() % 100;
		if (!plr2.empty() && ran < 72)
			return randomMove(plr2);
		ran = rand() % 100;
		if (!opp1.empty() && ran < 77)
			return randomMove(opp1);
		ran = rand() % 100;
		if (!plr1.empty() && ran < 65)
			return randomMove(plr1);
	}
#ifdef STARS_DEBUG_INFO
	else
		throw logic_error("no such mode.\n");
	if (list.empty())
		throw logic_error("call randomSuggestion with empty list");
#endif // STARS_DEBUG_INFO
	return randomMove(list);
}

short BoardState::randomSuggestion(const char plr, shortv& list, shortv oppList, const string& mode) {
#ifdef STARS_DEBUG_INFO
	if (list.empty())
		throw logic_error("call randomSuggestion(4 args) with empty list");
#endif
	shortv intersectionList;
	// preference No.1: take what can bring me winn-1 in a row, and what can
	// interrupt opponent's three in a row
	// but some times it block itself which is rather stupid
	shortv plrTList = makeThreeCols(plr, list), oppTList = makeThreeCols(rPlayer(plr), oppList);
	srand((unsigned)time(NULL));
	if (rand() % 100 < 85) {
// #ifdef STARS_DEBUG_INFO
// 		printf("    Debug: Trying intercept strategy 1:\n");
// #endif
		MyShortList::shortIntersection(intersectionList, plrTList, oppTList);
		if (intersectionList.empty()) {
			if (plrTList.empty()) {
				MyShortList::shortIntersection(intersectionList, list, oppTList);
				if (!intersectionList.empty())
					return randomSuggestion(plr, intersectionList, mode);
			}
			else
				return randomSuggestion(plr, plrTList, mode);
		}
		else
			return randomSuggestion(plr, intersectionList, mode);
	}

	// else if everything is empty
	// preference No.2: take the opponent's safe list
	MyShortList::shortIntersection(intersectionList, list, oppList);
// #ifdef STARS_DEBUG_INFO
// 	printf("    Debug: Trying intercept strategy 2:\n");
// #endif
	if (intersectionList.empty())
		return randomSuggestion(plr, list, mode);
	return randomSuggestion(plr, intersectionList, mode);
}

#ifdef STARS_DEBUG_INFO
bool BoardState::match() {
	for (short i = 0; i < cols; ++i) {
		short j = 0;
		while (j < rows && (board[i][j] == 'X' || board[i][j] == '0'))
			++j;
		if (top[i] != j)
			return false;
	}
	return true;
}
#endif

bool BoardState::winPieceNearBy(const short col, const short ro) {
	// grow up, right, upright, downright
	short i		  = 1;
	char  present = board[col][ro];
	bool  canUp = ro <= top[col] - winn, canRight = col <= cols - winn, canDown = ro >= winn - 1;
	if (canRight) {
		// right
		for (i = 1; i < winn; ++i)
			if (board[col + i][ro] != present)
				break;
		if (i == winn)
			return true;
		// up & right
		for (i = 1; i < winn; ++i)
			if (board[col + i][ro + i] != present)
				break;
		if (i == winn)
			return true;
		if (canDown) {
			// down & right
			for (i = 1; i < winn; ++i)
				if (board[col + i][ro - i] != present)
					break;
			if (i == winn)
				return true;
		}
	}
	if (canUp) {
		// up
		for (i = 1; i < winn; ++i)
			if (board[col][ro + i] != present)
				return false;
		return true;
	}
	return false;
}

bool BoardState::winPieceButOne(const short col, const short ro, const short win) {
	// grow right, upright, downright
	short i			 = 1;
	bool  butOneMet  = false;
	char  present	 = board[col][ro];
	bool  canUp = ro <= top[col] - win, canRight = col <= cols - win, canDown = ro >= win - 1;
	if (!canRight)
		return false;
	// right
	for (i = 1; i < win; ++i)
		if (board[col + i][ro] != present && butOneMet)
			break;
		else if (board[col + i][ro] == ' ')
			butOneMet = true;
		else
			break;
	if (i == win)
		return true;
	if (canDown) {
		// down & right
		for (butOneMet = false, i = 1; i < win; ++i)
			if (board[col + i][ro - i] != present && butOneMet)
				break;
			else if (board[col + i][ro - i] == ' ')
				butOneMet = true;
			else
				break;
		if (i == win)
			return true;
	}
	// up & right
	for (butOneMet = false, i = 1; i < win; ++i)
		if (board[col + i][ro + i] != present && butOneMet)
			return false;
		else if (board[col + i][ro + i] == ' ')
			butOneMet = true;
		else
			return false;
	if (i == win)
		return true;
	return false;
}

void BoardState::refreshBoard(char** hb) {
	for (short i = 0; i < cols; ++i)
		for (short j = 0; j < rows; ++j)
			board[i][j] = hb[i][j];
}

void BoardState::refreshTop() {
	for (short i = 0; i < cols; ++i) {
		short j = 0;
		while (j < rows && board[i][j] != ' ')
			++j;
		top[i] = j;
	}
}

void BoardState::customBoard(const short cl, const short ro, const short wi) {
	free();
	cols = cl;
	rows = ro;
	winn = wi;
	generate();
}

void BoardState::areaTopTransform() {
	for (short i = 0; i < cols; ++i)
		starArea[i] = 0;
	for (short i = 0; i < cols; ++i)
		setATopWithTop(i, top[i]);

	// check for bulks of piece
	for (short i = 0; i < cols - 1;++i){
		if (top[i] && top[i+1]) {
			setATopWithNumber(i + 3, 1);
			setATopWithNumber(i - 2, 1);
		}
	}

	// then check for overflow
	for (short i = 0; i < cols; ++i)
		if (starArea[i] > rows)
			starArea[i] = rows;
	
	// check if stars are even
	int stars = starNumber();
	if(stars % 2 == 0)
		return;
	// randomly chose a non-full column and add it
	shortv starNotZero, starNotFull, inter;
	for (short i = 0; i < cols; ++i) {
		if (starArea[i])
			starNotFull.push_back(i + 1); // not full of stars(maybe doesn't have stars)
		if (rows != starArea[i])
			starNotZero.push_back(i + 1); // have stars but may be full of stars
	}
	MyShortList::shortIntersection(inter, starNotFull, starNotZero);
	if (inter.empty())
		++starArea[randomMove(starNotZero) - 1];
	else
		++starArea[randomMove(inter) - 1];
}

void BoardState::areaTopRestore() {
	for (short i = 0; i < cols; ++i)
		starArea[i] = rows;
}

void BoardState::starShow() {
	areaTopTransform();
	short i = 1;
	for (; i <= cols && i <= 10; ++i)
		printf(" %d", i);
	for (; i <= cols; ++i)
		printf("%d", i);
	printf("\n");
	for (short i = rows - 1; i > -1; --i) {
		for (short j = 0; j < cols; ++j) {
			if (i < starArea[j])
				printf("|%c", board[j][i]);
			else
				printf("|+");
		}
		printf("|\n");
	}
	areaTopRestore();
}

void BoardState::setATopWithTop(short i, short t) {
	// important numbers here!
	if (t == 0)
		return;
	setATopWithNumber(i - 2, t / 2 + 1);
	setATopWithNumber(i - 1, t + 1 > 4 ? t + 1 : 4);
	setATopWithNumber(i, t + 2);
	setATopWithNumber(i + 1, t + 1 > 4 ? t + 1 : 4);
	setATopWithNumber(i + 2, t / 2 + 1);
}

shortv BoardState::aTopFullColumn() {
	areaTopTransform();
	shortv list;
	for (short i = 0; i < cols;++i)
		if (starArea[i]==0)
			list.push_back(i + 1);
	return list;
}

int BoardState::starNumber() {
	int sum = 0;
	for (int i = 0; i < cols;++i)
		sum += rows - starArea[i];
	return sum;
}

// count how many three in a row or winn but one in a row is there in the board,
// just like isOver with the winn set to winn - 1
int BoardState::threeRowCount(const char plr, shortv& safeList) {
	int rax = 0;
	winn--;
	for (short i = 0; i < cols; ++i)
		for (short j = 0; j < top[i]; ++j)
			if (specialPiece(i, j) || winPieceButOne(i, j, winn + 1))
				++rax;
	winn++;
	return rax;
}

shortv BoardState::makeThreeCols(const char plr, shortv& safeList) {
	shortv rax;
	int	   counter;
	for (short i : safeList) {
		counter = threeRowCount(plr, safeList);
		add(plr, i);
		counter -= threeRowCount(plr, safeList);
		remove(i);
		if (counter < 0)
			rax.push_back(i);
	}
	return rax;
}

bool BoardState::specialPiece(const short col, const short ro) {
	// grow up, right, upright, downright
	short i		  = 1;
	char  present = board[col][ro];
	bool  canUp = ro <= top[col] - winn, canRight = col <= cols - winn, canDown = ro >= winn - 1;
	if (canRight) {
		// right
		for (i = 1; i < winn; ++i)
			if (board[col + i][ro] != present)
				break;
		if (i == winn && ((col + i < cols && board[col + i][ro] == ' ') ||
			(col > 0 && board[col - 1][ro] == ' ')))
			return true;
		// up & right
		for (i = 1; i < winn; ++i)
			if (board[col + i][ro + i] != present)
				break;
		if (i == winn && ((col + i < cols && ro + i < rows &&
			board[col + i][ro + i] == ' ') ||
			(col > 0 && ro > 0 && board[col - 1][ro - 1] == ' ')))
			return true;
		if (canDown) {
			// down & right
			for (i = 1; i < winn; ++i)
				if (board[col + i][ro - i] != present)
					break;
			if (i == winn && ((col + i < cols && ro - i >= 0 &&
				board[col + i][ro - i] == ' ') ||
				(col > 0 && ro + 1 <= rows && board[col - 1][ro + 1] == ' ')))
				return true;
		}
	}
	if (canUp) {
		// up
		for (i = 1; i < winn; ++i)
			if (board[col][ro + i] != present)
				return false;
		if ((ro > 0 && board[col][ro - 1] == ' ') || (ro + i < rows && board[col][ro + i] == ' '))
			return true;
	}
	return false;
}

void BoardState::retInit(vector<oneMove>& his) {
	// return the state to the 'initial state' accroding to the history move
	for (vector<oneMove>::reverse_iterator riter = his.rbegin();
		riter != his.rend();++riter) {
		if (riter->mode=="debug"||riter->mode=="add"||riter->mode=="normal")
			remove(riter->move);
		else if (riter->mode=="reverse")
			add(riter->player, riter->move);
#ifdef STARS_DEBUG_INFO
		else
			throw logic_error("unexpected, unhandled mode in retInit in boardState.h\n");
#endif	// STARS_DEBUG_INFO
	}
}
