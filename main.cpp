#include <stdlib.h>
#include "lib/boardInterface.h"


/*********/
int main() {
    string input;
    char plr = 'X', opp = '0';
    BoardAnalyse boardAnalyser;
    BoardInterface boardInterface(boardAnalyser);
    printf("Use 1~8 to place a piece, h for help, 0 to exit.\n");
    printf("Game is afoot!\n");

    // normal mode
    boardInterface.normalMode();

    cout << "Over!\n";
    system("pause");
    return 0;
}
/*******/