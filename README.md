
![alt text](https://github.com/ahmedabuamra/MinesweeperGame/blob/master/Screenshots/Name.png)
===

## Description
Minesweeper is a single-player video game originated from the 1960s, its objective is to clear a given grid containing a given number of mines without blowing any of them.


## How to run:

Make sure you have SFML & TGUI libraries installed, you can install them from here:  
	- [SFML](https://www.sfml-dev.org/download.php)  
	- [TGUI](https://tgui.eu/download/)

## Theme:
The game uses a retrowave theme to deliver classic nostalgic vibes to the players.
![alt text](https://github.com/ahmedabuamra/MinesweeperGame/blob/master/Screenshots/Pallete.png)

## Objective:
Your objective is to clear the given grid containing mines without blowing any of them.

## How to play:
### Before starting the game, you can choose from 5 different modes:

  - **Easy:** 8x8 grid with 10 mines.  
  - **Medium:** 10x10 grid with 20 mines.  
  - **Hard:** 15x15 grid with 45 mines.  
  - **Brutal:** 20x20 grid with 80 mines.  
  - **Custom:** Custom grid size and amount of mines.  
  
- Left-click on a tile to reveal it,
  - If a mine is revealed, you lose the game.
  - If a number (1-8) is revealed, take this number as an indicator of many mines this tile is adjacent to.
  - If an empty tile is revealed, all empty tiles will be revealed recursively.

- Right-click on a tile to place a flag/question mark,  
  - Place a flag on tile when you're sure it contains a mine underneath to avoid mistakenly revealing it.
  - Place a question mark when it's logically impossible to determine the content of the tile, so you can come back to it later.

- When all the empty tiles are revealed and/or all the mines are flagged, you win!
- The faster you finish a game, the higher “score” you will have, the higher your ranking will be.

## Main Program Flow
![alt text](https://github.com/ahmedabuamra/MinesweeperGame/blob/master/Screenshots/Flowchart.png)

## Screenshots

![alt text](https://github.com/ahmedabuamra/MinesweeperGame/blob/master/Screenshots/Main%20Menu.png)


![alt text](https://github.com/ahmedabuamra/MinesweeperGame/blob/master/Screenshots/GameDemo.png)
