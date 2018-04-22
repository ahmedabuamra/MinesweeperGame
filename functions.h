#ifndef MINESWEEPER_FUNCTIONS_H
#define MINESWEEPER_FUNCTIONS_H

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <set>
#include <cstring>
#include <iomanip>
#include <chrono> //Includes the time related functions implemented in C++11

#define endl '\n'
#define Mines_Flag 9 //This flag is set to 9 as it is impossible for one tile to have more than 8 mines in its adjacent tiles.
#define genericScoreNum 999999999999999
#define mineSetSize (int) minesCoordinates.size()

//For regular input and output, and manipulation.
using std::cin;
using std::cout;
using std::setw;
using std::getline;

//For file input and output
using std::ofstream;
using std::ifstream;

//For containers
using std::string;
using std::vector;
using std::set;
using std::pair;

//For time related functions from the chrono file header.
using std::chrono::steady_clock;
using std::chrono::minutes;
using std::chrono::seconds;
using std::chrono::milliseconds;
using std::chrono::duration_cast;


struct player
{
	string name = "GENERIC";
	long long minutes = genericScoreNum, seconds = genericScoreNum;
	long long milliSeconds = genericScoreNum;
};


//Check the function definitions in the functions.cpp file to see the explanation for what each function does


void charline(int x, char c, char spaces);

//Scoreboard functions
void initialiseVector(vector <player> &topPlayers);
void readScoreboard(vector <player> &topPlayers);
void printScoreboard(vector <player> &topPlayers);
void writeScoreboard(vector <player> &topPlayers);
void swapStructs(struct player* first, struct player* second);
void sortScoreboard(vector <player> &topPlayers);
int checkPlayerScore(struct player mainPlayer, vector <player> &topPlayers);
void readyScoreboard(vector <player> &topPlayers);

//Game functions
void initVisibleToHash(int height, int width);
void chooseDifficulty(char c, int &height, int &width, int &numOfMines);
void initialiseGame(int &height, int &width, int &numOfMines, vector <player> &topPlayers, struct player &mainPlayer);
void randomiseMineCoordinates(set < pair<int, int> > &minesCoordinates,
int numOfMines, int height, int width);
void setValueForTile(int r, int c);
void setValuesForGrid(int height, int width);
void placeMines(set < pair<int, int> > &minesCoordinates);
void showMinesIfLost(int height, int width);
void printVisible(int height, int width);
void openTiles(int y, int x, int height, int width);
void clickTile(int height, int width, int y, int x, char operation);
void getVisiable(char arr[50][50]);
char endGame(int height, int width, int numOfMines);
void playGame(int height, int width, int numOfMines, struct player mainPlayer, vector <player> &topPlayers);



#endif //MINESWEEPER_FUNCTIONS_H
