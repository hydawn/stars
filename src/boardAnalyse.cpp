#include "boardAnalyse.h"

void BoardAnalyse::go(const char plr, const short move) {
	// debug - delete this for performance
	if (state.colIsFull(move))
		throw runtime_error("Error: trying to add to a full column.\n");
	state.add(plr, move);
}

void BoardAnalyse::reverse(const short column) {
	if (state.colIsEmpty(column))
		throw runtime_error("Error: tying to reverse an empty column\n");
	state.remove(column);
}

shortv BoardAnalyse::firstPoint(const char plr) {
	shortv fp, nfc;
	state.nonFullColumn(nfc);
	for (short col : nfc) {
		state.add(plr, col);
		if (plr == state.isOver())
			fp.push_back(col);
		state.remove(col);
	}
	return fp;
}

shortv BoardAnalyse::firstPoint(const char plr, shortv& nfc) {
	shortv fp;
	for (short col : nfc) {
		state.add(plr, col);
		if (plr == state.isOver())
			fp.push_back(col);
		state.remove(col);
	}
	return fp;
}

// check if game is over or board is full before call this
string BoardAnalyse::analyse(const char plr, shortv& list) {
	shortv tempList, goodList, temp1, temp2, nonFull;
	vIter  col;

	state.nonFullColumn(nonFull);
	list = firstPoint(plr, nonFull);
	if (!list.empty())
		return "good";

	char opp = state.rPlayer(plr);
	list	 = firstPoint(opp, nonFull);
	if (list.size() > 1)
		return "bad";
	if (list.size() == 1) {
		state.add(plr, list[0]);
		temp1 = firstPoint(opp);
		state.remove(list[0]);
		if (!temp1.empty())
			return "bad";
		return "free";
	}

	// I'll go
	list = nonFull;
	col	 = list.begin();
	while (col != list.end()) {
		state.add(plr, *col);
		state.nonFullColumn(nonFull);
		tempList = firstPoint(opp, nonFull);
		if (!tempList.empty()) {
			state.remove(*col);
			col = list.erase(col);
			continue;
		}
		tempList = firstPoint(plr, nonFull);
		if (tempList.size() > 1) {
			goodList.push_back(*col);
		} else if (tempList.size() == 1) {
			state.add(opp, tempList[0]);
			state.nonFullColumn(nonFull);
			temp1 = firstPoint(plr, nonFull);
			if (!temp1.empty())
				goodList.push_back(*col);
			else {
				temp1 = firstPoint(opp, nonFull);
				if (temp1.size() > 1) {
					state.remove(tempList[0]);
					state.remove(*col);
					col = list.erase(col);
					continue;
				}
				if (temp1.size() == 1) {
					state.add(plr, temp1[0]);
					temp2 = firstPoint(opp);
					state.remove(temp1[0]);
					if (!temp2.empty()) {
						state.remove(tempList[0]);
						state.remove(*col);
						col = list.erase(col);
						continue;
					}
				}
			}
			state.remove(tempList[0]);
		}
		state.remove(*col);
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
// max depth = 9
string BoardAnalyse::returnMove(const char plr, shortv& list, const short depth) {
	string word = analyse(plr, list);
	if (word == "good" || word == "bad" || list.size() == 1)
		return word;

	/*********************************debug************************************/
	int addNumber1 = state.getAddNumber(), removeNumber1 = state.getRemoveNumber();
	/*********************************debug************************************/
	char   opp = state.rPlayer(plr);
	shortv list1, goodList;
	vIter  col = list.begin();

	// one big loop ahead of us
	while (col != list.end()) {
		state.add(plr, *col);
		word = analyse(opp, list1);
		if (word == "good") {
			state.remove(*col);
			col = list.erase(col);
			continue;
		} else if (word == "bad")
			goodList.push_back(*col);
		else {
			shortv list2;
			short  goodCount1 = 0;
			bool   isBad1	  = false;
			for (vIter col1 = list1.begin(); col1 != list1.end(); ++col1) {
				state.add(opp, *col1);
				word = analyse(plr, list2);
				if (word == "bad") { // bad
					state.remove(*col1);
					isBad1 = true;
					break; // goes straight to line197 if (col1!=list1.end())
				} else if (word == "good")
					++goodCount1;
				else {
					shortv list3;
					short  badCount2 = 0;
					short  isGood2	 = false;
					for (vIter col2 = list2.begin(); col2 != list2.end(); ++col2) {
						state.add(plr, *col2);
						word = analyse(opp, list3);
						if (word == "good") // bad for me
							++badCount2;
						else if (word == "bad") { // good
							state.remove(*col2);
							isGood2 = true;
							break;
						} else if (depth > 2) {
							shortv list4;
							short  goodCount3 = 0;
							bool   isBad3	  = false;
							for (vIter col3 = list3.begin(); col3 != list3.end(); ++col3) {
								state.add(opp, *col3);
								word = analyse(plr, list4);
								if (word == "bad") {  // bad
									state.remove(*col3);
									isBad3 = true;
									break;	// straight into 'if after while'
								} else if (word == "good")
									++goodCount3;
								else if (depth > 3) {
									shortv list5;
									bool   isGood4	 = false;
									short  badCount4 = 0;
									for (vIter col4 = list4.begin(); col4 != list4.end(); ++col4) {
										state.add(plr, *col4);
										word = analyse(opp, list5);
										if (word == "bad") {  // good
											isGood4 = true;
											state.remove(*col4);
											break;
										} else if (word == "good")
											++badCount4;
										else if (depth > 4) {
											shortv list6;
											bool   isBad5	  = false;
											short  goodCount5 = 0;
											for (vIter col5 = list5.begin(); col5 != list5.end(); ++col5) {
												state.add(opp, *col5);
												word = analyse(plr, list6);
												if (word == "good")
													++goodCount5;
												else if (word == "bad") {
													state.remove(*col5);
													isBad5 = true;
													break;
												} else if (depth > 5) {
													shortv list7;
													bool   isGood6	 = false;
													short  badCount6 = 0;
													for (vIter col6 = list6.begin(); col6 != list6.end(); ++col6) {
														state.add(plr, *col6);
														word = analyse(opp, list7);
														if (word == "bad") {  // good
															state.remove(*col6);
															isGood6 = true;
															break;
														} else if (word == "good") {
															++badCount6;
														} else if (depth > 6 && !list7.empty()) {
															// this is gonna be a disaster, it's just too much for a function
															shortv list8;
															bool   isBad7	  = false;
															short  goodCount7 = 0;
															for (vIter col7 = list7.begin(); col7 != list7.end(); ++col7) {
																state.add(opp, *col7);
																word = analyse(plr, list8);
																if (word == "good")
																	++goodCount7;
																else if (word == "bad") {
																	state.remove(*col7);
																	isBad7 = true;
																	break;
																} else if (depth > 7 && !list8.empty()) {
																	shortv list9;
																	bool   isGood8	 = false;
																	short  badCount8 = 0;
																	for (vIter col8 = list8.begin(); col8 != list8.end(); ++col8) {
																		state.add(plr, *col8);
																		word = analyse(opp, list9);
																		if (word == "good")
																			++badCount8;  // bad for me
																		else if (word == "bad") {
																			isGood8 = true;
																			state.remove(*col8);
																			break;
																		} else if (depth > 8 && !list9.empty()) {
																			shortv list10;
																			bool   isBad9	  = false;
																			short  goodCount9 = 0;
																			for (vIter col9 = list9.begin(); col9 != list9.end(); ++col9) {
																				state.add(opp, *col9);
																				word = analyse(plr, list10);
																				if (word == "good")
																					++goodCount9;
																				else if (word == "bad") {
																					state.remove(*col9);
																					isBad9 = true;
																					break;
																				} else
																					;
																				state.remove(*col9);
																			}
																			if (isBad9)
																				++badCount8;
																			else if (goodCount9 == list9.size() && goodCount9 != 0) {
																				isGood8 = true;
																				state.remove(*col8);
																				break;
																			}
																		}
																		state.remove(*col8);
																	}
																	if (isGood8)
																		++goodCount7;
																	else if (badCount8 == list8.size()) {
																		state.remove(*col7);
																		isBad7 = true;
																		break;
																	}
																}
																state.remove(*col7);
															}
															if (isBad7)
																++badCount6;
															else if (goodCount7 == list7.size()) {
																state.remove(*col6);
																isGood6 = true;
																break;
															}
														}
														state.remove(*col6);
													}
													if (isGood6)
														++goodCount5;
													else if (badCount6 == list6.size() && !list6.empty()) {
														state.remove(*col5);
														isBad5 = true;
														break;
													}
												}
												state.remove(*col5);
											}
											if (isBad5)
												++badCount4;
											else if (goodCount5 == list5.size() && !list5.empty()) {
												isGood4 = true;
												state.remove(*col4);
												break;
											}
										} else if (list5.size() == 1) {
											shortv list6;
											state.add(opp, list5[0]);
											word = analyse(plr, list6);
											if (word == "good") {
												state.remove(list5[0]);
												state.remove(*col4);
												isGood4 = true;
												break;
											} else if (word == "bad")
												++badCount4;
											// else if (list6.size() == 1)
											// 	;
											state.remove(list5[0]);
										}
										state.remove(*col4);
									}
									if (isGood4)
										++goodCount3;
									else if (badCount4 == list4.size() && !list4.empty()) {
										state.remove(*col3);
										isBad3 = true;
										break;	// straight into 'if after while'
									}
								} else if (list4.size() == 1) {
									state.add(plr, list4[0]);
									shortv list5;
									word = analyse(opp, list5);
									if (word == "good") {
										state.remove(list4[0]);
										state.remove(*col3);
										isBad3 = true;
										break;	// straight into 'if after while'
									} else if (word == "bad")
										++goodCount3;
									else if (list5.size() == 1) {
										shortv list6;
										state.add(opp, list5[0]);
										word = analyse(plr, list6);
										if (word == "bad") {
											state.remove(list5[0], list4[0], *col3);
											isBad3 = true;
											break;	// straight into 'if after while'
										} else if (word == "good")
											++goodCount3;
										state.remove(list5[0]);
									}
									state.remove(list4[0]);
								}
								state.remove(*col3);
							}
							if (isBad3)
								++badCount2;
							if (goodCount3 == list3.size() && !list3.empty()) {
								state.remove(*col2);
								isGood2 = true;
								break;
							}
						} else if (list3.size() == 1) {	 // if opp has to go here
							state.add(opp, list3[0]);
							shortv list4;
							word = analyse(plr, list4);
							if (word == "bad") {
								++badCount2;
							} else if (word == "good") {
								state.remove(list3[0]);
								state.remove(*col2);
								++goodCount1;
								break;
							} else if (list4.size() == 1) {
								state.add(plr, list4[0]);
								shortv list5;
								word = analyse(opp, list5);
								if (word == "good") {
									++badCount2;
								} else if (word == "bad") {
									state.remove(list4[0], list3[0], *col2);
									++goodCount1;
									break;
								} else if (list5.size() == 1) {
									state.add(opp, list5[0]);
									shortv list6;
									word = analyse(plr, list6);
									if (word == "bad") {
										++badCount2;
									} else if (word == "good") {
										printf("finally!, comething good!\n\n");
										state.remove(list5[0]);
										state.remove(list4[0], list3[0], *col2);
										++goodCount1;
										break;
									}
									state.remove(list5[0]);
								}
								state.remove(list4[0]);
							}
							state.remove(list3[0]);
						}
						state.remove(*col2);
					}
					if (badCount2 == list2.size() && !list2.empty()) { // if all bad, then opp go col1 is bad
						state.remove(*col1);
						isBad1 = true;
						break;
					}
					if (isGood2)
						++goodCount1;
				}
				state.remove(*col1);
			}
			if (isBad1) {
				state.remove(*col);
				col = list.erase(col);
				continue;
			}
			if (goodCount1 == list1.size() && !list1.empty())
				goodList.push_back(*col);
		}
		state.remove(*col);
		++col;
	}

	/*******************************debug**********************************/
	addNumber1	  = state.getAddNumber() - addNumber1;
	removeNumber1 = state.getRemoveNumber() - removeNumber1;
	// printf("addNumber=%d, removeNumber=%d\n", addNumber1, removeNumber1);
	if (addNumber1 != removeNumber1)
		throw runtime_error("add and remove didn't match!\n");
	/*******************************debug**********************************/
	if (!goodList.empty()) {
		list = goodList;
		return "good";
	}
	if (list.empty())
		return "bad";
	return "free";
}

// check if game is over or board is full before call this, given list must not
// be empty
// this is not a recursive function
// string BoardAnalyse::returnSituation(const char plr, shortv& list, short returnMoveDepth /*3*/, int recursiveCount /*0*/, int countTop /*3*/) {
// 	/****************************debug theory**********************************
// 	this recursiveSituation is merely an infinite version of returnMove. If you
// 	can see this, you will soon find a flaw in this function: there are no
// 	'break;'! in returnMove, I won't always prepare a goodCount and badCount, I
// 	always define a count and a isGood/isBad before going into a loop. Now, why
// 	should I define a goodList to track all the good col? I will never use them
// 	anyway! And in this case, most recursiveSituation is actually called by
// 	another recursiveSituation where 'another recursiveSituation doesn't care
// 	what our goodList are! Therefore, I wrote another version of
// 	recursiveSituation which doesn't take shortv as an argument, and
// 	change this formal recursiveSituation's name as returnSituation, and the
// 	newly defined recursiveSituationInline as recursiveSituation
// 	****************************debug theory**********************************/
// 	if (list.empty()) {
// 		throw runtime_error("recursiveSituation: given list is empty!\n");
// 		return "end";
// 	}

// 	// a loop
// 	vIter  col = list.begin();
// 	char   opp = state.rPlayer(plr);
// 	shortv goodList, list1;
// 	string word;
// 	while (col != list.end()) {
// 		state.add(plr, *col);
// 		word = returnMove(opp, list1, returnMoveDepth);
// 		if (word == "free") {
// 			if (!list1.empty())
// 				word = recursiveSituation(opp, list1, returnMoveDepth, recursiveCount, countTop);
// 			// else
// 			// 	printf("game is over, what do I do? ---- nothing I suppose.\n");
// 		}
// 		if (word == "good") {
// 			state.remove(*col);
// 			col = list.erase(col);
// 			continue;
// 		} else if (word == "bad")
// 			goodList.push_back(*col);
// 		state.remove(*col);
// 		++col;
// 	}
// 	if (!goodList.empty()) {
// 		list = goodList;
// 		return "good";
// 	} else if (list.empty())
// 		return "bad";
// 	return "free";
// }

// // shouldn't call this with an emptylist
// string BoardAnalyse::recursiveSituation(const char plr, shortv& list, short returnMoveDepth /*3*/, int recursiveCount /*0*/, int countTop /*3*/) {
// 	/*****************************debug theory*********************************
// 	Here is another version of recursiveSituation unfortunately, doesn't take
// 	shortv isn't gonna work, so here we are. Now I can use for loop and
// 	count and break! This will work faster than the last one.
// 	But the problem is, this one doesn't tell you which col is good, it just
// 	tells you this whole shortv is good or bad. So don't call this first.
// 	This is basically the old recursiveSituation copied here
// 	*****************************debug theory*********************************/
// 	/*****************************debug action*********************************/
// 	if (list.empty()) {
// 		throw runtime_error("recursiveSituation: given list is empty!\n");
// 		return "end";
// 	}
// 	/*****************************debug action*********************************/

// 	// main
// 	shortv list1;
// 	string word;
// 	short  badCount = 0;
// 	// or use isGood and ditch the col != list.end()
// 	vIter  col		= list.begin();
// 	for (; col != list.end(); ++col) {
// 		state.add(plr, *col);
// 		word = returnMove(state.rPlayer(plr), list1, returnMoveDepth);
// 		if (word == "free") {
// 			if (!list1.empty())
// 				word = recursiveSituation(state.rPlayer(plr), list1, returnMoveDepth, recursiveCount, countTop);
// 			// else /*debug - delete this else for performance considerations*/
// 			// 	printf("game is over, what do I do? ---- nothing I suppose.\n");
// 		}
// 		if (word == "good")
// 			++badCount;
// 		else if (word == "bad") {
// 			state.remove(*col);
// 			break;
// 		}
// 		state.remove(*col);
// 	}
// 	if (col != list.end())
// 		// this is actually based on a fact that list.empty() == false, which is
// 		// not always the case if the function isn't called properly
// 		return "good";
// 	else if (badCount == list.size())
// 		return "bad";
// 	return "free";
// }

int BoardAnalyse::respond(const char plr, oneMove& thisMove, bool showCal, bool showTime, bool starsOn) {
	/*
	 * Maybe it's better to move it to boardInterface
	 */
	// list for myself
	shortv list, oppList, nonFullList;
	long long timeUsed		  = 0;
	short	  returnMoveDepth = 6;
	string word;

	// pre-test
	state.nonFullColumn(nonFullList);
	if (nonFullList.empty())
		throw runtime_error("call respond with full board!\n");
	if (state.isOver() == plr || state.isOver() == state.rPlayer(plr))
		throw runtime_error("call respond with ended game!\n");

	// analyse
	if (starsOn && nonFullList.size() > 4)
		state.areaTopTransform();
	else
		printf("Stars have lost their powers\n");
	do {
		timeUsed = returnTime(plr, list, returnMoveDepth++, word);
	} while (word == "free" && timeUsed < 81 && returnMoveDepth < 10);
	// if (!list.empty() && returnMoveDepth == 10 && timeUsed!=0) {
	// 	int returnSituationDepth = 1;
	// 	printf("This recursive is entered");
	// 	while (timeUsed < 32 && returnSituationDepth < 6) {
	// 		timeUsed = recursiveTime(plr, list, returnMoveDepth, returnSituationDepth, word);
	// 		++returnSituationDepth;
	// 	}
	// 	cout << "ReturnSituationDepth = " << returnSituationDepth << endl;
	// }
	state.areaTopRestore();

	// in case something unpleasent happens:
	if (starsOn && returnMoveDepth > 8 && timeUsed < 81 && nonFullList.size() < 12) {
		returnMoveDepth = 2;
		while (timeUsed < 64 && returnMoveDepth < 10) {
			timeUsed = returnTime(plr, list, ++returnMoveDepth, word);
		}
		// this opp list is for the random suggestion functions
		if (returnMoveDepth > 2)
			returnMove(state.rPlayer(plr), oppList, returnMoveDepth - 1);
		else
			returnMove(state.rPlayer(plr), oppList, returnMoveDepth);
		cout << "\treturnMoveDepth without stars = " << returnMoveDepth << endl;
	}
	else if (starsOn && list.size() > 4)
		cout << "\treturnMoveDepth with stars = " << returnMoveDepth << endl;
	else  // starsOn == false
		cout << "\treturnMoveDepth without stars = " << returnMoveDepth << endl;

	// show info if needed
	if (showCal) {
		cout << "\tword = " << word << "\n\tlist = [ ";
		for (short c : list)
			cout << c << " ";
		printf("]\n");
	}
	if (showTime)
		cout << "\tcalculate time used: " << timeUsed << " ms\n";

	// record, suggest and return
	thisMove.word = word;
	thisMove.list = list;
	thisMove.time = timeUsed;
	if (word != "bad")
		return state.randomSuggestion(plr, list, oppList);
	else if (word == "bad") {
		word = analyse(plr, list);
		if (word == "bad")
			return state.randomMove();
		else
			return state.randomSuggestion(plr, list, oppList);
	}
	else
		throw runtime_error("wrong word returned in respond\n");
	return 0;
}

long long BoardAnalyse::returnTime(const char plr, shortv& list, const short returnMoveDepth, string& word) {
	long long				 timeUsed;
	system_clock::time_point start, end;
	start		 = system_clock::now();
	word		 = returnMove(plr, list, returnMoveDepth);
	end			 = system_clock::now();
	auto elapsed = duration_cast<milliseconds>(end - start);
	timeUsed	 = elapsed.count();
	return timeUsed;
}

// long BoardAnalyse::recursiveTime(const char plr, shortv& list, const short returnMoveDepth, int countTop, string& word) { ;
// 	long					 timeUsed;
// 	system_clock::time_point start, end;
// 	start		 = system_clock::now();
// 	word		 = returnSituation(plr, list, returnMoveDepth, 0, countTop);
// 	end			 = system_clock::now();
// 	auto elapsed = duration_cast<milliseconds>(end - start);
// 	timeUsed	 = elapsed.count();
// 	return timeUsed;
// }
