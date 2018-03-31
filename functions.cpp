#include "functions.h"


//Boolean flag to tell if a mine has been clicked on
bool clickedOnMine(false);

//Arrays are made global to prevent stack overflow, and make them easier to use with functions.
int grid[50][50];
char visible[50][50];



void initVisibleToHash(int height, int width)
{
	//A function that initialises that visible grid to the character '#'
	for (int i = 1; i <= height; i++)
		for (int j = 1; j <= width; j++)
			visible[i][j] = '#';
}

void chooseDifficulty(char c, int &height, int &width, int &numOfMines)
{
	//A function that initialises the grid size and number of mines based on the difficulty
	//chosen by the player.
	switch (c)
	{
		case 'E':height = 8, width = 8, numOfMines = 10; break; //Easy case

		case 'M':height = 10, width = 10, numOfMines = 20; break; //Medium case

		case 'H':height = 15, width = 15, numOfMines = 45; break; //Hard case

		case 'B':height = 20, width = 20, numOfMines = 80; break; //Brutal case

		case 'C':
		{
			cout << "Enter grid size (height * width), and # of mines: ";
			cin >> height >> width >> numOfMines;
			break;
		}  //Custom case
		default: cout << "Error!" << endl;
	}


}

void initialiseGame(int &height, int &width, int &numOfMines)
{
	//A function that passes the height, width and the number of mines in the grid
	//by reference to input them

	cout << setw(15) << "Welcome to Minesweeper!" << endl;
	cout << setw(0) << "Enter the difficulty you want to play (E, M, H, B, C): ";

	char diff;
	cin >> diff;

	chooseDifficulty(diff, height, width, numOfMines);

}

void randomiseMineCoordinates(set < pair<int, int> > &minesCoordinates,
										int numOfMines, int height, int width)
{
	//A function that randomises the coordinates for the mines.
	while (mineSetSize < numOfMines)
	{
		int y = rand() % height + 1;
		int x = rand() % width + 1;
		minesCoordinates.insert(std::make_pair(y, x));
	}
}

void setValueForTile(int r, int c)
{
	//A function that sets the values for the tile based on the mines in the adjacent tiles around it
	int cnt = 0;
	for (int row = r - 1; row <= r + 1; row++)
	{
		for (int col = c - 1; col <= c + 1; col++)
		{
			if (grid[row][col] == Mines_Flag)
				cnt++;
		}
	}
	grid[r][c] = cnt;
}

void setValuesForGrid(int height, int width)
{
	//A function that sets the values for the tiles on the whole grid
	for (int row = 1; row <= height; row++)
	{
		for (int col = 1; col <= width; col++)
		{
			if (grid[row][col] != Mines_Flag)
				setValueForTile(row, col);
		}
	}
}

void placeMines(set < pair<int, int> > &minesCoordinates)
{
	//A function that places the mines
	for (set<pair<int, int> >::iterator it = minesCoordinates.begin(); it != minesCoordinates.end(); it++)
	grid[it->first][it->second] = Mines_Flag;
}

void showMinesIfLost(int height, int width)
{
	//A function that makes the mines visible to the player if they lose
	for (int i = 1; i <= height; i++)
		for (int j = 1; j <= width; j++)
			if (grid[i][j] == Mines_Flag && visible[i][j] == 'F')
				visible[i][j] = '%';
			else if (grid[i][j] == Mines_Flag)
				visible[i][j] = '+';
}

void printVisible(int height, int width)
{
	//A function that prints the visible grid to the user

	//Printing out the top guide
	cout << "    ";
	for (int i = 1; i <= width; i++)
		cout << (char) (i + 64) << ' ';
	cout << endl << "    ";

	//Printing a separator between the top guide and the grid.
	for (int i = 1; i <= width; i++)
		cout << "- ";
	cout << endl;

	for (int i = 1; i <= height; i++)
	{
		cout << (char) (i + 64) << " | "; // Printing out a character and a pipe from the side guide
		//before printing the row itself
		for (int j = 1; j <= width; j++)
		{
			cout << visible[i][j] << ' ';
		}
		cout << endl;
	}
}

void openTiles(int y, int x, int height, int width)
{
	//A function that checks the open tiles around the tiles that was clicked on
	//using DFS
	if(y > height || y == 0 || x > width || x == 0 || visible[y][x] != '#')
		return;

	if(grid[y][x] != 0)
	{
		visible[y][x] = (char)(grid[y][x] + '0');
		return;
	}

	visible[y][x] = (char)(grid[y][x] + '0');

	openTiles(y+1,x, height, width); //North
	openTiles(y-1,x, height, width); //South
	openTiles(y,x+1, height, width); //East
	openTiles(y,x-1, height, width); //West

	openTiles(y-1,x-1, height, width); //South-West
	openTiles(y+1,x+1, height, width);	//North-East
	openTiles(y-1,x+1, height, width);	//South-East
	openTiles(y+1,x-1, height, width);	//North-West
}

void clickTile(int height, int width)
{
	// A function that imitates the action of clicking on a tile.

	char yC, xC;
	char operation;

	cin >> yC >> xC;
	cin >> operation;

	int y = yC - 64, x = xC - 64;

	if (operation == 'O')
	{
		if (grid[y][x] == Mines_Flag)
		{
			showMinesIfLost(height, width);
			visible[y][x] = '@'; // The '@' sign indicates an exploded mine
			clickedOnMine = true;
		}

		else if (visible[y][x] == 'F')
			visible[y][x] = '#';

		else if (visible[y][x] == '?')
			visible[y][x] = '#';

		else
			openTiles(y, x, height, width);

		printVisible(height, width);
	}

	else if (operation == 'F')
	{
		if (visible[y][x] == '#')
			visible[y][x] = 'F';

		printVisible(height, width);
	}
	else if (operation == '?')
	{
		if (visible[y][x] == '#')
			visible[y][x] = '?';

		printVisible(height, width);
	}
}

char endGame(int height, int width, int numOfMines)
{
	// A function that checks if the game has ended or not by checking the visible grid.

	int cntr(0), flagCntr(0);
	bool emptyAreVisible(true);

	for (int i = 1; i <= height; i++)
		for (int j = 1; j <= width; j++)
		{
			if (visible[i][j] == 'F' && grid[i][j] == Mines_Flag)
				cntr++;
			if (visible[i][j] == 'F')
				flagCntr++;
		}

	for (int i = 1; i <= height; i++)
		for (int j = 1; j <= width; j++)
		{
			if (grid[i][j] != Mines_Flag && visible[i][j] == '#')
				emptyAreVisible = false;
		}


	cout << "Number of flags placed:" << flagCntr << endl;
	if (cntr == numOfMines && flagCntr == numOfMines || emptyAreVisible)
	{
		cout << "Congratulations, you win!" << endl;
		return 'W'; // Returns W (for win) if the user puts flags on all of the mines
	}


	if (clickedOnMine)
	{
		cout << "You clicked on a mine, you lose!" << endl;
		return 'L'; // Returns L (for lose) if the user clicks on a mine
	}

	return 'N'; // Returns N (for nil) by default

}

void playGame(int height, int width, int numOfMines)
{
	auto start = steady_clock::now(); //Sets a start for counting time.

	cout << "Enter tile address and then the command(?,F,O): ";

	//Playing the game!
	do
	{
		clickTile(height, width);
	}
	while (endGame(height, width, numOfMines) == 'N');

	auto end = steady_clock::now(); //Sets an end for counting time.

	auto tSeconds = duration_cast <seconds> (end - start).count(); //Counting seconds
	auto tMinutes = duration_cast <minutes> (end - start).count(); //Counting minutes

	if (tMinutes > 0) //Condition that makes sure to prrint the minutes only if they are larger than 0.
	cout << "The game took " << tMinutes << " minutes, and "
		  << tSeconds - (tMinutes*60) << " seconds." << endl;
	else
		cout << "The game took " << tSeconds << " seconds." << endl;
}
