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
		topPlayers[i].minutes = topPlayers[i].seconds / 60; ;
		topPlayers[i].seconds %= 60;
	}
	scoreboardI.close();
}

void printScoreboard(vector <player> &topPlayers)
{
	//A function that prints the scoreboard to the player.
	int width = 4, width2 = 30;
	cout << "\t\t" << "Name" << setw(27) << "Minutes |" << "\t" << "Seconds" << endl;
	charline(47, '-', 'N');

	for (int i = 0; i < 10; i++)
	{
		if (i == 9) width = 3;

		if (topPlayers[i].milliSeconds != genericScoreNum)
			cout << i + 1 << setw(width) << " - " << topPlayers[i].name <<
			setw(width2 - topPlayers[i].name.size()) <<
			topPlayers[i].minutes << "\t\t\t" << topPlayers[i].seconds << endl;

		else cout << i + 1 << setw(width) << " - " << topPlayers[i].name <<
			setw(width2 - topPlayers[i].name.size()) <<
			"N/A" << "\t\t\t" << "N/A" << endl;
	}

}

void writeScoreboard(vector <player> &topPlayers)
{
	//A function that writes the scoreboard to the scoreboard.scrb file.
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

void swapStructs(struct player* first, struct player* second)
{
	//A function that swaps 2 structs using pointers.
	player temp;
	temp = *first;
	*first = *second;
	*second = temp;
}

void sortScoreboard(vector <player> &topPlayers)
{
	//A function that sorts the scoreboard in decreasing order.
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			if (topPlayers[j].milliSeconds > topPlayers[j + 1].milliSeconds)
			{
				swapStructs(&topPlayers[j], &topPlayers[j + 1]);
			}
		}
	}
}

int checkPlayerScore(struct player mainPlayer, vector <player> &topPlayers)
{
	// A function that checks if the player should enter the scoreboard, if so
	//it returns the index that they should stand in.

	int index(-1);
	for (int i = 0; i < 10; i++)
		if (mainPlayer.milliSeconds < topPlayers[i].milliSeconds)
		{
			index = i;
			vector <player>::iterator it;
			it = topPlayers.begin() + index;

			topPlayers.insert(it, mainPlayer);
			topPlayers.pop_back();
			return index;
		}
	return index; //returns -1 by default
}

void readyScoreboard(vector <player> &topPlayers)
{
	//A function that initialises the vector, reads the scoreboard, sorts the scoreboard,
	// and then writes the scoreboard again to the file.
	initialiseVector(topPlayers);
	readScoreboard(topPlayers);
	sortScoreboard(topPlayers);
	writeScoreboard(topPlayers);
}


//Game functions
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

void initialiseGame(int &height, int &width, int &numOfMines, vector <player> &topPlayers, struct player &mainPlayer)
{
	//A function that passes the height, width and the number of mines in the grid, as well as the user's name
	//by passing them by reference.

	cout << setw(15) << "Welcome to Minesweeper!" << endl;

	while (true)
	{
		cout << "Enter 'P' to play, 'S' to see the scoreboard, 'R' to reset the scoreboard: ";
		char choice;
		cin >> choice;
		if (choice == 'P')
		{
			cout << "Enter your name: ";

			cin.ignore();

			string name;
			getline(cin, name);

			mainPlayer.name = name;

			cout << setw(0) << "Enter the difficulty you want to play (E, M, H, B, C): ";
			char diff;
			cin >> diff;
			chooseDifficulty(diff, height, width, numOfMines);

			break;
		}

		else if (choice == 'S')
		{
			readyScoreboard(topPlayers);
			printScoreboard(topPlayers);
		}

		else if (choice == 'R')
		{
			topPlayers.clear();

			initialiseVector(topPlayers);
			writeScoreboard(topPlayers);
		}
	}
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

void getVisiable(char arr[50][50]) 
{
	for (int i = 0; i < 50; i++)
		for (int j = 0; j < 50; j++)
			arr[i][j] = visible[i][j];
}

/*
void playGame(int height, int width, int numOfMines, struct player mainPlayer, vector <player> &topPlayers)
{
	auto start = steady_clock::now(); //Sets a start for counting time.

	cout << "Enter tile address and then the command(?,F,O): ";

	//Playing the game!
	do
	{
		clickTile(height, width);
	} while (endGame(height, width, numOfMines) == 'N');

	auto end = steady_clock::now(); //Sets an end for counting time.

	auto tMilliseconds = duration_cast <milliseconds> (end - start).count(); //Counting milliseconds

																			 //Calculating Minutes and Seconds.
	long long  tSeconds = tMilliseconds / 1000;
	long long  tMinutes = tSeconds / 60;
	tSeconds %= 60;

	//Assigning the values to the player's info in the struct.
	mainPlayer.milliSeconds = tMilliseconds;
	mainPlayer.minutes = tMinutes;
	mainPlayer.seconds = tSeconds;

	readyScoreboard(topPlayers);
	int check = checkPlayerScore(mainPlayer, topPlayers);



	if (tMinutes > 0) //Condition that makes sure to print the minutes only if they are larger than 0.
	{
		cout << "The game took " << tMinutes << " minutes, and "
			<< tSeconds << " seconds." << endl;
	}

	else
	{
		cout << "The game took " << tSeconds << " seconds." << endl;
	}

	if (won)
	{
		if (check > -1)
			cout << "New highscore! You are no #" << check + 1 << " on the scoreboard!" << endl;

		printScoreboard(topPlayers);
		writeScoreboard(topPlayers);
	}
}
*/
