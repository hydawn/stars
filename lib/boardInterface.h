#ifndef _BOARDINTERFACE_H_
#define _BOARDINTERFACE_H_ 1
#include "boardAnalyse.h"

#define INTER_MAX_INPUT 64

using std::string;
using std::cin;

class BoardInterface
{
private:
    BoardAnalyse* analyse; // this might be more important?
    BoardRecord record;

public:
    BoardInterface() {generate(8,8,4);}
    BoardInterface(BoardAnalyse& hb) { analyse = &hb;}

    void generate(short c, short r, short w) { analyse = new BoardAnalyse;}

    // general
    string getInput(string mode);
    string getInput(string mode, char plr, double& inputTime);
    void getStateFromInput();

    // mode
    void addMode();
    void reverseMode();
    void debugMode(char arg='n');
    void normalMode();
    void settingsMode();
    void playMode();

    // ask & do
    void askToDebug(bool yes); // if yes == true, then default yes
    void askToSaveBoard(bool yes);
    void importNewBoard();

    // info
    string getHelp(string mode = "normal");
    string getInfo(string input = "info");

    // refresh
    void refreshRecord(const BoardRecord &record_) { record = record_; }

    // show
    void showComment(oneMove& move);
};

#endif
