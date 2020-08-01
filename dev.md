well, this is clearly more organized than CharmedPython
[toc]

# TODO
- a few read functions to read games from json, and a playMode to play back those
  saved games and cut in when want to.

# class board
in here, everything starts from 1,1 !!
## var
### board
## function
### go

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
if I go as far as to let it create list5, which means loop list4, it takes
1265ms to give me one answer! 1265ms/ 1 try!
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
however, 0 has a free list of 3,4,7 - if 0 goes 7 in advance, then x won't dare go 5!
7 is most interesting, I didn't see that, and if I haven't taken the strategy I
have taken now, I won't have code this by my self!
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
takes 358ms or 795ms sometimes for `ShortList`, and takes 998ms for the normal
`vector<short>` a little bit faster

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
c style string have no overload at =, so I have to use !strcmp() which return
0 if two strings are the same
## C input
if I use `cin >> word;` before using `cin.getline(input, 256);` I will always get
a \0. So I have to add a `char disposal = cin.get();` to get rid of the `\0`
that nobody want.
by the way, cin.clear() won't help

# class BoardHandle
in here, everything starts from 0,0 !!
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

## function
handleBoard's initialization
### isOver
return true is the game is over - board is full or winn appeared
### add
add to a col without checking if it's full
### remove
remove a col's element without check

# class BoardAnalyse

# class BoardInterface