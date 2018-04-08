# Minesweeper Game
CLI Minesweeper game, soon to be updated to a GUI application.
## How to run (CLI Version):
-Make sure you're browsing the "CLI-Game" branch on the repository.

-Download the .ZIP file from the repository.

-Make sure you have g++ installed. If not, install it using your package manager.

-Unzip the file and open a terminal in the directory the files are in. 

-Type the following into the terminal:
```
chmod +x run.sh
./run.sh
```

## How to play (CLI Version):

- Your goal is to find all of the mines without clicking on any of them. Having exactly n flags on n mines, or n tiles unopened with mines under them.

- Each tile has a number on it that indicates the number of mines adjacent to it.

- Try to deduce where the mines are and flag them, or open the tiles that don't contain mines in them.


### Input: 

Before playing, you have to choose a difficulty.

There are 5 options corresponding to the following:

1- 'E' - Easy (8×8 Grid with 10 Mines)

2- 'M' - Medium (10×10 Grid with 20 Mines)

3- 'H' - Hard (15×15 Grid with 45 Mines)

4- 'B' - Brutal (20×20 Grid with 80 Mines)

5- 'C' - Custom (custom grid size with a custom size of Mines)

Input is entered through the CLI, using the following format:

-> row column operation

You have 3 operations, 'O','F','?'

1- 'O' - Opens the tile, and the surrouding empty tiles if it is empty.
    i- If the cell already has a flag or ?, using 'O' on it, removes either of them, and returns it to its undiscovered state.
    
2- 'F' - Places a flag on the tile.

3- '?' - Places a question mark on the tile, this is for the user only, and won't affect mines or the program's calculations.

Example:

|  | A | B | C | D | E | F | G | H |
|--|---|---|---|---|---|---|---|---|
|A | # | # | # | # | # | # | # | # | 
|B | # | # | # | # | # | # | # | # | 
|C | # | # | # | # | # | # | # | # | 
|D | # | # | # | # | # | # | # | # | 
|E | # | # | # | # | # | # | # | # | 
|F | # | # | # | # | # | # | # | # | 
|G | # | # | # | # | # | # | # | # | 
|H | # | # | # | # | # | # | # | # | 

A A O - > Opens the tile A,A.

B A F - > Places a flag on the tile B,A.

B A O - > Removes the flag you just placed.

B A O - > Actually opens the tile B,A.

### Output: 

There are 15 symbols in this game, '#', 'F', '%', '+', '@', '?', 0-8.

'#' - > Indicates a tile that you haven't opened.

'F' - > Indicates a flag in the corresponding tile.

'+' - > Indicates a mine in the corresponding tile.

'?' - > Used by the user to state ambiguity on the cell it is placed on.

'%' - > Mine that was flagged but you lost the game.

'@' - > Exploded mine

Numbers from 0 to 8 - > Indicate the number of mines in the 8 adjacent tiles of the tile they're on.

## GUI Version with a special theme coming soon!!
