#include "boardInterface.h"
/*****
int main() {
	int i;
	for (i = 0; i < 24; ++i) {
		BoardInterface boardInterface;
		boardInterface.selfPlayMode();
	}
	cout << "i = " << i << endl;
	cout << "Over, hit 'Enter' to close ...";
	cin.get();
	return 0;
}
*****/


/*****/
int main() {
	BoardInterface boardInterface;
#ifdef STARS_DEBUG_INFO
	cout << "This is a debug version of main.cpp\n";
#endif
	cout << "Use numbers to place a piece, h for help, 0 to exit.\n";
	cout << "The game is on ->\n";

	// there was something normal going on here
	try {
		boardInterface.controlMode();
	}
	catch (const std::runtime_error& e) {
		cout << endl << e.what() << "\n    runtime_error: bug perhaps\n";
	}
#ifdef STARS_DEBUG_INFO
	catch (const std::logic_error& e) {
		cout << endl << e.what() << "\n    logic_error: a bug\n";
	}
#endif

	cout << "Over, hit 'Enter' to close ...";
	cin.get();
	return 0;
}
/*****/
