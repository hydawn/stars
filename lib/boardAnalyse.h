#ifndef _BOARDANALYSE_H_
#define _BOARDANALYSE_H_ 1
#include <string.h>
#include "boardRecord.h"

using std::string;

class BoardAnalyse {
    // more like an analyze class
private:
    // analyse
    shortv firstPoint(const char plr);
    shortv firstPoint(const char plr, shortv& nfc);
    string hiddenAnalyse(const char plr, shortv& list);
    string analyse(const char plr, shortv& list);
    string recursiveSituation(const char plr, shortv& list,
        short returnMoveDepth=3 ,int recursiveCount=0, int countTop=3);

public:
    BoardHandle board;
    BoardAnalyse() : board(8,8,4) {}
    BoardAnalyse(BoardHandle board_) : board(board_){}
    BoardAnalyse(const short r, const short c, const short w) : board(r,c,w) {}

    void show() { board.show(); }

    // analyse function
    /*
     * I should bear in mind that the whole concept of analyse function is to
     * narrow down the freeList, if not to gain "good" or "bad"
     */
    string returnMove(const char plr, shortv& list, const short depth=3);
    string returnSituation(const char plr, shortv& list,
        short returnMoveDepth=3 ,int recursiveCount=0, int countTop=3);
    int respond(const char plr, oneMove& thisMove, bool showCal, bool showTime);

    // change board
    void go(const char plr, const short move);
    void reverse(const short column);

    // getter
    short getColTop(const short column) const { return board.top[column - 1]; }
    short getColumn() const { return board.column; }
    short getRow() const { return board.row; }

    // is function
    // else transfer
    bool colIsFull(const short col) { return board.colIsFull(col); }
    bool colIsEmpty(const short col) { return board.colIsEmpty(col); }
    char gameIsOver() { return board.isOver(); }
    bool boardIsFull() { return board.boardIsFull(); }
    char rPlayer(const char plr) { return board.rPlayer(plr); }
    // short randomSuggestion(const char plr, shortv& list) { return board.randomSuggestion(plr, list); }
    // not so useful for now
    // void changeWidth(short wi) {}
    // void changeLength(short le) {}
    // void changeWinNumber(short wn) {}
};

#endif