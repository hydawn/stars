#include <stdlib.h>
#include "lib/boardInterface.h"


/*********/
int main() {
    BoardAnalyse boardAnalyser;
    BoardInterface boardInterface(boardAnalyser);
    printf("Use numbers to place a piece, h for help, 0 to exit.\n");
    printf("Game is afoot!\n");

    boardInterface.normalMode();

    cout << "Over!\n";
    system("pause");
    return 0;
}
/*******/