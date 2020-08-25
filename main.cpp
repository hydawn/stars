#include "boardInterface.h"

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
	} catch (const std::runtime_error& err) {
		cout << endl << err.what() << "- a bug perhaps\n";
	}

	cout << "Over, hit 'Enter' to close ...";
	cin.get();
	return 0;
}
