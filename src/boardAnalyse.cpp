/*-- encoding: GBK --*/
#include "boardAnalyse.h"

void BoardAnalyse::go(const char plr, const short move) {
#ifdef STARS_DEBUG_INFO
	if (state.colIsFull(move))
		throw logic_error("Error: trying to add to a full column");
#endif
	state.add(plr, move);
}

void BoardAnalyse::reverse(const short column) {
#ifdef STARS_DEBUG_INFO
	if (state.colIsEmpty(column))
		throw logic_error("Error: tying to reverse an empty column");
#endif
	state.remove(column);
}

shortv BoardAnalyse::firstPoint(const char plr, shortv& nonFullList) {
	shortv firstPointList;
	for (short col : nonFullList) {
		state.add(plr, col);
		if (plr == state.isOver())
			firstPointList.push_back(col);
		state.remove(col);
	}
	return firstPointList;
}

// check if game is over before call this
// won't return free if list is empty, relax
string BoardAnalyse::analyse(const char plr, shortv& list) {
#ifdef STARS_DEBUG_INFO
	if (state.isOver() != 'N')
		throw logic_error("game is over, yet analyse is called");
#endif
	shortv nonFull, temp1;

	state.nonFullColumn(nonFull);
	if (nonFull.empty())
		return "bad";
	list = firstPoint(plr, nonFull);
	if (!list.empty())
		return "good";

	char opp = state.rPlayer(plr);
	list	 = firstPoint(opp, nonFull);
	if (list.size() > 1)
		return "bad";
	if (list.size() == 1) {
		state.add(plr, list[0]);
		state.sweepFullColumn(nonFull, list[0]);
		temp1 = firstPoint(opp, nonFull);
		state.remove(list[0]);
		if (!temp1.empty())
			return "bad";
		return "free";
	}

	// I'll go
	shortv tempList, goodList;
	vIter  col = list.begin();
	list	   = nonFull;
	while (col != list.end()) {
		state.add(plr, *col);
		state.nonFullColumn(nonFull);
		tempList = firstPoint(opp, nonFull);
		if (!tempList.empty()) { // bad
			state.remove(*col);
			col = list.erase(col);
			continue;
		}
		tempList = firstPoint(plr, nonFull);
		if (tempList.size() > 1) // good
			goodList.push_back(*col);
		else if (tempList.size() == 1) {
			state.add(opp, tempList[0]);
			// now it's my turn
			state.sweepFullColumn(nonFull, tempList[0]);
			temp1 = firstPoint(opp, nonFull);
			if (temp1.size() > 1) {
				state.remove(tempList[0]);
				state.remove(*col);
				col = list.erase(col);
				continue;
			}
			if (temp1.size() == 1) {
				shortv tempOpponent, tempPlayer;
				state.add(plr, temp1[0]); // all the way size()==1 to here
				state.sweepFullColumn(nonFull, temp1[0]);
				tempOpponent = firstPoint(opp, nonFull);
				tempPlayer	 = firstPoint(plr, nonFull);
				state.remove(temp1[0]);
				// opponent's turn
				if (!tempOpponent.empty()) {
					state.remove(tempList[0]);
					state.remove(*col);
					col = list.erase(col);
					continue;
				}
				// this is new
				if (tempPlayer.size() > 1)
					goodList.push_back(*col);
				//
			}
			else {
				temp1 = firstPoint(plr, nonFull);
				if (!temp1.empty()) // good
					goodList.push_back(*col);
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

string BoardAnalyse::oneMoveAnalyse(const char plr, const short col,
	const short depth, const short maxDepth) {
	// only analyse one Move
	char  opp = state.rPlayer(plr);
	string word;
	shortv list;

	state.add(plr, col);
	word = analyse(opp, list);
	if (word == "good") {
		word = "bad";
	}
	else if (word == "bad") {
		word = "good";
	}
	else if (list.size() == 1 && depth < maxDepth) {
		word = oneMoveAnalyse(opp, list[0], depth + 1, maxDepth);
		if (word == "good")
			word = "bad";
		else if (word == "bad")
			word = "good";
	}
	state.remove(col);
	return word;
}

string BoardAnalyse::oneMoveAnalyseDebug(const char plr, const short col,
	short goodNode, short badNode, const short depth, const short maxDepth) {
	// only analyse one Move
	char  opp = state.rPlayer(plr);
	short freeNode = -2;
	string word;
	shortv list;

	routes.add(col);
	routes.forward(col);
	state.add(plr, col);
	word = analyse(opp, list);
	if (word == "good") {
		routes.add(badNode);
		word = "bad";
	}
	else if (word == "bad") {
		routes.add(goodNode);
		word = "good";
	}
	else if (list.size() == 1 && depth < maxDepth) {
		word = oneMoveAnalyseDebug(opp, list[0], badNode, goodNode, depth + 1, maxDepth);
		if (word == "good")
			word = "bad";
		else if (word == "bad")
			word = "good";
		else
			word = "free";
	}
	else
		routes.add(routes.freeNode);
	state.remove(col);
	routes.backward();
	return word;
}

// check if game is over before call this
// max depth = 9
// without route
string BoardAnalyse::returnMove(const char plr, shortv& list, const short depth) {
#ifdef STARS_DEBUG_INFO
	if (state.isOver() != 'N' || state.boardIsFull())
		throw logic_error("game is over or board is full, yet returnMove is called");
	int addNumber1 = state.getAddNumber(), removeNumber1 = state.getRemoveNumber();
#endif
	string word = analyse(plr, list);
	if (word == "good" || word == "bad" || list.size() == 1)
		return word;

	char   opp = state.rPlayer(plr);
	shortv list1, goodList;
	vIter  col = list.begin();

	// one big loop ahead of us
	for (; col != list.end();) {
		state.add(plr, *col);
#ifdef STARS_DEBUG_INFO
		if (!state.match())
			throw logic_error("top and real top didn't match");
#endif
		word = analyse(opp, list1);
		if (word == "good") {
			state.remove(*col);
			col = list.erase(col);
			continue;
		}
		else if (word == "bad") {
			goodList.push_back(*col);
		}
		else {
#ifdef STARS_DEBUG_INFO
			if (list.empty())
				throw logic_error("list is empty!");
#endif // STARS_DEBUG_INFO
			shortv list2;
			short  goodCount1 = 0;
			bool   isBad1	  = false;

			vIter col1 = list1.begin();
			for (; col1 != list1.end(); ++col1) {
				state.add(opp, *col1);
				word = analyse(plr, list2);
				if (word == "bad") {
					state.remove(*col1);
					isBad1 = true;
					break;
				}
				else if (word == "good") {
					++goodCount1;
				}
				else {
#ifdef STARS_DEBUG_INFO
				if (list2.empty())
					throw logic_error("list2 is empty!");
#endif // STARS_DEBUG_INFO
					shortv list3;
					short  badCount2 = 0;
					short  isGood2	 = false;
					vIter  col2		 = list2.begin();
					for (; col2 != list2.end(); ++col2) {
						state.add(plr, *col2);
						word = analyse(opp, list3);
						if (word == "good") {
							++badCount2;
						}
						else if (word == "bad") { // good
							state.remove(*col2);
							isGood2 = true;
							break;
						}
						else if (depth > 2) {
#ifdef STARS_DEBUG_INFO
						if (list3.empty())
							throw logic_error("list3 is empty!");
#endif // STARS_DEBUG_INFO
							shortv list4;
							short  goodCount3 = 0;
							bool   isBad3	  = false;
							vIter col3 = list3.begin();
							for (; col3 != list3.end(); ++col3) {
								state.add(opp, *col3);
								word = analyse(plr, list4);
								if (word == "bad") {  // bad
									state.remove(*col3);
									isBad3 = true;
									break;	// straight into 'if after while'
								}
								else if (word == "good") {
									++goodCount3;
								}
								else if (depth > 3) {
#ifdef STARS_DEBUG_INFO
									if (list4.empty())
										throw logic_error("list4 is empty!");
#endif // STARS_DEBUG_INFO
									shortv list5;
									bool   isGood4	 = false;
									short  badCount4 = 0;
									vIter col4 = list4.begin();
									for (; col4 != list4.end(); ++col4) {
										state.add(plr, *col4);
										word = analyse(opp, list5);
										if (word == "bad") {  // good
											isGood4 = true;
											state.remove(*col4);
											break;
										}
										else if (word == "good") {
											++badCount4;
										}
										else if (depth > 4) {
#ifdef STARS_DEBUG_INFO
											if (list5.empty())
												throw logic_error("list5 is empty!");
#endif // STARS_DEBUG_INFO
											shortv list6;
											bool   isBad5	  = false;
											short  goodCount5 = 0;
											vIter col5 = list5.begin();
											for (; col5 != list5.end(); ++col5) {
												state.add(opp, *col5);
												word = analyse(plr, list6);
												if (word == "good") {
													++goodCount5;
												}
												else if (word == "bad") {
													state.remove(*col5);
													isBad5 = true;
													break;
												}
												else if (depth > 5) {
#ifdef STARS_DEBUG_INFO
													if (list6.empty())
														throw logic_error("list6 is empty!");
#endif // STARS_DEBUG_INFO
													shortv list7;
													bool   isGood6	 = false;
													short  badCount6 = 0;
													vIter  col6		 = list6.begin();
													for (; col6 != list6.end(); ++col6) {
														state.add(plr, *col6);
														word = analyse(opp, list7);
														if (word == "bad") {  // good
															state.remove(*col6);
															isGood6 = true;
															break;
														}
														else if (word == "good") {
															++badCount6;
														}
														else if (depth > 6 ) {
#ifdef STARS_DEBUG_INFO
															if (list7.empty())
																throw logic_error("list7 is empty!");
#endif // STARS_DEBUG_INFO
															shortv list8;
															bool   isBad7	  = false;
															short  goodCount7 = 0;
															vIter col7 = list7.begin();
															for (; col7 != list7.end(); ++col7) {
																state.add(opp, *col7);
																word = analyse(plr, list8);
																if (word == "good") {
																	++goodCount7;
																}
																else if (word == "bad") {
																	state.remove(*col7);
																	isBad7 = true;
																	break;
																}
																else if (depth > 7) {
#ifdef STARS_DEBUG_INFO
																	if (list8.empty())
																		throw logic_error("list8 is empty!");
#endif // STARS_DEBUG_INFO
																	shortv list9;
																	bool   isGood8	 = false;
																	short  badCount8 = 0;
																	vIter col8 = list8.begin();
																	for (; col8 != list8.end(); ++col8) {
																		state.add(plr, *col8);
																		word = analyse(opp, list9);
																		if (word == "good") {
																			++badCount8;  // bad for me
																		}
																		else if (word == "bad") {
																			isGood8 = true;
																			state.remove(*col8);
																			break;
																		}
																		else if (depth > 8) {
#ifdef STARS_DEBUG_INFO
																			if (list9.empty())
																				throw logic_error("list9 is empty!");
#endif // STARS_DEBUG_INFO
																			shortv list10;
																			bool   isBad9	  = false;
																			short  goodCount9 = 0;
																			vIter col9 = list9.begin();
																			for (; col9 != list9.end(); ++col9) {
																				state.add(opp, *col9);
																				word = analyse(plr, list10);
																				if (word == "good") {
																					++goodCount9;
																				}
																				else if (word == "bad") {
																					state.remove(*col9);
																					isBad9 = true;
																					break;
																				}
																				else if (list10.size() == 1) {
																					word = oneMoveAnalyse(plr, list10[0], 0);
																					if (word == "good")
																						++goodCount9;
																					else if (word == "bad") {
																						state.remove(*col9);
																						isBad9 = true;
																						break;
																					}
																				}
																				state.remove(*col9);
																			}
																			if (isBad9)
																				++badCount8;
																			else if (goodCount9 == list9.size()) {
																				isGood8 = true;
																				state.remove(*col8);
																				break;
																			}
																		}
																		else if (list9.size() == 1) {
																			word = oneMoveAnalyse(opp, list9[0], 0);
																			if (word == "good")
																				++badCount8;
																			else if (word == "bad") {
																				state.remove(*col8);
																				isGood8 = true;
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
																else if (list8.size() == 1) {
																	word = oneMoveAnalyse(plr, list8[0], 0);
																	if (word == "good")
																		++goodCount7;
																	else if (word == "bad") {
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
														else if (list7.size() == 1) {
															word = oneMoveAnalyse(opp, list7[0], 0);
															if (word == "good")
																++badCount6;
															else if (word == "bad") {
																state.remove(*col6);
																isGood6 = true;
																break;
															}
														}
														state.remove(*col6);
													}
													if (isGood6)
														++goodCount5;
													else if (badCount6 == list6.size()) {
														state.remove(*col5);
														isBad5 = true;
														break;
													}
												}
												else if (list6.size() == 1) {
													word = oneMoveAnalyse(plr, list6[0], 0);
													if (word == "good")
														++goodCount5;
													else if (word == "bad") {
														state.remove(*col5);
														isBad5 = true;
														break;
													}
												}
												state.remove(*col5);
											}
											if (isBad5)
												++badCount4;
											else if (goodCount5 == list5.size()) {
												isGood4 = true;
												state.remove(*col4);
												break;
											}
										}
										else if (list5.size() == 1) {
											word = oneMoveAnalyse(opp, list5[0], 0);
											if (word == "bad") {
												state.remove(*col4);
												isGood4 = true;
												break;
											}
											else if (word == "good")
												++badCount4;
										}
										state.remove(*col4);
									}
									if (isGood4)
										++goodCount3;
									else if (badCount4 == list4.size()) {
										state.remove(*col3);
										isBad3 = true;
										break;	// straight into 'if after while'
									}
								}
								else if (list4.size() == 1) {
									word = oneMoveAnalyse(plr, list4[0], 0);
									if (word == "good")
										++goodCount3;
									else if (word == "bad") {
										state.remove(*col3);
										isBad3 = true;
										break;
									}
								}
								state.remove(*col3);
							}
							if (isBad3)
								++badCount2;
							else if (goodCount3 == list3.size()) {
								state.remove(*col2);
								isGood2 = true;
								break;
							}
						}
						else if (list3.size() == 1) {
							word = oneMoveAnalyse(opp, list3[0], 0);
							if (word == "bad") {
								state.remove(*col2);
								isGood2 = true;
								break;
							}
							else if (word == "good")
								++badCount2;
						}
						state.remove(*col2);
					}
					if (badCount2 == list2.size()) {
						state.remove(*col1);
						isBad1 = true;
						break;
					}
					else if (isGood2)
						++goodCount1;
				}
				state.remove(*col1);
			}
			if (isBad1) {
				state.remove(*col);
				col = list.erase(col);
				continue;
			}
			else if (goodCount1 == list1.size())
				goodList.push_back(*col);
		}
		state.remove(*col);
		++col;
	}

#ifdef STARS_DEBUG_INFO
	addNumber1	  = state.getAddNumber() - addNumber1;
	removeNumber1 = state.getRemoveNumber() - removeNumber1;
	// printf("addNumber=%d, removeNumber=%d\n", addNumber1, removeNumber1);
	if (addNumber1 != removeNumber1)
		throw logic_error("add and remove didn't match");
	if (!state.match())
		throw logic_error("top and real top didn't match");
#endif
	if (!goodList.empty()) {
		list = goodList;
		return "good";
	}
	if (list.empty())
		return "bad";
	return "free";
}

string BoardAnalyse::returnMoveDebug(const char plr, shortv& list, const short depth) {
#ifdef STARS_DEBUG_INFO
	if (state.isOver() != 'N')
		throw logic_error("game is over, yet returnMove is called");
	int addNumber1 = state.getAddNumber(), removeNumber1 = state.getRemoveNumber();
#endif
	string word = analyse(plr, list);
	if (word == "good" || word == "bad" || list.size() == 1)
		return word;

	char   opp = state.rPlayer(plr);
	shortv list1, goodList;
	vIter  col = list.begin();
	short goodNode = -1, badNode = 0, freeNode = -2;

	routes.clear();
	routes.add(list);
	// one big loop ahead of us
	for (routes.forward(*col); col != list.end(); routes.nextNode()) {
		state.add(plr, *col);
#ifdef STARS_DEBUG_INFO
		if (!state.match())
			throw logic_error("top and real top didn't match");
#endif
		word = analyse(opp, list1);
		if (word == "good") {
			routes.add(badNode);
			state.remove(*col);
			col = list.erase(col);
			continue;
		}
		else if (word == "bad") {
			routes.add(goodNode);
			goodList.push_back(*col);
		}
		else {
#ifdef STARS_DEBUG_INFO
			if (list.empty())
				throw logic_error("list is empty!");
#endif // STARS_DEBUG_INFO
			shortv list2;
			short  goodCount1 = 0;
			bool   isBad1	  = false;

			routes.add(list1);
			vIter col1 = list1.begin();
			for (routes.forward(*col1); col1 != list1.end(); ++col1, routes.nextNode()) {
				state.add(opp, *col1);
				word = analyse(plr, list2);
				if (word == "bad") {
					routes.add(badNode);
					state.remove(*col1);
					isBad1 = true;
					break;
				}
				else if (word == "good") {
					routes.add(goodNode);
					++goodCount1;
				}
				else {
#ifdef STARS_DEBUG_INFO
				if (list2.empty())
					throw logic_error("list2 is empty!");
#endif // STARS_DEBUG_INFO
					shortv list3;
					short  badCount2 = 0;
					short  isGood2	 = false;
					vIter  col2		 = list2.begin();
					routes.add(list2);
					for (routes.forward(*col2); col2 != list2.end(); ++col2, routes.nextNode()) {
						state.add(plr, *col2);
						word = analyse(opp, list3);
						if (word == "good") {
							routes.add(badNode);
							++badCount2;
						}
						else if (word == "bad") { // good
							routes.add(goodNode);
							state.remove(*col2);
							isGood2 = true;
							break;
						}
						else if (depth > 2) {
#ifdef STARS_DEBUG_INFO
						if (list3.empty())
							throw logic_error("list3 is empty!");
#endif // STARS_DEBUG_INFO
							shortv list4;
							short  goodCount3 = 0;
							bool   isBad3	  = false;
							vIter col3 = list3.begin();
							routes.add(list3);
							for (routes.forward(*col3); col3 != list3.end(); ++col3, routes.nextNode()) {
								state.add(opp, *col3);
								word = analyse(plr, list4);
								if (word == "bad") {  // bad
									routes.add(badNode);
									state.remove(*col3);
									isBad3 = true;
									break;	// straight into 'if after while'
								}
								else if (word == "good") {
									routes.add(goodNode);
									++goodCount3;
								}
								else if (depth > 3) {
#ifdef STARS_DEBUG_INFO
									if (list4.empty())
										throw logic_error("list4 is empty!");
#endif // STARS_DEBUG_INFO
									shortv list5;
									bool   isGood4	 = false;
									short  badCount4 = 0;
									vIter col4 = list4.begin();
									routes.add(list4);
									for (routes.forward(*col4); col4 != list4.end(); ++col4, routes.nextNode()) {
										state.add(plr, *col4);
										word = analyse(opp, list5);
										if (word == "bad") {  // good
											routes.add(goodNode);
											isGood4 = true;
											state.remove(*col4);
											break;
										}
										else if (word == "good") {
											routes.add(badNode);
											++badCount4;
										}
										else if (depth > 4) {
#ifdef STARS_DEBUG_INFO
											if (list5.empty())
												throw logic_error("list5 is empty!");
#endif // STARS_DEBUG_INFO
											shortv list6;
											bool   isBad5	  = false;
											short  goodCount5 = 0;
											vIter col5 = list5.begin();
											routes.add(list5);
											for (routes.forward(*col5); col5 != list5.end(); ++col5, routes.nextNode()) {
												state.add(opp, *col5);
												word = analyse(plr, list6);
												if (word == "good") {
													routes.add(goodNode);
													++goodCount5;
												}
												else if (word == "bad") {
													routes.add(badNode);
													state.remove(*col5);
													isBad5 = true;
													break;
												}
												else if (depth > 5) {
#ifdef STARS_DEBUG_INFO
													if (list6.empty())
														throw logic_error("list6 is empty!");
#endif // STARS_DEBUG_INFO
													shortv list7;
													bool   isGood6	 = false;
													short  badCount6 = 0;
													vIter  col6		 = list6.begin();
													routes.add(list6);
													for (routes.forward(*col6); col6 != list6.end(); ++col6, routes.nextNode()) {
														state.add(plr, *col6);
														word = analyse(opp, list7);
														if (word == "bad") {  // good
															routes.add(goodNode);
															state.remove(*col6);
															isGood6 = true;
															break;
														}
														else if (word == "good") {
															routes.add(badNode);
															++badCount6;
														}
														else if (depth > 6) {
#ifdef STARS_DEBUG_INFO
															if (list7.empty())
																throw logic_error("list7 is empty!");
#endif // STARS_DEBUG_INFO
															// this is gonna be a disaster, it's just too much for a function
															shortv list8;
															bool   isBad7	  = false;
															short  goodCount7 = 0;
															vIter col7 = list7.begin();
															routes.add(list7);
															for (routes.forward(*col7); col7 != list7.end(); ++col7, routes.nextNode()) {
																state.add(opp, *col7);
																word = analyse(plr, list8);
																if (word == "good") {
																	routes.add(goodNode);
																	++goodCount7;
																}
																else if (word == "bad") {
																	routes.add(badNode);
																	state.remove(*col7);
																	isBad7 = true;
																	break;
																}
																else if (depth > 7) {
#ifdef STARS_DEBUG_INFO
																	if (list8.empty())
																		throw logic_error("list8 is empty!");
#endif // STARS_DEBUG_INFO
																	shortv list9;
																	bool   isGood8	 = false;
																	short  badCount8 = 0;
																	vIter col8 = list8.begin();
																	routes.add(list8);
																	for (routes.forward(*col8); col8 != list8.end(); ++col8, routes.nextNode()) {
																		state.add(plr, *col8);
																		word = analyse(opp, list9);
																		if (word == "good") {
																			routes.add(badNode);
																			++badCount8;  // bad for me
																		}
																		else if (word == "bad") {
																			routes.add(goodNode);
																			isGood8 = true;
																			state.remove(*col8);
																			break;
																		}
																		else if (depth > 8) {
#ifdef STARS_DEBUG_INFO
																			if (list9.empty())
																				throw logic_error("list9 is empty!");
#endif // STARS_DEBUG_INFO
																			shortv list10;
																			bool   isBad9	  = false;
																			short  goodCount9 = 0;
																			vIter col9 = list9.begin();
																			routes.add(list9);
																			for (routes.forward(*col9); col9 != list9.end(); ++col9, routes.nextNode()) {
																				state.add(opp, *col9);
																				word = analyse(plr, list10);
																				if (word == "good") {
																					routes.add(goodNode);
																					++goodCount9;
																				}
																				else if (word == "bad") {
																					routes.add(badNode);
																					state.remove(*col9);
																					isBad9 = true;
																					break;
																				}
																				else if (list10.size() == 1) {
																					word = oneMoveAnalyseDebug(plr, list10[0], goodNode, badNode, 0);
																					if (word == "good")
																						++goodCount9;
																					else if (word == "bad") {
																						state.remove(*col9);
																						isBad9 = true;
																						break;
																					}
																				}
																				else
																					routes.add(freeNode);
																				state.remove(*col9);
																			}
																			routes.backward();
																			if (isBad9)
																				++badCount8;
																			else if (goodCount9 == list9.size()) {
																				isGood8 = true;
																				state.remove(*col8);
																				break;
																			}
																		}
																		else if (list9.size() == 1) {
																			word = oneMoveAnalyseDebug(opp, list9[0], badNode, goodNode, 0);
																			if (word == "good")
																				++badCount8;
																			else if (word == "bad") {
																				state.remove(*col8);
																				isGood8 = true;
																				break;
																			}
																		}
																		else
																			routes.add(freeNode);
																		state.remove(*col8);
																	}
																	routes.backward();
																	if (isGood8)
																		++goodCount7;
																	else if (badCount8 == list8.size()) {
																		state.remove(*col7);
																		isBad7 = true;
																		break;
																	}
																}
																else if (list8.size() == 1) {
																	word = oneMoveAnalyseDebug(plr, list8[0], goodNode, badNode, 0);
																	if (word == "good")
																		++goodCount7;
																	else if (word == "bad") {
																		state.remove(*col7);
																		isBad7 = true;
																		break;
																	}
																}
																else
																	routes.add(freeNode);
																state.remove(*col7);
															}
															routes.backward();
															if (isBad7)
																++badCount6;
															else if (goodCount7 == list7.size()) {
																state.remove(*col6);
																isGood6 = true;
																break;
															}
														}
														else if (list7.size() == 1) {
															word = oneMoveAnalyseDebug(opp, list7[0], badNode, goodNode, 0);
															if (word == "good")
																++badCount6;
															else if (word == "bad") {
																state.remove(*col6);
																isGood6 = true;
																break;
															}
														}
														else
															routes.add(freeNode);
														state.remove(*col6);
													}
													routes.backward();
													if (isGood6)
														++goodCount5;
													else if (badCount6 == list6.size()) {
														state.remove(*col5);
														isBad5 = true;
														break;
													}
												}
												else if (list6.size() == 1) {
													word = oneMoveAnalyseDebug(plr, list6[0], goodNode, badNode, 0);
													if (word == "good")
														++goodCount5;
													else if (word == "bad") {
														state.remove(*col5);
														isBad5 = true;
														break;
													}
												}
												else
													routes.add(freeNode);
												state.remove(*col5);
											}
											routes.backward();
											if (isBad5)
												++badCount4;
											else if (goodCount5 == list5.size()) {
												isGood4 = true;
												state.remove(*col4);
												break;
											}
										}
										else if (list5.size() == 1) {
											word = oneMoveAnalyseDebug(opp, list5[0], badNode, goodNode, 0);
											if (word == "bad") {
												state.remove(*col4);
												isGood4 = true;
												break;
											}
											else if (word == "good")
												++badCount4;
										}
										else
											routes.add(freeNode);
										state.remove(*col4);
									}
									routes.backward();
									if (isGood4)
										++goodCount3;
									else if (badCount4 == list4.size()) {
										state.remove(*col3);
										isBad3 = true;
										break;	// straight into 'if after while'
									}
								}
								else if (list4.size() == 1) {
									word = oneMoveAnalyseDebug(plr, list4[0], goodNode, badNode, 0);
									if (word == "good")
										++goodCount3;
									else if (word == "bad") {
										state.remove(*col3);
										isBad3 = true;
										break;
									}
								}
								else
									routes.add(freeNode);
								state.remove(*col3);
							}
							routes.backward();
							if (isBad3)
								++badCount2;
							else if (goodCount3 == list3.size()) {
								state.remove(*col2);
								isGood2 = true;
								break;
							}
						}
						else if (list3.size() == 1) {
							word = oneMoveAnalyseDebug(opp, list3[0], badNode, goodNode, 0);
							if (word == "bad") {
								state.remove(*col2);
								isGood2 = true;
								break;
							}
							else if (word == "good")
								++badCount2;
						}
						else
							routes.add(freeNode);
						state.remove(*col2);
					}
					routes.backward();
					if (badCount2 == list2.size()) {
						state.remove(*col1);
						isBad1 = true;
						break;
					}
					else if (isGood2)
						++goodCount1;
				}
				state.remove(*col1);
			}
			routes.backward();
			if (isBad1) {
				state.remove(*col);
				col = list.erase(col);
				continue;
			}
			else if (goodCount1 == list1.size())
				goodList.push_back(*col);
		}
		state.remove(*col);
		++col;
	}
	routes.backward();

#ifdef STARS_DEBUG_INFO
	addNumber1	  = state.getAddNumber() - addNumber1;
	removeNumber1 = state.getRemoveNumber() - removeNumber1;
	// printf("addNumber=%d, removeNumber=%d\n", addNumber1, removeNumber1);
	if (addNumber1 != removeNumber1)
		throw logic_error("add and remove didn't match");
	if (!routes.match())
		throw logic_error("forward and backward didn't match");
	if (!state.match())
		throw logic_error("top and real top didn't match");
#endif
	if (!goodList.empty()) {
		list = goodList;
		return "good";
	}
	if (list.empty())
		return "bad";
	return "free";
}

#ifdef STARS_ADVANCED_FUNCTIONS
// check if game is over or board is full before call this, given list must not
// be empty
// this is not a recursive function
string BoardAnalyse::returnSituation(const char plr, shortv& list, short returnMoveDepth /*3*/, int recursiveCount /*0*/, int countTop /*3*/) {
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
#ifdef STARS_DEBUG_INFO
	if (list.empty()) {
		throw logic_error("recursiveSituation: given list is empty");
		return "end";
	}
#endif

	// a loop
	vIter  col = list.begin();
	char   opp = state.rPlayer(plr);
	shortv goodList, list1;
	string word;
	while (col != list.end()) {
		state.add(plr, *col);
		word = returnMove(opp, list1, returnMoveDepth);
		if (word == "free") {
			word = recursiveSituation(opp, list1, returnMoveDepth, recursiveCount, countTop);
		}
		if (word == "good") {
			state.remove(*col);
			col = list.erase(col);
			continue;
		}
		else if (word == "bad")
			goodList.push_back(*col);
		state.remove(*col);
		++col;
	}
	if (!goodList.empty()) {
		list = goodList;
		return "good";
	}
	else if (list.empty())
		return "bad";
	return "free";
}

// shouldn't call this with an emptylist
string BoardAnalyse::recursiveSituation(const char plr, shortv& list,
	short returnMoveDepth /*3*/, int recursiveCount /*0*/, int countTop /*3*/) {
	/*****************************debug theory*********************************
	Here is another version of recursiveSituation unfortunately, doesn't take
	shortv isn't gonna work, so here we are. Now I can use for loop and
	count and break! This will work faster than the last one.
	But the problem is, this one doesn't tell you which col is good, it just
	tells you this whole shortv is good or bad. So don't call this first.
	This is basically the old recursiveSituation copied here
	*****************************debug theory*********************************/
#ifdef STARS_DEBUG_INFO
	if (list.empty()) {
		throw logic_error("recursiveSituation: given list is empty");
		return "end";
	}
#endif

	// main
	char   opp	  = state.rPlayer(plr);
	bool   isGood = false;
	shortv list1;
	string word;
	short  badCount = 0;
	// or use isGood and ditch the col != list.end()
	vIter  col		= list.begin();
	for (; col != list.end(); ++col) {
		state.add(plr, *col);
		word = returnMove(opp, list1, returnMoveDepth);
		if (word == "free")
			word = recursiveSituation(opp, list1, returnMoveDepth,
				recursiveCount, countTop);
		if (word == "good")
			++badCount;
		else if (word == "bad") {
			isGood = true;
			state.remove(*col);
			break;
		}
		state.remove(*col);
	}
	if (isGood)
		return "good";
	if (badCount == list.size())
		return "bad";
	return "free";
}

#endif // STARS_ADVANCED_FUNCTIONS

int BoardAnalyse::respond(const char plr, oneMove& thisMove, bool showCal,
	bool showTime, bool starsOn, bool trackRoute) {
	// list for myself
	shortv plrList, oppList, nonFullList;
	long long timeUsed		  = 0;
	short	  returnMoveDepth = 2;
	string	  word;

	// pre-test
	state.nonFullColumn(nonFullList);
#ifdef STARS_DEBUG_INFO
	if (nonFullList.empty())
		throw logic_error("call respond with full board");
	if (state.isOver() == plr || state.isOver() == state.rPlayer(plr))
		throw logic_error("call respond with ended game");
#endif

	// analyse
	if (starsOn && nonFullList.size() > 4)
		state.areaTopTransform();
	do {
		timeUsed = returnTime(plr, plrList, ++returnMoveDepth, word, trackRoute);
	} while (word == "free" && timeUsed < maxcaltime && returnMoveDepth < 10);
	// this opp list is for the random suggestion functions
	if (returnMoveDepth > 2)
		returnMove(state.rPlayer(plr), oppList, returnMoveDepth - 1);
	else
		returnMove(state.rPlayer(plr), oppList, returnMoveDepth);
	state.areaTopRestore();

	// in case something unpleasent happens:
	if (starsOn && word != "free" && returnMoveDepth > 5 && timeUsed < maxcaltime && nonFullList.size() < 12) {
		returnMoveDepth = 2;
		do {
			timeUsed = returnTime(plr, plrList, ++returnMoveDepth, word, trackRoute);
		} while (word == "free" && timeUsed < maxcaltime && returnMoveDepth < 10);
#ifndef STARS_LANG_CHINESE
		if (showCal)
			cout << "    calculation depth without stars = " << returnMoveDepth - 1 << endl;
	}
	else if (showCal) {
		if (starsOn)
			cout << "    calculation depth with stars = " << returnMoveDepth - 1 << endl;
		else 
			cout << "    calculation depth without stars = " << returnMoveDepth - 1 << endl;
	}

	// show info if needed
	if (showCal) {
		cout << "    word = " << word << ", list = [ ";
		for (short c : plrList)
			cout << c << " ";
		printf("]\n");
	}
	if (showTime)
		cout << "    calculate time used: " << timeUsed << " ms\n";
#else
		if (showCal)
			cout << "    没有星星的计算深度 = " << returnMoveDepth - 1 << endl;
	}
	else if (showCal) {
		if (starsOn)
			cout << "    有星星的计算深度 = " << returnMoveDepth - 1 << endl;
		else 
			cout << "    没有星星的计算深度 = " << returnMoveDepth - 1 << endl;
	}

	// show info if needed
	if (showCal) {
		cout << "    玩家状态 = " << toChinese(word) << ", 推荐列表 = [ ";
		for (short c : plrList)
			cout << c << " ";
		printf("]\n");
	}
	if (showTime)
		cout << "    程序计算用时 " << timeUsed << " 毫秒\n";
#endif // STARS_LANG_CHINESE

	// record, suggest and return
	thisMove.word = word;
	thisMove.list = plrList;
	thisMove.time = timeUsed;
	if (word != "bad")
		return state.randomSuggestion(plr, plrList, oppList);
	else if (word == "bad") {
		word = analyse(plr, plrList);
		if (word == "bad") {
			oppList = firstPoint(rPlayer(plr), nonFullList);
			if (oppList.empty())
				return state.randomMove();
			return state.randomMove(oppList);
		}
		else
			return state.randomSuggestion(plr, plrList, oppList);
	}
#ifdef STARS_DEBUG_INFO
	else
		throw logic_error("wrong word returned in respond");
#endif
	return 0;
}

long long BoardAnalyse::returnTime(const char plr, shortv& list, const short returnMoveDepth, string& word, bool trackRoute) {
	long long				 timeUsed;
	system_clock::time_point start, end;
	start		 = system_clock::now();
	if (trackRoute)
		word = returnMoveDebug(plr, list, returnMoveDepth);
	else
		word = returnMove(plr, list, returnMoveDepth);
	end			 = system_clock::now();
	auto elapsed = duration_cast<milliseconds>(end - start);
	timeUsed	 = elapsed.count();
	return timeUsed;
}
