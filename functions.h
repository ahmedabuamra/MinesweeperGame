#ifndef MINESWEEPER_FUNCTIONS_H
#define MINESWEEPER_FUNCTIONS_H

#include <iostream>
#include <algorithm>
#include <set>
#include <cstring>
#include <iomanip>

#define endl '\n'
#define Mines_Flag 9 //This flag is set to 9 as it is impossible for one tile to have more than 8 mines in its adjacent tiles.
#define mineSetSize (int) minesCoordinates.size()


using std::cin;
using std::cout;
using std::set;
using std::pair;
using std::setw;

//Check the function definitions in the functions.cpp file to see the explanation for what each function does
void initVisibleToHash(int height, int width);
void getInitialValues(int &height, int &width, int &numOfMines);
void randomiseMineCoordinates(set < pair<int, int> > &minesCoordinates,
										int numOfMines, int height, int width);
void setValueForTile(int r, int c);
void setValuesForGrid(int height, int width);
void placeMines(set < pair<int, int> > &minesCoordinates);
void showMinesIfLost(int height, int width);
void printVisible (int height, int width);
void openTiles(int y, int x, int height, int width);
void clickTile(int height, int width);
char endGame(int height, int width, int numOfMines);
void playGame(int height, int width, int numOfMines);



#endif //MINESWEEPER_FUNCTIONS_H
