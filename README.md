# Stars |+|
A 1v1 & 8x8 command line based board game

![tag](./icon/tag.png)

# introduction
Type in number 1~8 to play, 0 to exit. Whoever to be the first to put 4 pieces in a row wins.

For example:
```
 1 2 3 4 5 6 7 8
| | | | | | | | |
| | | | | | | | |
| | | | | | | | |
| | | | | |0| | |
| | |X| |0|0| | |
| | |0|X|X|X| | |
|0| |X|0|X|X| | |
|0| |0|X|X|X|0| |
```
Now X wins.

Note that whenever you try to place a piece in a column, it always start piling from the button. You can't put a piece in the middle like any other proper board game would, which might bring boredom but certainly ease things up for me.


# import
You can see there's a lot of board in [oldTestingBoard.md](./oldTestingBoard.md). In case you want to import and play a new board, just paste it in the terminal like this:  
```
In debug mode
Player 'X' move> import
import a new board>
| |X|0| |X|0| | |
|0|0|X| |X|0| | |
|X|X|0| |0|X| | |
|0|0|0| |X|X| | |
|0|X|0| |X|X|X| |
|X|0|X| |X|0|0| |
|X|0|X| |0|0|0|X|
|X|0|X| |0|X|X|0|
Here's the imported board we are gonna use:
 1 2 3 4 5 6 7 8
| |X|0| |X|0| | |
|0|0|X| |X|0| | |
|X|X|0| |0|X| | |
|0|0|0| |X|X| | |
|0|X|0| |X|X|X| |
|X|0|X| |X|0|0| |
|X|0|X| |0|0|0|X|
|X|0|X| |0|X|X|0|
hit 'Enter' to continue>

In debug mode
Player 'X' move>
```
Then you're good to go &#128076;  

# other modes
Other modes such as play back mode and custom mode just there to explore \~

# known bugs for unknown memory reasons
If it stopped when you didn't ask it to and you don't know why, calm down and follow these steps below:
1. start up a new game
2. play it like nothing has ever happened
3. hope it won't suddenly stop in the middle of nowhere again &#x1F602;   

or you can tell me how it happened so that I might fix it one day &#129300; 

# thanks
[JsonCpp](https://github.com/open-source-parsers/jsoncpp) - from where those json.h, json-forwards.h and jsoncpp.cpp files are originally copied bt now deleted

My roommate - from whom I learned about this little game which was actually his homework. But the original instruction was to write in python, so no worries about copying and stuff ;-)
