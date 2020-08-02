#include "boardHandle.h"

int BoardHandle::removeNumber;
int BoardHandle::addNumber;

BoardHandle::BoardHandle(const Json::Value &root) : row(root["row"].asInt()),
    column(root["column"].asInt()), winn(root["winn"].asInt()) {
    generate();
    for (short i = 0; i < column;++i) {
        strcpy(board[i], root["board"][i].asCString());
        top[i] = root["top"][i].asInt();
    }
}

BoardHandle::BoardHandle(const short r, const short c, const short w) : 
    row(r), column(c), winn(w) {
    generate();
}

BoardHandle::~BoardHandle() {
    for (short i = 0; i < row; ++i)
        delete[] board[i];
    delete[] board;
    delete[] top;
}

void BoardHandle::generate() {
    // TODO - is quite stupid, change this when have the chance
    // it's best to use allocate?
    board = new char* [column];
    top = new short[column];
    for (short i = 0; i < column; ++i) {
        top[i] = 0;
        board[i] = new char[row + 1];
        for (short j = 0; j < row; ++j)
            board[i][j] = ' ';
    }
}

void BoardHandle::generate(char** b, const short* t) {
    board = new char* [column];
    top = new short[column];
    for (short i = 0; i < column; ++i) {
        board[i] = new char[row + 1];
        top[i] = t[i];
    }
    refreshBoard(b);
}

BoardHandle& BoardHandle::operator=(const BoardHandle &bh) {
    if(this==&bh)
        return *this;
    refreshBoard(bh.board);
    refreshTop();
    return *this;
}

Json::Value BoardHandle::boardToJson() {
    Json::Value root;
    for (short i = 0; i < column;++i){
        board[i][row] = '\0';
        // root.append(arraryToJson(board[i], row));
        root.append(board[i]);
    }
    return root;
}

Json::Value BoardHandle::topToJson() {
    return arraryToJson(top, column);
}

template<typename T>
Json::Value BoardHandle::arraryToJson(T a[], int n){
    Json::Value root;
    for (int i = 0; i < n;++i)
        root.append(a[i]);
    return root;
}

void BoardHandle::show() {
    for (short i = 1; i <= column; ++i)
        printf(" %d", i);
    printf("\n");
    for (short i = row - 1; i > -1; --i) {
        for (short j = 0; j < column; ++j)
            printf("|%c", board[j][i]);
        printf("|\n");
    }
}

bool BoardHandle::boardIsFull() {
    // static int timer = 0;
    // ++timer;
    // if (timer > 38815)
    //     printf("isOver %d ", timer);
    for (short i = 1; i <= column; ++i)
        if (!colIsFull(i))
            return false;
    return true;
}

char BoardHandle::isOver() {
    // static int timer = 0;
    // ++timer;
    // if (timer > 5000)
    //     printf("isOver %d ", timer);
    // if (boardIsFull())
    //     return 'F';
    // check if there is winn in line
    for (short i=0;i<column;++i)
        for (short j=0;j<top[i];++j)
            // assume that there is only one winn in line in the board
            if (winPieceNearBy(i, j)){
                //printf("debug - column=%d, row=%d\n", i, j);
                return board[i][j];
            }
    return 'N';
}

// before call this, make sure nonFull is empty()
void BoardHandle::nonFullColumn(shortv& nonFull) {
    nonFull.clear();// see if delete this will help?
    for (short i = 0; i < column; ++i)
        if (top[i] != column)
            nonFull.push_back(i+1);
    // static int timer = 0;
    // printf("nonfull %d ", ++timer);
}

char BoardHandle::rPlayer(const char plr) {
    if (plr == 'X')
        return '0';
    return 'X';
}

short BoardHandle::randomMove() {
    shortv list;
    nonFullColumn(list);
    if (list.empty())
        throw runtime_error("trying randomMove() in an empty list\n");
    return randomMove(list);
}

short BoardHandle::randomMove(shortv& list) {
    // debug
    if (list.empty())
        throw runtime_error("trying randomMove(shortv& list) in an empty list\n");
    srand((unsigned)time(NULL));
    return list[rand() % list.size()];
}

short BoardHandle::randomSuggestion(const char plr, shortv& list, const string& mode) {
    shortv opp2, opp1, plr2, plr1;
    char opp = rPlayer(plr);
    srand((unsigned)time(NULL));
    for (vIter col = list.begin(); col != list.end(); ++col) {
        if (colIsFull(*col) || colIsEmpty(*col));
        else if (board[*col - 1][top[*col - 1] - 1] == plr) {
            if (top[*col - 1] - 1 > 0 && board[*col - 1][top[*col - 1] - 2] == plr)
                plr2.push_back(*col);
            else
                plr1.push_back(*col);
        }
        else {
            if (top[*col - 1] -  1> 0 && board[*col - 1][top[*col - 1] - 2] == opp)
                opp2.push_back(*col);
            else
                opp1.push_back(*col);
        }
    }
    short ran = rand() % 100;
    if (mode == "progressive") {
        if (!opp2.empty() && ran < 95)
            return randomMove(opp2);
        ran = rand() % 100;
        if (!plr2.empty() && ran < 72)
            return randomMove(plr2);
    }
    else if (mode == "defensive") {
        if (!opp2.empty() && ran < 95)
            return randomMove(opp2);
        ran = rand() % 100;
        if (!plr2.empty() && ran < 72)
            return randomMove(plr2);
    }
    else
        throw runtime_error("no such mode.\n");
    ran = rand() % 100;
    if (!opp1.empty() && ran < 97)
        return randomMove(opp1);
    ran = rand() % 100;
    if (!plr1.empty() && ran < 95)
        return randomMove(plr1);
    if (list.empty())
        throw runtime_error("call randomSuggestion with empty list");
    return randomMove(list);
}

short BoardHandle::randomSuggestion(const char plr, shortv& list, shortv oppList, const string& mode) {
    shortv tempList;
    for (vIter i = list.begin(); i != list.end(); ++i)
        for (vIter j = oppList.begin(); j != oppList.end();++j)
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

bool BoardHandle::winPieceNearBy(const short col, const short ro) {
    // grow up, right, upright, downright
    short i = 1;
    char present = board[col][ro];
    bool canUp = ro <= top[col] - winn, canRight = col <= column - winn, canDown = ro >= winn - 1;
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

void BoardHandle::refreshBoard(char** hb) {
    for (short i = 0; i < column; ++i)
        for (short j = 0; j < row; ++j)
            board[i][j] = hb[i][j];
}

void BoardHandle::refreshTop() {
    for (short i=0;i<column;++i){
        short j=0;
        for (; j < row && board[i][j] != ' '; ++j) ;
        top[i]=j;
    }
}

