# CharmedCpp
A 1v1 & 8x8 command line based board game
![tag][tag0base64str]

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
Then you're good to go.  

# other modes
Other modes such as play back mode and custom mode just there to explore \~

# known bugs for unknown reasons
If it stopped when you didn't ask it to and you don't know why, calm down and follow these steps below:
1. start up a new game
2. play it like nothing has ever happened
3. hope it won't suddenly stop in the middle of nowhere again

# thanks
[JsonCpp](https://github.com/open-source-parsers/jsoncpp) - from where those [json.h](./lib/json.h), [json-forwards.h](./lib/json-forwards.h) and [jsoncpp.cpp](./lib/jsoncpp.cpp) files in lib/ are apparently copied

My roommate - from whom I learned about this little game which was actually his homework. But the original instruction was to write in python, so no worries about copying and stuff ;-)

[tag0base64str]:data:image/gif;base64,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
