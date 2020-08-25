#include "boardInterface.h"

// err = 7
void autoTest(int n) {
	int i, err = 0;
	for (i = 10; i < n; ++i) {
		BoardInterface boardInterface;
		err += boardInterface.autoTestMode(i % 8 + 1);
	}
	cout << "i = " << i << endl;
	cout << "err = " << err << endl;
	cout << "Over, hit 'Enter' to close ...";
	cin.get();
}

int main() {
	// autoTest(48);
	// return 0;
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
