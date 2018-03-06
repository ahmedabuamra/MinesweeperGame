#include <bits/stdc++.h>
#define MINES_FLAG 100000000
using namespace std;
set<pair<int, int> > minesCounter;
int grid[50][50], numOfMines, width, height;

void setValue(int r, int c) {
	int cnt = 0;
	for (int row = r - 1; row <= r + 1; row++)
		for (int col = c - 1; col <= c + 1; col++)
			if (grid[row][col] == MINES_FLAG)
				cnt++;
	grid[r][c] = cnt;
}

int main() {
	srand(time(NULL));
	cin >> numOfMines >> height >> width;

	while ((int) minesCounter.size() < numOfMines) {
		int y = rand() % height + 1;
		int x = rand() % width + 1;
		minesCounter.insert(make_pair(y, x));
	}

	for (set<pair<int, int> >::iterator it = minesCounter.begin(); it != minesCounter.end(); it++)
		grid[it->first][it->second] = MINES_FLAG;

	for (int row = 1; row <= height; row++)
		for (int col = 1; col <= width; col++)
			if (grid[row][col] != MINES_FLAG)
				setValue(row, col);

	for (int row = 1; row <= height; row++) {
		cout << "|";
		for (int col = 1; col <= width; col++) {
			if (grid[row][col] == MINES_FLAG)
				cout << "#|";
			else
				cout << grid[row][col] << "|";
		}
		cout << '\n';
	}
}
