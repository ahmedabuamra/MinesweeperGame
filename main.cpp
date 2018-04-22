#include "functions.h"
int main()
{
	//Declaration of variables
	set < pair<int, int> > minesCoordinates;
	int numOfMines, width, height;
	char but[50][50];
	vector <player> topPlayers; // Vector to hold the top players.
	player mainPlayer = {}; //Struct to hold the data for the player.

							//Linking srand with time to achieve less limited randomness
	srand(time(NULL));

	//Input
	initialiseGame(height, width, numOfMines, topPlayers, mainPlayer);

	//Initialising the tiles in the visible grid with the character '#', and the top players vector.
	initVisibleToHash(height, width);

	//Randomising coordinates for mines
	randomiseMineCoordinates(minesCoordinates, numOfMines, height, width);

	//Placing the mines
	placeMines(minesCoordinates);

	//Setting the numerical values for the tiles on the actual grid, indicating the number of mines adjacent to each tile.
	setValuesForGrid(height, width);

	//Printing out the visible grid initially to the user
	printVisible(height, width);

	//every time you need visible from functions.cpp
	getVisiable(but);
	//Playing the game!
	//playGame(height, width, numOfMines, mainPlayer, topPlayers);
	return 0;
}


