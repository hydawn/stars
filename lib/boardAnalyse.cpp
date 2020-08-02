#include "boardAnalyse.h"

void BoardAnalyse::go(const char plr, const short move) {
	// debug - delete this for performance
	if (board.colIsFull(move))
		throw runtime_error("Error: trying to add to a full column.\n");
	board.add(plr, move);
}

void BoardAnalyse::reverse(const short column){
	if(board.colIsEmpty(column))
		throw runtime_error("Error: tying to reverse an empty column\n");
	board.remove(column);
}

shortv BoardAnalyse::firstPoint(const char plr) {
	shortv fp, nfc;
	board.nonFullColumn(nfc);
	for (short col : nfc) {
		board.add(plr, col);
		if (plr == board.isOver())
			fp.push_back(col);
		board.remove(col);
	}
	return fp;
}

shortv BoardAnalyse::firstPoint(const char plr, shortv& nfc) {
	shortv fp;
	for (short col : nfc) {
		board.add(plr, col);
		if (plr == board.isOver())
			fp.push_back(col);
		board.remove(col);
	}
	return fp;
}

// this is actually copied from the original analyse to cope with the
// performance problem
string BoardAnalyse::hiddenAnalyse(const char plr, shortv& list)
{
	// static int timer = 0;
	// ++timer;
	// printf("analyse %d ", timer);
	// reform

	shortv tempList, goodList, temp1, temp2, nonFull;
	vIter col;

	board.nonFullColumn(nonFull);
	list = firstPoint(plr, nonFull);
	if (!list.empty())
		return "good";

	char opp = board.rPlayer(plr);
	list = firstPoint(opp, nonFull);
	if (list.size() > 1)
		return "bad";
	if (list.size() == 1) {
		board.add(plr, list[0]);
		temp1 = firstPoint(opp);
		board.remove(list[0]);
		if (!temp1.empty())
			return "bad";
		return "free";
	}

	// I'll go
	list = nonFull;
	col = list.begin();
	while (col != list.end()) {
		board.add(plr, *col);
		board.nonFullColumn(nonFull);
		tempList = firstPoint(opp, nonFull);
		if (!tempList.empty()) {
			board.remove(*col);
			col = list.erase(col);
			continue;
		}
		tempList = firstPoint(plr, nonFull);
		if (tempList.size() > 1) {
			goodList.push_back(*col);
		}
		else if (tempList.size() == 1) {
			board.add(opp, tempList[0]);
			board.nonFullColumn(nonFull);
			// it's my turn
			temp1 = firstPoint(plr, nonFull);
			if (!temp1.empty())
				goodList.push_back(*col);
			else {
				temp1 = firstPoint(opp, nonFull);
				if (temp1.size() > 1) {
					board.remove(tempList[0]);
					board.remove(*col);
					col = list.erase(col);
					continue;
				}
				if (temp1.size() == 1) {
					board.add(plr, temp1[0]);
					// shortv temp2 = firstPoint(opp);
					temp2 = firstPoint(opp);
					board.remove(temp1[0]);
					if (!temp2.empty()) {
						board.remove(tempList[0]);
						board.remove(*col);
						col = list.erase(col);
						continue;
					}
				}
			}
			board.remove(tempList[0]);
		}
		board.remove(*col);
		++col;
	}
	if (!goodList.empty()) {
		list = goodList;
		return "good";
	}
	if (list.empty())
		return "bad";
	return "free";
}

// check if game is over or board is full before call this
// check if there are only one move
// it's not well structured to read, but it is to perform
string BoardAnalyse::analyse(const char plr, shortv& list) {
	// static int timer = 0;
	// ++timer;
	// printf("analyse %d ", timer);
	// reform

	shortv tempList, goodList, temp1, temp2, nonFull;
	vIter col;

	board.nonFullColumn(nonFull);
	list = firstPoint(plr, nonFull);
	if (!list.empty())
		return "good";

	char opp = board.rPlayer(plr);
	list = firstPoint(opp, nonFull);
	if (list.size() > 1)
		return "bad";
	if (list.size() == 1) {
		board.add(plr, list[0]);
		temp1 = firstPoint(opp);
		board.remove(list[0]);
		if (!temp1.empty())
			return "bad";
		return "free";
	}

	// I'll go
	list = nonFull;
	col = list.begin();
	while (col != list.end()) {
		board.add(plr, *col);
		board.nonFullColumn(nonFull);
		tempList = firstPoint(opp, nonFull);
		if (!tempList.empty()) {
			board.remove(*col);
			col = list.erase(col);
			continue;
		}
		tempList = firstPoint(plr, nonFull);
		if (tempList.size() > 1) {
			goodList.push_back(*col);
		}
		else if (tempList.size() == 1) {
			board.add(opp, tempList[0]);
			board.nonFullColumn(nonFull);
			// it's my turn
			temp1 = firstPoint(plr, nonFull);
			if (!temp1.empty())
				goodList.push_back(*col);
			else {
				temp1 = firstPoint(opp, nonFull);
				if (temp1.size() > 1) {
					board.remove(tempList[0]);
					board.remove(*col);
					col = list.erase(col);
					continue;
				}
				if (temp1.size() == 1) {
					board.add(plr, temp1[0]);
					// shortv temp2 = firstPoint(opp);
					temp2 = firstPoint(opp);
					board.remove(temp1[0]);
					if (!temp2.empty()) {
						board.remove(tempList[0]);
						board.remove(*col);
						col = list.erase(col);
						continue;
					}
				}
			}
			board.remove(tempList[0]);
		}
		board.remove(*col);
		++col;
	}
	if (!goodList.empty()) {
		list = goodList;
		return "good";
	}
	if (list.empty())
		return "bad";
	return "free";
}

// check if game is over or board is full before call this
string BoardAnalyse::returnMove(const char plr, shortv& list, const short depth) {
	string word = analyse(plr, list);
	if (word == "good" || word == "bad" || list.size() == 1)
		return word;
	
	/*********************************debug************************************/
	int addNumber1 = board.getAddNumber(), removeNumber1 = board.getRemoveNumber();
	// long longCounter = 0;
	/*********************************debug************************************/
	char opp = board.rPlayer(plr);
	shortv list1, goodList;
	vIter col = list.begin();//, col1, col2;

	// one big loop ahead of us
	while (col != list.end()) {
		board.add(plr, *col);
		word = analyse(opp, list1);
		if (word == "good") {
			board.remove(*col);
			col = list.erase(col);
			continue;
		}
		else if (word == "bad")
			goodList.push_back(*col);
		else {
			shortv list2;
			short goodCount1 = 0;
			bool isBad1=false;
			for (vIter col1 = list1.begin();col1!=list1.end();++col1){
				board.add(opp, *col1);
				word = analyse(plr, list2);
				if (word == "bad") { // bad
					board.remove(*col1);
					isBad1=true;
					break; // goes straight to line197 if (col1!=list1.end())
				}
				else if (word=="good")
					++goodCount1;
				else {
					// let's see if opp go col1 will be good for me
					shortv list3;
					short badCount2 = 0;
					short isGood2 = false;
					// evaluate if plr go *col2 is good
					for (vIter col2 = list2.begin(); col2 != list2.end(); ++col2) {
						board.add(plr, *col2);
						word = analyse(opp, list3);
						if (word == "good")  // bad for me, can't go col2
							++badCount2;
						else if (word == "bad") { // good
							board.remove(*col2);
							isGood2 = true;
							// ++goodCount1;
							break;
						}
						else if (depth > 2) {
							shortv list4;
							short goodCount3 = 0;
							bool isBad3 = false;
							for (vIter col3 = list3.begin(); col3 != list3.end(); ++col3) {
								board.add(opp, *col3);
								word = analyse(plr, list4);
								if (word == "bad") { // bad
									board.remove(*col3);
									isBad3 = true;
									break; //straight into 'if after while'
								}
								else if (word == "good")
									++goodCount3;
								else if (depth > 3) {
									shortv list5;
									bool isGood4 = false;
									short badCount4 = 0;
									for (vIter col4 = list4.begin(); col4 != list4.end();++col4) {
										board.add(plr, *col4);
										word = analyse(opp, list5);
										if (word == "bad") { // good, then col3 is good
											isGood4 = true;
											board.remove(*col4);
											break;
										}
										else if (word == "good")
											++badCount4;
										else if (depth > 4) {
											shortv list6;
											bool isBad5 = false;
											short goodCount5 = 0;
											for (vIter col5 = list5.begin(); col5 != list5.end();++col5){
												board.add(opp, *col5);
												word = analyse(plr, list6);
												if(word=="good")
													++goodCount5;
												else if (word=="bad"){
													board.remove(*col5);
													isBad5 = true;
													break;
												}
												else if (depth > 5) {
													shortv list7;
													bool isGood6 = false;
													short badCount6 = 0;
													for (vIter col6 = list6.begin(); col6 != list6.end();++col6) {
														board.add(plr, *col6);
														word = analyse(opp, list7);
														if (word=="bad") {// good
															board.remove(*col6);
															isGood6 = true;
															break;
														}
														else if (word=="good"){
															++badCount6;
														}
														else if (depth > 6 && !list7.empty()) {
															// this is gonna be a disaster, it's just too much for a function
															shortv list8;
															bool isBad7 = false;
															short goodCount7 = 0;
															for (vIter col7 = list7.begin(); col7 != list7.end(); ++col7) {
																board.add(opp, *col7);
																word = analyse(plr, list8);
																if (word == "good") ++goodCount7;
																else if (word == "bad") {
																	board.remove(*col7);
																	isBad7 = true; break;
																}
																else if (depth > 7 && !list8.empty()) {
																	shortv list9;
																	bool isGood8 = false;
																	short badCount8 = 0;
																	for (vIter col8 = list8.begin(); col8 != list8.end(); ++col8) {
																		board.add(plr, *col8);
																		word = analyse(opp, list9);
																		if (word == "good") ++badCount8;//bad for me
																		else if (word == "bad") {
																			isGood8 = true;
																			board.remove(*col8);
																			break;
																		}
																		else if (depth > 8 && !list9.empty()){
																			shortv list10;
																			bool isBad9 = false;
																			short goodCount9 = 0;
																			// ++longCounter;
																			// printf("%d%d %d%d %d%d %d%d %d player x hit here.\n\n", *col,*col1, *col2, *col3, *col4, *col5, *col6, *col7, *col8);
																			for (vIter col9 = list9.begin(); col9 != list9.end(); ++col9) {
																				board.add(opp, *col9);
																				word = analyse(plr, list10);
																				if (word=="good")
																					++goodCount9;
																				else if(word=="bad"){
																					board.remove(*col9);
																					isBad9 = true;
																					break;
																				}
																				else
																					;
																				board.remove(*col9);
																			}
																			if (isBad9) ++badCount8;
																			else if (goodCount9 == list9.size() && goodCount9!=0){
																				isGood8 = true;
																				board.remove(*col8); break;
																			}
																		}
																		board.remove(*col8);
																	}
																	if (isGood8) ++goodCount7;
																	else if (badCount8 == list8.size()) {
																		board.remove(*col7);
																		isBad7 = true; break;
																	}
																}
																board.remove(*col7);
															}
															if (isBad7)++badCount6;
															else if(goodCount7==list7.size()){
																board.remove(*col6);
																isGood6 = true; break;
															}
														}
														board.remove(*col6);
													}
													if (isGood6) ++goodCount5;
													else if (badCount6==list6.size() && !list6.empty()){
														board.remove(*col5);
														isBad5 = true; break;
													}
												}
												board.remove(*col5);
											}
											if(isBad5) ++badCount4;
											else if(goodCount5==list5.size() && !list5.empty()){
												isGood4 = true;
												board.remove(*col4); break;
											}
										}
										else if (list5.size() == 1) {
											shortv list6;
											board.add(opp, list5[0]);
											word = analyse(plr, list6);
											if (word == "good") {
												board.remove(list5[0]);
												board.remove(*col4);
												isGood4 = true;
												break;
											}
											else if (word == "bad")
												++badCount4;
											else if (list6.size() == 1) {
												// shortv list7;
												// blahblahblah
											}
											board.remove(list5[0]);
										}
										board.remove(*col4);
									}
									if (isGood4)
										++goodCount3;
									else if(badCount4==list4.size() && !list4.empty()){
										board.remove(*col3);
										isBad3 = true;
										break; //straight into 'if after while'
									}
								}
								else if (list4.size() == 1) {
									board.add(plr, list4[0]);
									shortv list5;
									word = analyse(opp, list5);
									if (word == "good") { // col4 is bad, so col3 is bad
										board.remove(list4[0]);
										board.remove(*col3);
										isBad3 = true;
										break; //straight into 'if after while'
									}
									else if (word == "bad") ++goodCount3; //break if there was a for loop for list4
									else if (list5.size() == 1) {
										shortv list6;
										board.add(opp, list5[0]);
										word = analyse(plr, list6);
										if (word == "bad") {
											board.remove(list5[0], list4[0], *col3);
											isBad3 = true;
											break; //straight into 'if after while'
										}
										else if (word=="good") ++goodCount3;//break if there was a for loop for list4
										else
											;
										board.remove(list5[0]);
									}
									else
										;
									board.remove(list4[0]);
								}
								board.remove(*col3);
							}
							if (isBad3) { // col2 is bad for me
								++badCount2;
							}
							if (goodCount3 == list3.size() && !list3.empty()){
								board.remove(*col2);
								isGood2 = true;
								// ++goodCount1;
								break;
							}
						}
						else if (list3.size() == 1) { // if opp has to go here
							board.add(opp, list3[0]);
							shortv list4;
							word = analyse(plr, list4);
							if (word == "bad") { // bad for me
								++badCount2;
							}
							else if (word == "good"){
								board.remove(list3[0]);
								board.remove(*col2);
								++goodCount1;
								break; // break the list2 for loop
							}
							else if (list4.size() == 1) {
								board.add(plr, list4[0]);
								shortv list5;
								word = analyse(opp, list5);
								if (word == "good") {
									++badCount2;
								}
								else if (word == "bad") {
									board.remove(list4[0], list3[0], *col2);
									++goodCount1;
									break;
								}
								else if (list5.size()==1){
									board.add(opp, list5[0]);
									shortv list6;
									word=analyse(plr, list6);
									if (word=="bad"){
										++badCount2;
									}
									else if (word=="good") {
										printf("finally!, comething good!\n\n");
										board.remove(list5[0]);
										board.remove(list4[0], list3[0], *col2);
										++goodCount1;
										break;
									}
									board.remove(list5[0]);
								}
								board.remove(list4[0]);
							}
							board.remove(list3[0]);
						}
						board.remove(*col2);
					}
					if (badCount2==list2.size() && !list2.empty() ) { // if all bad, then opp go col1 is bad for me
						board.remove(*col1);
						isBad1 = true;
						break;
					}
					if (isGood2)
						++goodCount1;
				}
				board.remove(*col1);
			}
			if (isBad1) { // then it's bad
				board.remove(*col);
				col = list.erase(col);
				continue;
			}
			if (goodCount1 == list1.size() && !list1.empty()) {
				goodList.push_back(*col);
				// printf("%d is added to goodList\n", *col);
			}
		}
		board.remove(*col);
		++col;
	}
	
	/*******************************debug**********************************/
	addNumber1 = board.getAddNumber()- addNumber1;
	removeNumber1 = board.getRemoveNumber()- removeNumber1;
	// printf("addNumber=%d, removeNumber=%d\n", addNumber1, removeNumber1);
	// printf("depth=%d longCounter\n", depth);//longCounter);
	if (addNumber1 != removeNumber1) {
		throw runtime_error("add and remove didn't match!\n");
	}
	/*******************************debug**********************************/
	if (!goodList.empty()){
		list = goodList;
		return "good";
	}
	if (list.empty())
		return "bad";
	return "free";
}

// not so sure I still need this, after all, returnMove is quite big
// go straight into loop and return word in player's perspective
// check if game is over or board is full before call this, given list must not
// be empty
string BoardAnalyse::returnSituation(const char plr, shortv& list,
	short returnMoveDepth/*3*/, int recursiveCount/*0*/, int countTop/*3*/) {
	/****************************debug theory**********************************
	this recursiveSituation is merely an infinite version of returnMove. If you
	can see this, you will soon find a flaw in this function: there are no
	'break;'! in returnMove, I won't always prepare a goodCount and badCount, I
	always define a count and a isGood/isBad before going into a loop. Now, why
	should I define a goodList to track all the good col? I will never use them
	anyway! And in this case, most recursiveSituation is actually called by
	another recursiveSituation where 'another recursiveSituation doesn't care
	what our goodList are! Therefore, I wrote another version of
	recursiveSituation which doesn't take shortv as an argument, and
	change this formal recursiveSituation's name as returnSituation, and the
	newly defined recursiveSituationInline as recursiveSituation
	****************************debug theory**********************************/
	if (list.empty())
	{
		throw runtime_error("recursiveSituation: given list is empty!\n");
		return "end";
	}

	// a loop
	vIter col = list.begin();
	char opp = board.rPlayer(plr);
	shortv goodList, list1;
	string word;
	while (col!=list.end()){
		board.add(plr, *col);
		word = returnMove(opp, list1, returnMoveDepth);
		if (word=="free"){
			if (!list1.empty())
				word = recursiveSituation(opp, list1, returnMoveDepth,
					recursiveCount, countTop);
			else
				printf("game is over, what do I do? ---- nothing I suppose.\n");
		}
		if (word=="good"){
			board.remove(*col);
			col = list.erase(col);
			continue;
		}
		else if (word=="bad")
			goodList.push_back(*col);
		board.remove(*col);
		++col;
	}
	if (!goodList.empty()){
		list = goodList;
		return "good";
	}
	else if (list.empty())
		return "bad";
	return "free";
}

// shouldn't call this with an emptylist
string BoardAnalyse::recursiveSituation(const char plr, shortv& list,
	short returnMoveDepth/*3*/, int recursiveCount/*0*/, int countTop/*3*/) {
	/*****************************debug theory*********************************
	Here is another version of recursiveSituation unfortunately, doesn't take
	shortv isn't gonna work, so here we are. Now I can use for loop and
	count and break! This will work faster than the last one.
	This is basically the old recursiveSituation copied here
	*****************************debug theory*********************************/
	/*****************************debug action*********************************/
	if (list.empty())
	{
		throw runtime_error("recursiveSituation: given list is empty!\n");
		return "end";
	}
	/*****************************debug action*********************************/

	// main
	shortv list1;
	string word;
	short badCount = 0;
	vIter col = list.begin();
	for (;col != list.end();++col)
	{
		board.add(plr, *col);
		word = returnMove(board.rPlayer(plr), list1, returnMoveDepth);
		if (word=="free"){
			if (!list1.empty())
				word = recursiveSituation(board.rPlayer(plr), list1,
					returnMoveDepth, recursiveCount, countTop);
			else /*debug - delete this else for performance considerations*/
				printf("game is over, what do I do? ---- nothing I suppose.\n");
		}
		if (word=="good")
			++badCount;
		else if (word=="bad"){
			board.remove(*col);
			break;
		}
		board.remove(*col);
	}
	if (col != list.end())
		// this is actually based on a fact that list.empty() == false, which is
		// not always the case if the function isn't called properly
		return "good";
	else if (badCount == list.size())
		return "bad";
	return "free";
}

int BoardAnalyse::respond(const char plr, oneMove& thisMove, bool showCal, bool showTime) {
	/*
	 * Maybe it's better to move it to boardInterface
	*/

	shortv list, oppList;
	double timeUsed = 0;

	board.nonFullColumn(list);
	if (list.empty())
		throw runtime_error("call respond with full board!\n");
	if (board.isOver() == plr || board.isOver() == board.rPlayer(plr))
		throw runtime_error("call respond with ended game!\n");

	system_clock::time_point start = system_clock::now();
	string word = returnMove(plr, list, 3);
    system_clock::time_point end = system_clock::now();
	auto elapsed = duration_cast<milliseconds>(end - start);
	timeUsed = elapsed.count();
	string word0 = returnMove(board.rPlayer(plr), oppList, 3);

	if (showCal) {
		std::cout << "\nword = " << word <<"\nlist = [ ";
		for(short c:list)
			std::cout << c << " ";
		printf("]\n");
	}
	if (showTime)
		cout << "Computer time used: " << timeUsed << " ms\n";


	thisMove.word = word;
	thisMove.list = list;
	thisMove.time = timeUsed;
	if (word != "bad")
		return board.randomSuggestion(plr, list, oppList);
	else if (word == "bad") {
		word = analyse(plr, list);
		if (word == "bad")
			return board.randomMove();
		else return board.randomSuggestion(plr, list, oppList);
	}
	else
		throw runtime_error("what?\n");
	return 0;
}
