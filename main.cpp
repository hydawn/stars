/*-- encoding: GBK --*/
#include "boardTest.h"

vector<string> charListToVector(int argc, char *args[]) {
	vector<string> rax;
	for (int i = 0; i< argc; ++i) {
		rax.push_back(args[i]);
	}
	return rax;
}

bool inCharList(int argc, char *argv[], const string& str) {
	for (int i = 0; i < argc; ++i)
		if (!strcmp(argv[i], str.c_str()))
			return true;
	return false;
}

// ./Stars test play
void displayHelp() {
	cout << "Usage" << endl
		 << endl
#ifdef STARS_PLATFORM_WINDOWS
		 << "  .\\Stars" << endl
		 << "  .\\Stars [options]" << endl
		 << "  .\\Stars test <number> [options] ..." << endl
#else
#ifdef STARS_PLATFORM_LINUX
		 << "  ./Stars" << endl
		 << "  ./Stars [options]" << endl
		 << "  ./Stars test <number> [options] ..." << endl
		 << endl
		 << "If already installed, use Stars instead" << endl
#endif // STARS_PLATFORM_LINUX
#endif //STARS_PLATFORM_WINDOWS
		 << endl
		 << "It is not recommended to use test without other options\n"
		 << endl
		 << "Option              Description\n"
		 << "  start               start the game in debug mode (default)\n"
		 << "  play                play the game in play mode\n"
		 << "  help                display this help info\n"
		 << "  settings            use settings mode\n"
		 << "  version             show version\n"
		 << "  test <number>       test the game for <number> times, default 1, number must\n"
		 << "                        follow the -test option closely, option test can be\n"
		 << "                        used with the following options:\n"
		 << "      --debug         test debug mode (normal mode, default)\n"
		 << "      --play          test play mode\n"
		 << "      --no-hint       turn off hint\n"
		 << "      --no-ask        won't ask to reverse and ask to save board\n"
		 << "      --less-print    print only moves, board and result\n"
		 << "      --no-print      print only the finishing result\n\n"
		 << endl;
}

bool argsHandle(int argc, char *argv[], string& firstMode) {
#ifdef STARS_VERSION_DEBUG
	string version = STARS_VERSION_DEBUG;
#else
#ifdef STARS_VERSION_RELEASE
	string version = STARS_VERSION_RELEASE;
#endif // STARS_VERSION_RELEASE
#endif // STARS_VERSION_DEBUG
	vector<string> argl = {"--no-hint", "--less-print", "--no-print", "--no-ask",
		"--debug", "--play"};
	if (argc > 16)
		throw invalid_argument("Too many arguments");
	if (argc == 2) {
		if (!strcmp(argv[1], "test")) {
			autoTest(1, charListToVector(argc, argv));
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
		if (!strcmp(argv[1], "play")) {
			firstMode = argv[1];
			return true;
		}
		if (!strcmp(argv[1], "start")) {
			return true;
		}
		else
			throw invalid_argument("No such argument");
	}
	if (argc > 2) {
		if (!strcmp(argv[1], "test")) {
			int rounds = 1;
			if (!inVector(argl, argv[2])) { //must be a number
				try {
					rounds = myStoi(argv[2]);
				}
				catch(const std::exception&) {
					throw invalid_argument("argsHandle: wrong usage of option test");
				}
			}
			if (rounds < 1)
				throw invalid_argument("Wrong number");
			if (rounds > 1000000)
				throw invalid_argument("Too many rounds of test");
			autoTest(rounds, charListToVector(argc, argv));
			return false;
		}
		throw invalid_argument("Argument doesn't exit or wrong usage");
	}
	return true;
}

int main(int argc, char *argv[]) {
	string firstMode;
#ifdef STARS_DEBUG_INFO
	char  a[8]		 = "./Stars";
	char  b[5]		 = "test";
	char  c[4]		 = "1";
	char  d[16]		 = "--less-print";
	char  e[16]		 = "--no-ask";
	char  f[16]		 = "--play";
	//char* fakeArgv[] = {a, b, c, d, e, f};
	char* fakeArgv[] = {a, b, e};
	try {
		//if (!argsHandle(3, fakeArgv, firstMode))
		if (!argsHandle(argc, argv, firstMode))
			return 0;
	}
#else
	try {
		if (!argsHandle(argc, argv, firstMode))
			return 0;
	}
#endif // STARS_DEBUG_INFO
	catch (const std::runtime_error& e) {
		cout << "Stars: runtime_error: ";
		std::cerr << e.what() << '\n';
		return 1;
	}
	catch (const std::invalid_argument& e) {
		cout << "Stars: invalid_argument: ";
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
		interface.controlMode(firstMode);
	}
	catch (const std::runtime_error& e) {
		cout << "Stars: runtime_error: ";
		std::cerr << e.what() << '\n';
		cout << "a bug perhaps\n";
		cout << "Over, hit 'Enter' to close ...";
		cin.get();
		return 1;
	}
	catch (const std::invalid_argument& e) {
		cout << "Stars: invalid_argument: ";
		std::cerr << e.what() << '\n';
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
