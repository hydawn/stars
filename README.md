# Stars |+| &#x1f320; &#x2b50; &#x1f30f;
A 1v1 & 8x8 command line based board game

![tag](./icon/tag.png)

# Introduction
Type in number 1~8 to play, q to exit. Whoever to be the first to put 4 pieces in a row wins.

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

Note that whenever you try to place a piece in a column, it always start piling from the button. You can't put a piece in the middle like any other proper board game would, which might bring boredom but certainly makes it easier for me.

# Play &#x1F3AE;
## Windows
### Option 1: Download directly
Download in [here](https://github.com/HanyuDuan/stars/releases), click to run.  

### Option 2: Build &#x1F528; with cmake  
In cmd build an English version:

```cmd
git clone https://github.com/HanyuDuan/stars.git
script\build.cmd
```


## Linux
### Build &#x1F528; with cmake
```sh
git clone https://github.com/HanyuDuan/stars.git
./script/build.sh
```
Use `./stars` to start the game.  
`./build.sh install` to install, `./build.sh uninstall` to uninstall &#x1f5d1;  

## macOS
Same as Linux.

# Import
You can see there's a few board in [oldTestingBoard.md](./oldTestingBoard.md). If you want to import and play in a new board, just enter `import` or `I` and you'll see this:  
```
In debug mode
Player 'X' move> I
Paste a board down below>
 1 2 3 4 5 6 7 8
```
Then paste it in the terminal like this:  
```
In debug mode
Player 'X' move> I
Paste a board down below>
 1 2 3 4 5 6 7 8
| |0| |X| |+|+|+|
| |X| |0| | |+|+|
| |0| |X|X| | |+|
| |0| |X|0|X| |+|
| |X| |X|X|X| |+|
| |X| |0|0|0| | |
| |0| |X|0|X|0| |
| |0|X|0|X|0|0| |

In debug mode
Player 'X' move>
```
Then you're good to go &#128076;  

# Other modes
Other modes such as play back and custom just there to explore \~

# Known bugs for unknown reasons
## A sudden stop
If it stopped when you didn't ask it to or throw some errors and if you don't know why, calm down and follow these three steps:  
1. start up a new game
2. play it like nothing ever happened
3. hope it won't suddenly stop in the middle of nowhere again  

Or you can tell me how it happened so that I might fix it one day &#x1F609;  

## Settings went nuts
Sometimes your personal settings get wiped out and reset to factory settings &#129300;  

# Thanks
[JsonCpp](https://github.com/open-source-parsers/jsoncpp) - from where those dependencies files are copied  

My roommate - from whom I learned about this little game which was actually his homework. But the original instruction was to write in python, so ... ;-)
