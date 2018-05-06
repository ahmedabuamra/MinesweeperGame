#ifndef MINESWEEPERGUI_GUIFUNCTIONS_H
#define MINESWEEPERGUI_GUIFUNCTIONS_H

#include <TGUI/TGUI.hpp>
#include<sstream>
#include<SFML/Audio.hpp>
#include"functions.h"

//GUI functions
void mainWindowDisplay(float W_width, float W_height, string W_name);
void mainMenu();
void levelMenu();
void levelSelect(int level);
void launchLevel(int width, int height, int numOfmines);
void disableWidgets();
void customLevel();
void backButton();
void soundPlay(string type, string path, bool isLoop);
void store(int i, int j, int width, int height, char operation);
void finishedLevel(int width, int height, bool won);
void credits();
void nameInput();
void showScoreBoard();
void name_timeDisplay();

#endif //MINESWEEPERGUI_GUIFUNCTIONS_H