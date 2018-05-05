
Minesweeper Game
===


How to run:
---------
Make sure you have SFML & TGUI libraries installed, you can install them from here:  
	- [SFML](https://www.sfml-dev.org/download.php)  
	- [TGUI](https://tgui.eu/download/)



Objective:
-----------
Your objective is to clear the given grid containing mines without blowing any of them.

How to play:
--------------
- Before starting the game, you can choose from 5 different modes:

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

- When all the empty tiles are revealed and all the mines are flagged, you then shall win!

- The faster you finish a game the higher your score will be.

Credits:
--------
Music: [Pertubator](https://perturbator.bandcamp.com/).










