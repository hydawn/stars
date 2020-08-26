/*-- encoding: GBK --*/
#include "boardTest.h"

// bool inArgList(int argc, char *argv[], char str[]) {
// 	for (int i = 0; i < argc; ++i)
// 		if (!strcmp(argv[i], str))
// 			return true;
// 	return false;
// }

void displayHelp() {
	cout << "Usage" << endl
		 << endl
#ifdef STARS_PLATFORM_WINDOWS
		 << "  .\\Stars" << endl
		 << "  .\\Stars [options]" << endl
		 << "  .\\Stars [options] <number>" << endl
		 << "  .\\Stars [options] [options] <number>" << endl
#else
#ifdef STARS_PLATFORM_LINUX
		 << "  ./Stars" << endl
		 << "  ./Stars [options]" << endl
		 << "  ./Stars [options] <number>" << endl
		 << "  ./Stars [options] [options] <number>" << endl
		 << endl
		 << "If already installed, use Stars instead" << endl
#endif // STARS_PLATFORM_LINUX
#endif //STARS_PLATFORM_WINDOWS
		 << endl
		 << "It is not recommended to use test without other options\n"
		 << endl
		 << "Options\n"
		 << "  play                          play the game (default)\n"
		 << "  settings                      use settings mode\n"
		 << "  test <number>                 test the game for <number> times, default 1\n"
		 << "  test no-hint <number>         test and turn off hint\n"
		 << "  test less-print <number>      test and print only moves and results\n"
		 << "  test no-print <number>        test and print only the finishing result\n"
		 << "  version                       show version\n"
		 << "  help                          display this help info\n"
		 << endl;
}

bool argsHandle(int argc, char *argv[]) {
#ifdef STARS_VERSION_DEBUG
	string version = STARS_VERSION_DEBUG;
#else
#ifdef STARS_VERSION_RELEASE
	string version = STARS_VERSION_RELEASE;
#endif // STARS_VERSION_RELEASE
#endif // STARS_VERSION_DEBUG

	if (argc > 4)
		throw runtime_error("Too many arguments");
	if (argc == 2) {
		if (!strcmp(argv[1], "test")) {
			autoTest(1);
			return false;
		}
		if (!strcmp(argv[1], "help")) {
			displayHelp();
			return false;
		}
		if (!strcmp(argv[1], "version")) {
			cout << "Stars version: " << version << endl;
			return false;
		}
		if (!strcmp(argv[1], "settings")) {
			BoardInterface interface;
			interface.settingsMode();
			return false;
		}
		if (strcmp(argv[1], "play"))
			throw runtime_error("No such argument");
	}
	if (argc > 2) {
		if (!strcmp(argv[1], "test")) {
			string option;
			int rounds;
			if (strcmp(argv[2], "no-hint") && strcmp(argv[2], "less-print") &&
				strcmp(argv[2], "no-print")) {
				// ./Stars test 8
				rounds = atoi(argv[2]);
			}
			else {
				// ./Stars test less-print*
				option = argv[2];
				rounds = 1;
			}
			if (argc > 3) {
				// ./Stars test less-print 8
				if (strcmp(argv[2], "no-hint") && strcmp(argv[2], "less-print") &&
					strcmp(argv[2], "no-print")) {
					throw runtime_error("No such argument");
				}
				rounds = atoi(argv[3]);
				option = argv[2];
			}
			if (rounds < 1)
				throw runtime_error("Wrong number");
			if (rounds > 1000000)
				throw runtime_error("Too many rounds of test");
			autoTest(rounds, option);
			return false;
		}
		throw runtime_error("Such argument doesn't exit");
	}
	return true;
}

int main(int argc, char *argv[]) {
	// char  a[8]		 = "./Stars";
	// char  b[5]		 = "test";
	// char  c[11]		 = "less-print";
	// char  d[2]		 = "1";
	// char* fakeArgv[] = {a,b,c,d};
	try {
		if (!argsHandle(argc, argv))
			return 0;
	}
	catch (const std::runtime_error& e) {
		cout << "Stars: runtime_error: ";
		std::cerr << e.what() << '\n';
		return 1;
	}
	catch (const std::logic_error& e) {
		cout << "Stars: logic_error: ";
		std::cerr << e.what() << '\n';
		return 1;
	}

	// normal play mode
	BoardInterface interface;
#ifndef STARS_LANG_CHINESE
#ifdef STARS_DEBUG_INFO
	cout << "This is a debug version of Stars\n";
#endif // STARS_DEBUG_INFO
	cout << "Use numbers to place a piece, h for help, 0 to exit.\n";
#else
#ifdef STARS_DEBUG_INFO
	cout << "一个调试版本Stars\n";
#endif // STARS_DEBUG_INFO
	cout << "输入数字下棋，输入h获得帮助，输入0退出\n";
#endif // STARS_LANG_CHINESE

	// there was something normal going on here
	try {
		interface.controlMode();
	}
	catch (const std::runtime_error& e) {
		cout << "Stars: runtime_error: ";
		std::cerr << e.what() << '\n';
		cout << "a bug perhaps\n";
		cout << "Over, hit 'Enter' to close ...";
		cin.get();
		return 1;
	}
#ifdef STARS_DEBUG_INFO
	catch (const std::logic_error& e) {
		cout << "Stars: logic_error: ";
		std::cerr << e.what() << '\n';
		cout << "a bug\n";
		cout << "Over, hit 'Enter' to close ...";
		cin.get();
		return 1;
	}
#endif
#ifndef STARS_LANG_CHINESE
	cout << "Over, hit 'Enter' to close ...";
#else
	cout << "完毕，按回车键关闭 ...";
#endif // STARS_LANG_CHINESE

	cin.get();
	return 0;
}
