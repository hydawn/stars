#include "boardHandle.h"

int BoardHandle::removeNumber;
int BoardHandle::addNumber;

BoardHandle::BoardHandle() : row(8), column(8), winn(4) {
    generate();
}

BoardHandle::BoardHandle(const BoardHandle& input) : row(input.row),
    column(input.column), winn(input.winn){
    generate(input.board, input.top);
}

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
    // it's best to use allocate
    board = new char* [column];
    top = new short[column];
    for (short i = 0; i < column; ++i) {
        board[i] = new char[row + 1];
        top[i] = 0;
    }
    for (short i = 0; i < column; ++i)
        for (short j = 0; j < row; ++j)
            board[i][j] = ' ';
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

BoardHandle& BoardHandle::operator=(const BoardHandle &bh){
    if(this==&bh)
        return *this;
    refreshBoard(bh.board);
    refreshTop();
    return *this;
}

Json::Value BoardHandle::boardToJson(){
    Json::Value root;
    for (short i = 0; i < column;++i){
        board[i][row] = '\0';
        // root.append(arraryToJson(board[i], row));
        root.append(board[i]);
    }
    return root;
}

Json::Value BoardHandle::topToJson()
{
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
    return randomMove(list);
}

short BoardHandle::randomMove(shortv& list) {
    srand((unsigned)time(NULL));
    return list[rand() % list.size()];
}

short BoardHandle::randomSuggestion(const char plr, shortv& list) {
    shortv opp2, opp1, plr2, plr1, nfl;
    char opp = rPlayer(plr);
    nonFullColumn(nfl);
    srand((unsigned)time(NULL));
    for (vIter col = nfl.begin(); col != nfl.end(); ++col) {
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
    short ran = rand() % 1000;
    if (!opp2.empty() && ran < 92)
        return randomMove(opp2);
    if (!plr2.empty() && ran < 85)
        return randomMove(plr2);
    if (!plr1.empty() && ran < 75)
        return randomMove(plr1);
    if (!opp1.empty() && ran < 55)
        return randomMove(plr1);
    return randomMove(list);
}

short BoardHandle::randomSuggestion(const char plr, shortv& list, shortv oppList) {
    shortv tempList;
    for (vIter i = list.begin(); i != list.end(); ++i)
        for (vIter j = oppList.begin(); j != oppList.end();++j)
            if (*j == *i) {
                tempList.push_back(*i);
                j = oppList.erase(j);
                break;
            }
    if (tempList.empty())
        return randomSuggestion(plr, list);
    return randomSuggestion(plr, tempList);
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

void BoardHandle::clearBoardAndTop() {
    for (short i = 0; i < column;++i) {
        for (short j = 0; j < top[i];++j)
            board[i][j] = ' ';
        top[i] = 0;
    }
}
