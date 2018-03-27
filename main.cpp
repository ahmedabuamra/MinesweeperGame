#include "functions.h"

int main()
{
	//Declaration of variables
	set < pair<int, int> > minesCoordinates;
	int numOfMines, width, height;


	//Linking srand with time to achieve less limited randomness
	srand(time(NULL));

	//Input
	getInitialValues(height, width, numOfMines);

	//Initialising the tiles in the visible grid with the character '#'
	initVisibleToHash(height, width);

	//Randomising coordinates for mines
	randomiseMineCoordinates(minesCoordinates, numOfMines, height, width);

	//Placing the mines
	placeMines(minesCoordinates);


	//Setting the numerical values for the tiles on the actual grid, indicating the number of mines adjacent to each tile.
	setValuesForGrid(height, width);

	//Printing out the visible grid initially to the user
	printVisible(height, width);

	//Playing the game!
	playGame(height, width, numOfMines);

    return 0;
}
