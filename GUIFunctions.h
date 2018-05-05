#include <TGUI/TGUI.hpp>
#include<sstream>
#include<SFML/Audio.hpp>
#include"functions.h"

//GUI functions
void MainWindowDisplay(float W_width, float W_height, string W_name);
void MainMenu();
void LevelMenu();
void Level_Select(int level);
void Launch_Level(int width, int height, int numOfmines);
void DisableWidgets();
void CustomLevel();
void Back_Button();
void SoundPlay(string type, string path, bool isLoop);
void Store(int i, int j, int width, int height, char operation);
void FinishedLevel(int width, int height, bool won);
void Credits();
