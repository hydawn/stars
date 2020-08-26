#include "boardTest.h"

void autoTest(int n) {
	BoardInterface boardInterface;
	boardInterface.settingsMode();
	int i, err = 0, XWinn = 0, ZeroWinn = 0;
	for (i = 0; i < n; ++i) {
		try {
			BoardTest test;
			try {
				test.controlMode();
				if (test.toWinn == 'X')
					++XWinn;
				else if (test.toWinn == '0')
					++ZeroWinn;
			}
			catch(const std::exception& e) {
				std::cerr << e.what() << '\n';
				test.askToSaveBoard(true);
				++err;
			}
		}
		catch(const std::exception& e) {
			std::cerr << e.what() << '\n';
			cout << "Error occurred when creating or destroying BoardTest\n";
		}
	}
	cout << "i = " << i << endl;
	cout << "err = " << err << endl;
	cout << "XWinn = " << XWinn << endl;
	cout << "ZeroWinn = " << ZeroWinn << endl;
	cout << "Over, hit 'Enter' to close ...";
	cin.get();
}

int main() {
	//autoTest(55);
	//return 0;
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
