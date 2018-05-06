/*
*****************************************************
Important !											*
- To run this project you require :					*
*
* SFML Library										*
https://www.sfml-dev.org							*
*
*TGUI Library (based on SFML)						*
https://tgui.eu/									*
*
*****************************************************
*/
#include"GUIFunctions.h"

int main()
{
	soundPlay("music", "sounds/Music.flac", true);

	try
	{
		mainMenu();
	}

	catch (const tgui::Exception& e)
	{
		cout << "Failed to load TGUI widgets: " << e.what() << endl;
		return 1;
	}

	mainWindowDisplay(1024, 768, "Minesweeper");

	return 0;
}
