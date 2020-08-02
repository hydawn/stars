# Old Testing Board
Back in the days when I use python, I generated (by hand) a few games for testing how smart the program was.  
Now you can import these board and play for fun.  

## 2020-7-5-12:56 shallow analysis - solved
### describe
```
 1 2 3 4 5 6 7 8
| | | | | | | | |
| | | | | | | | |
| | | | | | | | |
| | | | | | |0| |
| | |0| | | |X| |
| | |X|X| | |X| |
| | |0|0|X| |0| |
| | |0|X|0|X|X|0|

 1 2 3 4 5 6 7 8
| | | | | | | | |
| | | | | | | | |
| | | | | | | | |
| | | | | | | | |
| | |0| | | | | |
| | |X|X| | |X| |
| | |0|0|X| |0| |
| | |0|X|0|X|X|0|
```
Look at the first board, if X go 4, then 0 must go 5, then X can go 5, then 0 has no where to go. So in the prev board, which is the 2nd board, now X is gonna go 7. And the computer didn't realize the danger. The computer should prevent X from taking 4 by taking 4, if 4 isn't in safe list then computer should take 5.  
About taking 5, that is the intelligence I'm talking about, purely recursive functions won't be able to see that 5 is an option? - I don't think so.
### resolution
so traverse the safeList, all is unavailable except 4 or 5. That's how recursive functions gain intelligence - The program might not know why 5 (or even 4) is  available, but it worked.

## 2020-7-5-21:50 shallow analysis - solved
### describe
board:
```
 1 2 3 4 5 6 7 8
| | | | | | | | |
| | | | | | | | |
| | | | | | | | |
| | | | | | | | |
| | | | | | | | |
| |X|X| | | | | |
| |0|0| | | | | |
| |X|0|0| |X| | |
if X go 3,
 1 2 3 4 5 6 7 8
| | | | | | | | |
| | | | | | | | |
| | | | | | | | |
| | | | | | | | |
| | |X| | | | | |
| |X|X| | | | | |
| |0|0| | | | | |
| |X|0|0| |X| | |
the function return free - it's wrong! the computer's fate is sealed
Let's say 0 go randomly - 3
 1 2 3 4 5 6 7 8
| | | | | | | | |
| | | | | | | | |
| | | | | | | | |
| | |0| | | | | |
| | |X| | | | | |
| |X|X| | | | | |
| |0|0| | | | | |
| |X|0|0| |X| | |
Then X go 2, 
(Graph A):
 1 2 3 4 5 6 7 8
| | | | | | | | |
| | | | | | | | |
| | | | | | | | |
| | |0| | | | | |
| |X|X| | | | | |
| |X|X| | | | | |
| |0|0| | | | | |
| |X|0|0| |X| | |
0 thought that 0 has to go 2, then X go 4:
 1 2 3 4 5 6 7 8
| | | | | | | | |
| | | | | | | | |
| | | | | | | | |
| |0|0| | | | | |
| |X|X| | | | | |
| |X|X| | | | | |
| |0|0|X| | | | |
| |X|0|0| |X| | |
now computer is defeated for X can go 4 after 0 goes 5
```

## 2020-7-6-16:40 shallow analysis - partly solved
```
 1 2 3 4 5 6 7 8
| | | | |0| | | |
| | | | |X|X|0| |
|0| | |X|X|0|X| |
|0| | |0|0|X|0| |
|X| | |X|X|X|0| |
|X| | |0|0|0|X| |
|0| | |X|X|X|0|X|
|0| | |X|0|X|0|0|
```
when I go 4, the computer should go 6, instead, computer goes 1?

## 2020-7-6-16:48 shallow analysis of me - solved?
```
 1 2 3 4 5 6 7 8
| | | | | | | | |
| | | | | | | | |
| | | | | | | | |
| | | | | | | | |
| | | |X| | | | |
| | | |0| |0| | |
| |0| |0| |X| |0|
| |0|X|X|0|X|X|X|
```
here, I(X) must go 3 or 5, but I didn't see that  

## 2020-7-14-22:31 wrong tactic in autoMove and recRetMove
```
 1 2 3 4 5 6 7 8
| | | | | | | | |
| | | | | | | | |
| | |0| | | | | |
| | |X|X|0| | | |
| | |X|X|X| | | |
| | |X|0|X| | | |
|0|X|0|0|0| | | |
|0|0|X|0|X| |X| |
```
if X go 1, computer will not found that there is a 'emergency' in column 8 because there is an emergency in 2 (which will be fixed by going 5) computer will go 5 and stop examine column 345678


## 2020-7-16-23:27 shallow analysis
```
 1 2 3 4 5 6 7 8
| | | |0|X|0| | |
| | |0|X|X|0| | |
| | |X|X|0|X| | |
| | |0|0|0|X| |0|
| | |X|X|X|0| |X|
| | |0|0|X|X| |0|
| | |X|X|0|X| |0|
|0| |X|0|X|X|0|0|
```
here, if X go 1, 0 has to go 1, then recRetMove can see it's a 'bad'

## 2020-7-16-11:51 shallow analysis
```
 1 2 3 4 5 6 7 8
|X| | | | | | | |
|0| | | | | | | |
|0| | | | |0| | |
|0| | |X| |X| | |
|X| |0|0| |X| | |
|X| |X|X| |0| | |
|0| |0|X| |0| | |
|0| |X|0|X|X| | |
```
if X go 3, 0 will go 2 - which is suicidal
