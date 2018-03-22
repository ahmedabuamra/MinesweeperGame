#include <iostream>
#include <algorithm>
#include <set>
#include <cstring>

#define endl '\n'
#define Mines_Flag 9 //This flag is set to 9 as it is impossible for one tile to have more than 8 mines in its adjacent tiles.
#define mineSetSize (int) minesCoordinates.size()


using std::cin;
using std::cout;
using std::set;
using std::pair;
using std::memset;

//Arrays are made global to prevent stack overflow, and make them easier to use with functions.
int grid[50][50];
char visible[50][50];

//Check the function definitions to see the explanation for what each function does
void setValueForTile(int r, int c);
void setValuesForGrid(int height, int width);
void printVisible (int height, int width);
void checkEmpty (int r, int c, int direction);
void checkEmptyAdjacent (int r, int c);
void printActual(int height, int width);
void getInitialValues(int &height, int &width, int &numOfMines);
void randomiseMineCoordinates(set < pair<int, int> > &minesCoordinates,
															int numOfMines, int height, int width);
void placeMines(set < pair<int, int> > &minesCoordinates);


int main()
{
    memset(visible, '#', sizeof(visible)); //Initialising the visible grid with the character '#'

    //Declaration of variables
    set < pair<int, int> > minesCoordinates;
    int numOfMines, width, height;

    //Linking srand with time to achieve less limited randomness
    srand(time(NULL));

    //Input
	getInitialValues(height, width, numOfMines);

    //Randomising coordinates for mines
	randomiseMineCoordinates(minesCoordinates, numOfMines, height, width);

    //Placing the mines
    placeMines(minesCoordinates);


    //Setting the numerical values for the tiles on the actual grid, indicating the number of mines adjacent to each tile.
    setValuesForGrid(height, width);

    //Printing out the actual grid for testing
	printActual(height, width);


	////////////////////////////FOR TESTING////////////////////////////
	//THIS BLOCK IS FOR TESTING AND WILL BE PUT INTO A FUNCTION BEFORE THE FINAL RELEASE

	int x(0),y;//Dummy variables to take input from the user

	//For testing only, entering x = 20 terminates the input from the user.

    while (x!=20)
    {
        cin >> y >> x;
        if (grid[y][x] == Mines_Flag)
            cout << "You encountered a mine!";
        else
            visible[y][x] = (char) grid[y][x] + '0';

        for (int i = 1; i < 8; i++)
            checkEmpty(y, x, i);

        printVisible(height, width); //Prints the grid that is visible to the user
    }
	////////////////////////////END OF TESTING////////////////////////////
    return 0;
}


void getInitialValues(int &height, int &width, int &numOfMines)
{
	//A function that passes the height, width and the number of mines in the grid
	//by reference to input them
	cin >> height >> width >> numOfMines;
}

void randomiseMineCoordinates(set < pair<int, int> > &minesCoordinates,
										int numOfMines, int height, int width)
{
	//A function that randomises the coordinates for the mines.
	while (mineSetSize < numOfMines)
	{
		int y = rand() % height + 1;
		int x = rand() % width + 1;
		minesCoordinates.insert({y, x});
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

void printVisible(int height, int width)
{
    //A function that prints the visible grid to the user
    for (int i = 1; i <= height; i++)
    {
        for (int j = 1; j <= width; j++)
        {
            cout << visible[i][j] << ' ';
        }
        cout << endl;
    }
}

void printActual(int height, int width)
{
	//A function that prints the actual grid to the user, used for testing only
	//It will be removed in the final release
	for (int row = 1; row <= height; row++)
	{
		cout << "|";
		for (int col = 1; col <= width; col++)
		{
			if (grid[row][col] == Mines_Flag)
				cout << "+|";

			else if (grid[row][col]==0)
				cout << "_|";

			else
				cout << grid[row][col] << '|';
		}
		cout << endl;
	}

}

void checkEmpty (int r, int c, int direction)
{
/*
 This function checks all of the 8 directions around the tile for tile that don't contain mines.
 Here are the direction where T is the tile:
              8  1  2
              7  T  3
              6  5  4
*/

    while (c>0&&r>0)
    {
        visible[r][c] = (char) grid[r][c] + '0';

			if      (direction == 1) r--;
			else if (direction == 2) {r--; c++;}
			else if (direction == 3) c++;
			else if (direction == 4) {r++; c++;}
			else if (direction == 5) r++;
			else if (direction == 6) {r++; c--;}
			else if (direction == 7) c--;
			else if (direction == 8) {r--; c--;}

			checkEmptyAdjacent (r, c);

        if (grid[r][c] == Mines_Flag)
            break;
        else if (grid[r][c] !=0)
        {
            visible[r][c] = (char) grid[r][c] + '0';
            break;
        }
    }
}

void checkEmptyAdjacent (int r, int c)
{
	//A function supposed to to check the empty tiles adjacent to the
	//current tile

	for (int row = r - 1; row <= r + 1; row++)
	{
		for (int col = c - 1; col <= c + 1; col++)
		{
			if (grid[r][c] == 0)
			{
				if (grid[row][col] != Mines_Flag)
				visible[row][col] = (char) grid[row][col] + '0';
			}
			else if (grid[r][c] > 0 && grid[r][c] < Mines_Flag)
			{
				if (grid[row][col] == 0)
					visible[row][col] = '0';
			}

		}
	}
}
