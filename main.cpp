/*-- encoding: UTF-8 --*/
#include "boardTest.h"

// ./stars test play
void displayHelp(string mode = "help") {
#ifdef STARS_PLATFORM_WINDOWS
	string stars = ".\\stars";
#else
#	ifdef STARS_PLATFORM_LINUX
	string stars   = "./stars";
#	endif // STARS_PLATFORM_LINUX
#endif     // STARS_PLATFORM_WINDOWS
	string testHelp =
		"Option:\n"
		"  -t, --test\n\n"
		"Description:\n  Test the game by automatically play it in a specified "
		"mode for a few rounds\n\n"
		"Usage:\n"
		"  stars --test <number> [suboption] [suboption]\n\n"
		"Suboptions:\n"
		"  --debug             test debug mode (default)\n"
		"  --play              test play mode\n"
		"  --no-hint           turn off hint\n"
		"  --no-ask            won't ask to reverse and ask to save board\n"
		"  --less-print        print only moves, board and result\n"
		"  --no-print          print only the finishing result\n\n"
		"The argument <number> must follow the option test closely, the "
		"program will\n"
		"test the game for that number of rounds. If argument <number> is not "
		"given\n"
		"it will be set to one by default.\n\n"
		"[suboption] can also be omitted and set to its default value\n";
	if (mode == "--test" || mode == "-t") {
		cout << testHelp;
		return;
	}
	else if (mode != "--help" && mode != "-h") {
		cout << "No suboption help info for " << mode << endl;
		throw invalid_argument("no help info");
	}
	cout
		<< "Usage:\n" 
		<< "  stars\n"
		<< "  stars [options]\n"
		<< "  stars [--test] <number> [suboption] ...\n"
		<< '\n'
		<< "  If not installed, use '" << stars << "' instead\n"
		<< '\n'
		<< '\n'
		<< "Options:            Description\n"
		<< "  -s, --start         start the game in debug mode (default)\n"
		<< "  -p, --play          start the game in play mode\n"
		<< "  -h, --help          display this help info\n"
		<< "  -S, --settings      go into settings mode\n"
		<< "  -v, --version       show version\n"
		<< "  -t, --test          test the game, several suboptions available\n"
		<< '\n'
		<< "  It is not recommended to use --test without suboptions\n"
		<< '\n'
		<< "Use " << stars
		<< " --help [option] to see help for available suboptions" << endl;
}

bool argsHandle(int argc, char* argv[], string& firstMode) {
#ifdef STARS_VERSION_DEBUG
	string version = STARS_VERSION_DEBUG;
#else
#	ifdef STARS_VERSION_RELEASE
	string version = STARS_VERSION_RELEASE;
#	endif // STARS_VERSION_RELEASE
#endif     // STARS_VERSION_DEBUG
	vector<string> argl = {"--no-hint", "--less-print", "--no-print",
						   "--no-ask",  "--debug",      "--play"};
	if (argc > 16)
		throw invalid_argument("Too many arguments");
	if (argc == 2) {
		if (!strcmp(argv[1], "--test") || !strcmp(argv[1], "-t")) {
			autoTest(1, MainArgsHandle::charListToVector(argc, argv));
			return false;
		}
		if (!strcmp(argv[1], "--help") || !strcmp(argv[1], "-h")) {
			displayHelp("--help");
			return false;
		}
		if (!strcmp(argv[1], "--version") || !strcmp(argv[1], "-v")) {
			cout << "stars version: " << version << endl;
			return false;
		}
		if (!strcmp(argv[1], "--settings") || !strcmp(argv[1], "-S")) {
			BoardInterface interface;
			interface.settingsMode();
			return false;
		}
		if (!strcmp(argv[1], "--play") || !strcmp(argv[1], "-p")) {
			firstMode = "play";
			return true;
		}
		if (!strcmp(argv[1], "--start") || !strcmp(argv[1], "-s")) {
			return true;
		}
		else {
			printf("No such option: %s\n", argv[1]);
			throw invalid_argument("no such option");
		}
	}
	if (argc > 2) {
		if (!strcmp(argv[1], "--help") || !strcmp(argv[1], "-h")) {
			displayHelp(argv[2]);
			return false;
		}
		if (!strcmp(argv[1], "--test") || !strcmp(argv[1], "-t")) {
			int rounds = 1;
			if (!MainArgsHandle::inVector(argl, argv[2])) { // must be a number
				try {
					rounds = ToInt::myStoi(argv[2]);
				}
				catch (const std::exception&) {
					printf("Invalid option: %s", argv[2]);
					throw invalid_argument("wrong usage of option --test");
				}
			}
			if (rounds < 1) {
				cout << "<number> = " << rounds << endl;
				throw invalid_argument("too small a number");
			}
			if (rounds > 1000000) {
				cout << "<number> = " << rounds << endl;
				throw invalid_argument("too many rounds of test");
			}
			// now [0], [1], [2] is good, check for wrong options
			for (int i = 3; i < argc; ++i)
				if (!MainArgsHandle::inVector(argl, argv[i])) {
					printf("invalid option: %s\n", argv[i]);
					throw invalid_argument("no such option");
				}
			autoTest(rounds, MainArgsHandle::charListToVector(argc, argv));
			return false;
		}
		printf("option %s doesn't exit or wrong usage", argv[1]);
		throw invalid_argument("invalid option");
	}
	return true;
}

int main(int argc, char* argv[]) {
	string firstMode;
#ifdef STARS_DEBUG_INFO
	char a[8]  = "./stars";
	char b[5]  = "test";
	char c[4]  = "1";
	char d[16] = "--less-print";
	char e[16] = "--no-ask";
	char f[16] = "--play";
	// char* fakeArgv[] = {a, b, c, d, e, f};
	char* fakeArgv[] = {a, b, e};
	try {
		// if (!argsHandle(3, fakeArgv, firstMode))
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
		cout << "stars: runtime_error: ";
		std::cerr << e.what() << '\n';
		return 1;
	}
	catch (const std::invalid_argument& e) {
		cout << "stars: invalid_argument: ";
		std::cerr << e.what() << '\n';
		cout << "Use option '--help' to see help\n";
		return 1;
	}
	catch (const std::logic_error& e) {
		cout << "stars: logic_error: ";
		std::cerr << e.what() << '\n';
		return 1;
	}

	// normal play mode
	BoardInterface interface;
#ifndef STARS_LANG_CHINESE
#	ifdef STARS_DEBUG_INFO
	cout << "This is a debug version of Stars\n";
#	endif // STARS_DEBUG_INFO
	cout << "Enter number to play, \"help\" for help info, \"quit\" to "
			"quit the whole game\n";
#else
#	ifdef STARS_DEBUG_INFO
	cout << "一个调试版本Stars\n";
#	endif // STARS_DEBUG_INFO
	cout << "输入数字下棋，输入help获得帮助，输入exit退出\n";
#endif     // STARS_LANG_CHINESE

	// there was something normal going on here
	try {
		interface.controlMode(firstMode);
	}
	catch (const std::runtime_error& e) {
		cout << "stars: runtime_error: ";
		std::cerr << e.what() << '\n';
		cout << "a bug perhaps\n";
		cout << "Over, hit 'Enter' to close ...";
		cin.get();
		return 1;
	}
	catch (const std::invalid_argument& e) {
		cout << "stars: invalid_argument: ";
		std::cerr << e.what() << '\n';
		cout << "Over, hit 'Enter' to close ...";
		cin.get();
		return 1;
	}
	catch (const std::logic_error& e) {
		cout << "stars: logic_error: ";
		std::cerr << e.what() << '\n';
		cout << "a bug\n";
		cout << "Over, hit 'Enter' to close ...";
		cin.get();
		return 1;
	}

#ifndef STARS_LANG_CHINESE
	cout << "Over, hit 'Enter' to close ...";
#else
	cout << "完毕，按回车键关闭 ...";
#endif // STARS_LANG_CHINESE
	cin.get();
	return 0;
}
