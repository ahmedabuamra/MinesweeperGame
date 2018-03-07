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


void setValue(int r, int c);
void printVisible(int height, int width);
void checkEmpty (int r, int c, int direction);


int main()
{
    memset(visible, '#', sizeof(visible)); //Initialising the visible grid with the character '#'

    //Variables
    set < pair<int, int> > minesCoordinates;
    int numOfMines, width, height;

    //Linking srand with time to achieve less limited randomness
    srand(time(NULL));

    //Input
    cin >> height >> width >> numOfMines;

    //Randomising coordinates for mines
    while (mineSetSize < numOfMines)
    {
        int y = rand() % height + 1;
        int x = rand() % width + 1;
        minesCoordinates.insert({y, x});
    }

    //Placing the mines
    for (set<pair<int, int> >::iterator it = minesCoordinates.begin(); it != minesCoordinates.end(); it++)
    grid[it->first][it->second] = Mines_Flag;


    //Setting the numerical values for the tiles on the actual grid, indicating the number of mines adjacent to each tile.
    for (int row = 1; row <= height; row++)
    {
        for (int col = 1; col <= width; col++)
        {
            if (grid[row][col] != Mines_Flag)
                setValue(row, col);
        }
    }

    //Printing out the actual grid for testing
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

    int x(0),y;//Dummy variables to take input from the user

    while (x!=20) //For testing only, entering x = 20 terminates the input from the user.
    {
        cin >> y >> x;
        if (grid[y][x]==Mines_Flag)
            cout << "You encountered a mine!";
        else
            visible[y][x] = (char) grid[y][x]+'0';

        for (int i = 1; i < 8; i++)
            checkEmpty(y, x, i);

        printVisible(height,width); //Prints the grid that is visible to the user
    }
	
	return 0;
}

void setValue(int r, int c)
{
    //Function that sets the values for the tiles based on the mines in the adjacent tiles around them
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



        if (grid[r][c] == Mines_Flag)
            break;
        else if (grid[r][c] !=0)
        {
            visible[r][c] = (char) grid[r][c] + '0';
            break;
        }
    }
}
