#include "functions.h"

bool clickedOnMine(false);//Boolean flag to tell if a mine has been clicked on.
bool won(false);//Boolean flag to tell if the player has won.

//Arrays are made global to prevent stack overflow, and make them easier to use with functions.
int grid[50][50];
char visible[50][50];

void charline(int x, char c, char spaces)
{
	//A function that prints a string of characters x number of times.
	if (spaces == 'Y')
		for (int i = 0; i < x; i++)
			cout << c << ' ';

	else if (spaces == 'N')
		for (int i = 0; i < x; i++)
			cout << c;

	cout << endl;
}

//Scoreboard functions
void initialiseVector(vector <player> &topPlayers)
{
	//A function that initialises the topPlayers vector with generic data to avoid runtime errors.
	for (int i = 0; i < 10; i++)
		topPlayers.push_back(player());
}

void readScoreboard(vector <player> &topPlayers)
{
	//A function that reads the scoreboard from the scoreboard.scrb file.
	ifstream scoreboardI;
	string name;
	long long milliSeconds;

	scoreboardI.open("scoreboard.scrb");
	for (int i = 0; i < 10; i++)
	{
		getline(scoreboardI, name, '*');

		scoreboardI >> milliSeconds;

		scoreboardI.ignore();

		topPlayers[i].name = name;
		topPlayers[i].milliSeconds = milliSeconds;

		topPlayers[i].seconds = milliSeconds / 1000;
		topPlayers[i].seconds %= 60;
	}
	scoreboardI.close();
}

void writeScoreboard(vector <player> &topPlayers)
{
	//A function that writes the scoreboard to the scoreboard.scrb file.
	sortScoreboard(topPlayers);
	int width = 30; //For formatting only
	ofstream scoreboardO;
	scoreboardO.open("scoreboard.scrb");
	for (int i = 0; i < 10; i++)
	{
		scoreboardO << topPlayers[i].name << "*" <<
						setw(width - (int)topPlayers[i].name.size()) <<
						topPlayers[i].milliSeconds << endl;
	}
	scoreboardO.close();
}

bool compareMilliseconds(player a, player b)
{
	return (bool)(a.milliSeconds < b.milliSeconds);
}

void sortScoreboard(vector <player> &topPlayers)
{
	sort(topPlayers.begin(), topPlayers.end(), compareMilliseconds);
}

void readyScoreboard(vector <player> &topPlayers)
{
	//A function that initialises the vector, reads the scoreboard
	initialiseVector(topPlayers);
	readScoreboard(topPlayers);
	writeScoreboard(topPlayers);
}


//Game functions
void initVisibleToHash(int height, int width)
{
	won = false;
	//A function that initialises that visible grid to the character '#'
	for (int i = 1; i <= height; i++)
		for (int j = 1; j <= width; j++)
			visible[i][j] = '#';
}

void randomiseMineCoordinates(set < pair<int, int> > &minesCoordinates,
										int numOfMines, int height, int width)
{
	//A function that randomises the coordinates for the mines.

	//clear every time, to solve bug number
	minesCoordinates.clear();
	srand(time(NULL));
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
	memset(grid, 0, sizeof(grid));
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
		cout << (char)(i + 64) << ' ';
	cout << endl << "    ";

	//Printing a separator between the top guide and the grid.
	charline(width, '-', 'Y');

	for (int i = 1; i <= height; i++)
	{
		cout << (char)(i + 64) << " | "; // Printing out a character and a pipe from the side guide
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
	if (y > height || y == 0 || x > width || x == 0 || visible[y][x] != '#')
		return;

	if (grid[y][x] != 0)
	{
		visible[y][x] = (char)(grid[y][x] + '0');
		return;
	}

	visible[y][x] = (char)(grid[y][x] + '0');

	openTiles(y + 1, x, height, width); //North
	openTiles(y - 1, x, height, width); //South
	openTiles(y, x + 1, height, width); //East
	openTiles(y, x - 1, height, width); //West

	openTiles(y - 1, x - 1, height, width); //South-West
	openTiles(y + 1, x + 1, height, width);	//North-East
	openTiles(y - 1, x + 1, height, width);	//South-East
	openTiles(y + 1, x - 1, height, width);	//North-West
}

void clickTile(int height, int width, int y, int x, char operation)
{
	// A function that imitates the action of clicking on a tile.

	y++, x++; // the program is one based while the buttons are zero based
	if (operation == 'O')
	{
		if (grid[y][x] == Mines_Flag && visible[y][x] != 'F')
		{
			showMinesIfLost(height, width);
			visible[y][x] = '@'; // The '@' sign indicates an exploded mine
			clickedOnMine = true;
		}

			/*else if (visible[y][x] == 'F')
			visible[y][x] = '#';
			else if (visible[y][x] == '?')
			visible[y][x] = '#';*/

		else
			openTiles(y, x, height, width);

		printVisible(height, width);
	}

	else if (operation == 'F')
	{
		if (visible[y][x] == '#')
			visible[y][x] = 'F';

		else if (visible[y][x] == 'F')
			visible[y][x] = '?';

		else if (visible[y][x] == '?')
			visible[y][x] = '#';

		printVisible(height, width);
	}
	else if (operation == '?')
	{
		if (visible[y][x] == '#')
			visible[y][x] = '?';

		else if (visible[y][x] == '?')
			visible[y][x] = '#';

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
			if ((grid[i][j] != Mines_Flag && visible[i][j] == '#') || (visible[i][j] == 'F' && grid[i][j] != Mines_Flag))
				emptyAreVisible = false;
		}


	cout << "Number of flags placed:" << flagCntr << endl;
	bool nFlagsCheck = (cntr == numOfMines && flagCntr == numOfMines);
	if (nFlagsCheck || emptyAreVisible)
	{
		cout << "Congratulations, you win!" << endl;
		//	cout << emptyAreVisible << ' ' << nFlagsCheck << endl;
		won = true;
		return 'W'; // Returns W (for win) if the user puts flags on all of the mines
	}


	if (clickedOnMine)
	{
		cout << "You clicked on a mine, you lose!" << endl;
		return 'L'; // Returns L (for lose) if the user clicks on a mine
	}

	return 'N'; // Returns N (for nil) by default

}

void getVisible(char arr[50][50])
{
	for (int i = 0; i < 50; i++)
		for (int j = 0; j < 50; j++)
			arr[i][j] = visible[i][j];
}

bool getClickedONMine()
{
	return clickedOnMine;
}

bool getWin()
{
	return won;
}