well, this is clearly more organized than CharmedPython

# cmt code
## 001
- major bug in BoardAnalyse::respond fixed line 572
- fix play back mode can't handle input board
- rm feature added to play mode and play back mode
- print message reorganized
- quit and help in play mode and play back mode added
- BoardRecord::showSavedGames reorganized
- return type changed
- note deleted

## 002
- normal erased
- dev updated
- star even improved
- shortList.cpp added
- other minor fix

## 003
- route tree added to track the computer's analyse route
- ways to reset the max calculation time added

## 004
- took single route analyse out in a new function - performance improved
- fix problem: maxcaltime won't be write to json
- fix generate function in boardInterface.h
- which is never used until now
- many copy constructors added
- some renaming
- other minor changes

## 005
- using string for input handle
- get board input function secured, get import interface improved
- using string for input is good, but I don't want to change the old code - that's a waste of time
- fix: returnModeDebug route related wrongly coded - thus performance improved
- intercept strategy fixed
- winPieceButOne added, intercept strategy improved
- changed some runtime_error to logic error

# TODO
- add debug and release flag
- change getStateFromInput to fit all size of board - there's no need to do that
- add GUI - I don't think so
- record calculate depth - but why?
- Then I will stop

- actually, seen that starArea + while returnMove is doing quite well, there's no need to use recursive function anymore - I was actually proud of that, few thing here that needed brain work. Now that it's useless, I suddenly lost interest in all of this stuff. No then I will stop, I'll stop right now.

wait, I got a new idea  
the last time I got a new idea (starArea) was yesterday when taking a walk. Just a few minutes ago, when I was taking a shower, thinking about giving up the project and start writing my essay, it occurred to me that I can record every "route" that in the end produce something (like good or bad), and record that route, after hundreds of times of "letting-computer-play-with-itself", I should gathered enough data to produce a conclusion about how to decide which area should be the star area.  
That sounds ... interesting?   
I can use a tree, never used a tree (explicitly?) before.   

# pre-release note - version 1.0 - Beta
preRelease - total failure, no pause in bash, clock() is not working as expected, core dumped, returnSituation is never used, depth > 3 is never used.

# areaTop (now starArea)
I was thinking about cut branches lately, originally, I thought a little version of the current board would be useful ('cause I've been developing that kind board shape lately), like this:   
if I want to analyse this:
```
| | | | | | | | |
| | | | | | | | |
| | | | | | | | |
| | | | | | | | |
| | | | | | | | |
| | | | | | |X| |
| | | | | | |X|0|
| | | | | |X|0|0|
```
then I'll create a new little board like this:
```
| | | | | |
| | | | | |
| | | |X| |
| | | |X|0|
| | |X|0|0|
```
then I thought the shape isn't good, so, maybe this will do:
```
      | |  
    | | | |
    | |X| |
| | | |X|0|
| | |X|0|0|
```
but how should I create that? - maybe I should create a new class, and inherit it from class BoardState (formal BoardHandle), seems that inheritance is useful after all. So I got this:   
```cpp
class BoardArea : public BoardState {
public:
    short* areaTop;
    BoardArea() { generate(); }
    BoardArea(const BoardState& bs);
    virtual ~BoardArea() { free(); }

    virtual void show();
    virtual bool colIsFull(const short col) { return areaTop[col - 1] == row; }
    virtual void nonFullColumn(shortv& nonFull);
    void refreshAreaTop();
    virtual BoardArea& operator=(const BoardArea& ba);
};
```
Then I found that this `BoardArea(const BoardState& bs)` function is quite annoying, it will call `BoardState()` first, but that will create a `(8, 8, 4)` type of board. I don't want that, so I'll have to free first before generate (afterward is when I realize that I should use `BoardArea::BoardArea(const BoardState& bs) : BoardState(bs)` instead, but I forgot). Even if I got it right, how am I gonna nest that into other board class? Change BoardState to BoardArea? then why should I have BoardState in the first place?   
Anyway, it seems concise to just add a new item areaTop and change nonFullColumn instead of creating a new class. And that's what I choose.  

## critics
the first version was to add two above, add one in the side, and ad 1/2 on the side of the side:  
```cpp
void BoardState::setATopWithTop(short i, short t) {
    if (t == 0)
        return;
    setATopWithNumber(i - 2, t / 2 - 1);
    setATopWithNumber(i - 1, t + 1);
    setATopWithNumber(i, t + 2);
    setATopWithNumber(i + 1, t + 1);
    setATopWithNumber(i + 2, t / 2 - 1);
}
```
Doesn't end well in situation like this:  
```
 1 2 3 4 5 6 7 8
|+|+|+|+|+|+|+|+|
|+|+|+|+|+|+|+|+|
|+|+|+|+|+|+|+|+|
|+| | |+|+|+|+|+|
| | | | |+|+|+|+|
| |0|0| | |+|+|+|
|0|X|X|X| |+|+|+|
|X|0|X|X| |+|+|+|
```
now the computer think it's safe - what if I go 5 or 6 ?


# analyse testing board
## 2020-7-25-23:11
```
| | | |0|X|0| | |
| | |0|X|X|0| | |
| | |X|X|0|X| | |
| | |0|0|0|X| |0|
| | |X|X|X|0| |X|
| | |0|0|X|X| |0|
| | |X|X|0|X| |0|
|0| |X|0|X|X|0|0|
```
if I let loose, I'm get all the correct answer: X=good=1,2; 0=free=1 (4969ms/100times)  
but if I make it mo more than list4, I still got X=good=1 (993ms/100times)  
and the performance is 5 times better, what should I do then?  

## 2020-8-3 ???
```
 1 2 3 4 5 6 7 8
|0| |X|0|0| |+| |
|0| |X|X|0| | | |
|X| |X|0|0| | |0|
|X| |0|0|X| | |0|
|X| |X|0|X| | |X|
|0| |X|X|0| | |X|
|0| |0|X|0| |X|0|
|X| |X|0|X| |0|X|
```
if X goes 2:
```
word = bad
list = [ ]
Computer time used: 1 ms

0 goes here 7
Player time used: 11541ms

Here is hint provided for you
ReturnMoveDepth = 10

word = good
list = [ 7 ]
Computer time used: 0 ms
This is going really well~
this 7 is recommended
```
then I go 7 as instructed, and then
```
word = good
list = [ 7 ]
Computer time used: 0 ms
This is going really well~
this 7 is recommended
 1 2 3 4 5 6 7 8
|0| |X|0|0| |+| |
|0| |X|X|0| | | |
|X| |X|0|0| | |0|
|X| |0|0|X| | |0|
|X| |X|0|X| | |X|
|0| |X|X|0| |0|X|
|0| |0|X|0| |X|0|
|X|X|X|0|X| |0|X|


In debug mode
Player 'X' move> 7
ReturnMoveDepth = 10

word = bad
list = [ ]
Computer time used: 1 ms

0 goes here 8
Player time used: 12380ms

Here is hint provided for you
ReturnMoveDepth = 10

word = free
list = [ 8 ]
Computer time used: 0 ms
One move left, where can you go then?
this 8 is recommended
 1 2 3 4 5 6 7 8
|0| |X|0|0| | | |
|0| |X|X|0| | |0|
|X| |X|0|0| | |0|
|X| |0|0|X| | |0|
|X|%|X|0|X| |X|X|
|0| |X|X|0| |0|X|
|0| |0|X|0| |X|0|
|X|X|X|0|X| |0|X|
```
what???
I think it's the stars, if there are stars, then X will hit % point first, but if there aren't any stars, then 0 will hit % first. This explains why I suddenly goes from must win to must lose.

Then I make the stars lost their powers when `nonFullList.size() < 5`, but `returnMove()` still cannot see that in this board, when X goes 7 in the first place, X is about to fail. Maybe it's time to use that recursive function. However, I forgot which one should I call, returnSituation or recursiveSituation, I even forgot why there's two situation in the first place??.

# 2020-8-16-17:41 more depth less time
```
| | | | | | | | |
| | | | | | | | |
| | | | | | | | |
| |0|0| | | | | |
| |X|X| | | | | |
| |X|X|0| | | | |
| |0|0|X|X| | | |
| |0|X|0|0| | |X|
```
if X go 4:   
depth = 5, time used = 1451ms, word = "free"   
depth = 6, time used = 42ms, word = "bad"   
so, how should I use this?   

# performance testing board
## nearly empty board
  BlankBoard1
```
| | | | | | | | |
| | | | | | | | |
| | | | | | | | |
| | | | | | | | |
| | | | | | | | |
| | | | | | | | |
| | | | | | | | |
|0| | | | | |0| |
```
guess how long it takes?  
if I go as far as to let it create list5, which means loop list4, it takes 1265ms to give me one answer! 1265ms/ 1 try!  
if depth = 2, 21ms / 1 try  
if depth = 3, 169ms / 1 try  
if depth = 4, 1270ms / 1 try  
if depth = 5, 8621ms / 1 try  
if depth = 6, 66290ms / 1 try  
if depth = 7, 452385ms / 1 try

currently, the max number is 7, the default depth is 3

what if 1/4 of the board if full?  
QuatreFullBoard1  
```
| | | | |0| | |X|
| | | | |X| | |X|
| | | | |X| | |0|
| | | | |0| | |0|
| | | | |X| | |0|
| | | | |X| | |X|
| | | | |0| | |0|
| | | | |0| | |X|
```
if depth = 2, 11ms / 1 try  
if depth = 3, 48ms / 1 try  
if depth = 4, 265ms / 1 try  
if depth = 5, 1099ms / 1 try  
if depth = 6, 5838ms / 1 try  
if depth = 7, 23865ms / 1 try


now, what if half the column is full?  
HalfFullBoard1
```
| | | | |X|0|X|X|
| | | | |0|X|0|X|
| | | | |0|X|X|0|
| | | | |X|0|0|0|
| | | | |0|X|0|0|
| | | | |0|X|0|X|
| | | | |X|0|X|0|
| | | | |X|0|X|X|
```

if depth = 2, 2.3ms / 1 try  
if depth = 3, 8ms / 1 try  
if depth = 4, 33ms / 1 try  
if depth = 5, 117ms / 1 try  
if depth = 6, 428ms / 1 try  
if depth = 7, 1429ms / 1 try  

NearlyFullBoard1
```
| |0| | |X|0|X|X|
| |X| | |0|X|0|X|
| |X| | |0|X|X|0|
| |0| | |X|0|0|0|
| |X| | |0|X|0|0|
| |X| | |0|X|0|X|
| |0| | |X|0|X|0|
| |0| | |X|0|X|X|
```
if depth = 7, 67ms / 1 try, perfectly affordable.

I tried to make everything inline, but it won't make too much a difference

## depth required
```
| |X| |0|X| | | |
| |X| |X|0| |0| |
| |0| |0|0| |X| |
| |0| |X|0| |X| |
| |X| |0|X| |X| |
| |X| |X|X| |0| |
| |X| |0|X| |X|0|
|0|0| |X|0|0|0|X|
```
if 0 didn't go depth=8, it won't find out that this is bad already

## interesting board1
```
 1 2 3 4 5 6 7 8
| | | | | | | | |
| | | | | | | | |
| | | | | | | | |
| | | | | | | | |
| | | | | | | | |
| |X| | | |X| | |
| |0|X|0| |0| | |
|0|X|X|0|X|0| | |
now if x go 3 and 4, you'll get
 1 2 3 4 5 6 7 8
| | | | | | | | |
| | | | | | | | |
| | | | | | | | |
| | | | | | | | |
| | |0|0| | | | |
| |X|X|X| |X| | |
| |0|X|0| |0| | |
|0|X|X|0|X|0| | |
however, 0 has a free list of 3,4,7 - if 0 goes 7 in advance, then x won't dare go 5! 7 is most interesting, I didn't see that, and if I haven't taken the strategy I have taken now, I won't have code this by my self!
```

## performance & depth test board
```
| | | |0|X|0| | |
| | |0|X|X|0| | |
| | |X|X|0|X| | |
| | |0|0|0|X| |0|
| | |X|X|X|0| |X|
| | |0|0|X|X| |0|
| | |X|X|0|X| |0|
|0| |X|0|X|X|0|0|
```
here  
returnMove(plr=X, list, depth=8) takes 76ms and return `good,[1,2]`  
returnSituation(plr=x, list, 3,0,3) (default) takes 82ms and return `good, [1,2,3,8]`  
returnSituation is better indeed.  
HOWEVER  
if I let depth=9, then returnMove takes only 27ms to return `good, [1,2,3,8]`   
??? in this case, the higher the depth, the faster the program?


## analyse function
```
| | |X|0|X|0| | |  
| | |0|X|X|0| | |
| | |X|X|0|X| | |
|0| |0|0|0|X| |0|
|0| |X|X|X|0| |X|
|X| |0|0|X|X| |0|
|0|X|X|X|0|X| |0|
|0|X|X|0|X|X|0|0| 
```
now, it's `*col==3 *col1==1 *col2==1 *col2==1 *col4==2 *col5==1 *col6==2`  
(X goes first) and we are in the list6 loop, x just go that  
2 move, and analyse() return a free instead of bad, if it can return a bad, then  
it can release returnMove a lot of pressure?

# performance improved - just a little
However, it's more dangerous now  
this nearly empty board (depth == 4)
```
| | | | | | | | |
| | | | | | | | |
| | | | | | | | |
| | | | | | | | |
| | | | | | | | |
| | | | | | | | |
| | | | | | | | |
|0| | | | | |0| |
```
takes 358ms or 795ms sometimes for `ShortList`, and takes 998ms for the normal `vector<short>` a little bit faster

```
| | | |0|X|0| | |
| | |0|X|X|0| | |
| | |X|X|0|X| | |
| | |0|0|0|X| |0|
| | |X|X|X|0| |X|
| | |0|0|X|X| |0|
| | |X|X|0|X| |0|
|0| |X|0|X|X|0|0|
```
this however, `ShortList` takes 21ms, `vector<short>` takes 25ms




# worth noticing

## C string compare (char[])
c style string have no overload at =, so I have to use !strcmp() which return 0 if two strings are the same

## C input
if I use `cin >> word;` before using `cin.getline(input, 256);` I will always get a \0. So I have to add a `char disposal = cin.get();` to get rid of the `\0` that nobody want.  
by the way, cin.clear() won't help

## class
`generate()` and `free()` is quite useful when trying to refresh (add other functions to) a class containing new and delete

# class BoardHandle
in here, everything starts from 0,0  
handling some simple function  
## var
### board
`board[column][row]` = `board[x][y]`
used to represent the board, computer is '0', the player is 'X'  
starts with (0,0)
```
|[0][1]|[1][1]|
|[0][0]|[1][0]|
```

### top
```
top[column]=topRow
for example:
 1 2 3 4 5 6 7 8
| | | | | | | | |
| | | | | | | | |
| | | | |0| | | |
| | | |0|X| | | |
|0| |X|X|X|X| | |
top={1,0,1,2,3,1,0,0}
```
