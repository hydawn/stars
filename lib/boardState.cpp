#include "boardState.h"

int BoardState::removeNumber;
int BoardState::addNumber;

BoardState::BoardState(const Json::Value& root)
	: row(root["row"].asInt()), column(root["column"].asInt()), winn(root["winn"].asInt()) {
	starsOn = false;
	generate();
	for (short i = 0; i < column; ++i) {
		strcpy(board[i], root["board"][i].asCString());
		top[i] = root["top"][i].asInt();
	}
}

void BoardState::generate() {
	// TODO - is quite stupid, change this when have the chance
	// it's best to use allocate?
	board	 = new char*[column];
	top		 = new short[column];
	starArea = new short[column];
	for (short i = 0; i < column; ++i) {
		top[i]		= 0;
		starArea[i] = row;
		board[i]	= new char[row + 1];
		for (short j = 0; j < row; ++j)
			board[i][j] = ' ';
	}
}

void BoardState::generate(char** b, const short* t) {
	board	= new char*[column];
	top		= new short[column];
	starArea = new short[column];
	for (short i = 0; i < column; ++i) {
		board[i]   = new char[row + 1];
		top[i]	   = t[i];
		starArea[i] = row;
	}
	refreshBoard(b);
}

void BoardState::free() {
	for (short i = 0; i < column; ++i)
		delete[] board[i];
	delete[] board;
	delete[] top;
	delete[] starArea;
}

BoardState& BoardState::operator=(const BoardState& bh) {
	if (this == &bh)
		return *this;
	free();
	column	= bh.column;
	row		= bh.row;
	winn	= bh.winn;
	starsOn = bh.starsOn;
	generate(bh.board, bh.top);
	return *this;
}

Json::Value BoardState::boardToJson() {
	Json::Value root;
	for (short i = 0; i < column; ++i) {
		board[i][row] = '\0';
		root.append(board[i]);
	}
	return root;
}

Json::Value BoardState::topToJson() { return arraryToJson(top, column); }

template <typename T>
Json::Value BoardState::arraryToJson(T a[], int n) {
	Json::Value root;
	for (int i = 0; i < n; ++i)
		root.append(a[i]);
	return root;
}

void BoardState::show() {
	short i = 1;
	for (; i <= column && i <= 10; ++i)
		printf(" %d", i);
	for (; i <= column; ++i)
		printf("%d", i);
	printf("\n");
	for (short i = row - 1; i > -1; --i) {
		for (short j = 0; j < column; ++j)
			printf("|%c", board[j][i]);
		printf("|\n");
	}
}

bool BoardState::boardIsFull() {
	// static int timer = 0;
	// ++timer;
	// if (timer > 38815)
	//     printf("isOver %d ", timer);
	for (short i = 1; i <= column; ++i)
		if (!colIsFull(i))
			return false;
	return true;
}

char BoardState::isOver() {
	// static int timer = 0;
	// ++timer;
	// if (timer > 5000)
	//     printf("isOver %d ", timer);
	// if (boardIsFull())
	//     return 'F';
	// check if there is winn in line
	for (short i = 0; i < column; ++i)
		for (short j = 0; j < top[i]; ++j)
			// assume that there is only one winn in line in the board
			if (winPieceNearBy(i, j)) {
				// printf("debug - column=%d, row=%d\n", i, j);
				return board[i][j];
			}
	return 'N';
}

void BoardState::nonFullColumn(shortv& nonFull) {
	nonFull.clear();  // see if delete this will help?
	for (short i = 0; i < column; ++i)
		if (top[i] != starArea[i])
			nonFull.push_back(i + 1);
	// static int timer = 0;
	// printf("nonfull %d ", ++timer);
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
	if (list.empty())
		throw runtime_error("trying randomMove() in an empty list\n");
	return randomMove(list);
}

short BoardState::randomMove(shortv& list) {
	// debug
	if (list.empty())
		throw runtime_error("trying randomMove(shortv& list) in an empty list\n");
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
	} else if (mode == "defensive") {
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
	} else
		throw runtime_error("no such mode.\n");
	if (list.empty())
		throw runtime_error("call randomSuggestion with empty list");
	return randomMove(list);
}

short BoardState::randomSuggestion(const char plr, shortv& list, shortv oppList, const string& mode) {
	shortv tempList;
	for (vIter i = list.begin(); i != list.end(); ++i)
		for (vIter j = oppList.begin(); j != oppList.end(); ++j)
			if (*j == *i) {
				tempList.push_back(*i);
				j = oppList.erase(j);
				break;
			}
	if (tempList.empty()) {
		if (list.empty())
			throw runtime_error("call randomSuggestion(4) with empty list");
		return randomSuggestion(plr, list, mode);
	}
	return randomSuggestion(plr, tempList, mode);
}

bool BoardState::winPieceNearBy(const short col, const short ro) {
	// grow up, right, upright, downright
	short i		  = 1;
	char  present = board[col][ro];
	bool  canUp = ro <= top[col] - winn, canRight = col <= column - winn, canDown = ro >= winn - 1;
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

void BoardState::refreshBoard(char** hb) {
	for (short i = 0; i < column; ++i)
		for (short j = 0; j < row; ++j)
			board[i][j] = hb[i][j];
}

void BoardState::refreshTop() {
	for (short i = 0; i < column; ++i) {
		short j = 0;
		for (; j < row && board[i][j] != ' '; ++j)
			;
		top[i] = j;
	}
}

void BoardState::customBoard(const short cl, const short ro, const short wi) {
	free();
	column = cl;
	row	   = ro;
	winn   = wi;
	generate();
}

void BoardState::areaTopTransform() {
	for (short i = 0; i < column; ++i)
		starArea[i] = 0;
	for (short i = 0; i < column; ++i)
		setATopWithTop(i, top[i]);

	// check for bulks of piece
	for (short i = 0; i < column - 1;++i){
		if (top[i] && top[i+1]) {
			setATopWithNumber(i + 3, 1);
			setATopWithNumber(i - 2, 1);
		}
	}

		// then check for overflow
		for (short i = 0; i < column; ++i)
			if (starArea[i] > row)
				starArea[i] = row;
}

void BoardState::areaTopRestore() {
	for (short i = 0; i < column; ++i)
		starArea[i] = row;
}

void BoardState::starShow() {
	areaTopTransform();
	short i = 1;
	for (; i <= column && i <= 10; ++i)
		printf(" %d", i);
	for (; i <= column; ++i)
		printf("%d", i);
	printf("\n");
	for (short i = row - 1; i > -1; --i) {
		for (short j = 0; j < column; ++j) {
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
	setATopWithNumber(i - 2, t / 2);
	setATopWithNumber(i - 1, t + 1);
	setATopWithNumber(i, t + 2);
	setATopWithNumber(i + 1, t + 1);
	setATopWithNumber(i + 2, t / 2);
}

shortv BoardState::aTopFullColumn() {
	areaTopTransform();
	shortv list;
	for (short i = 0; i < column;++i)
		if (starArea[i]==0)
			list.push_back(i + 1);
	return list;
}
