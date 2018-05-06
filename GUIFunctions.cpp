#include "GUIFunctions.h"

//Global variables declarations

// Create Window and gui
sf::RenderWindow* window;
tgui::Gui gui;

auto theme = tgui::Theme::create("art/Black.txt");
auto windowWidth = tgui::bindWidth(gui);     // width of the window
auto windowHeight = tgui::bindHeight(gui);   // height of the window
std::vector<tgui::Widget::Ptr> Menu_Widgets; // Storing all created Gui widgets

// Input Boxes
tgui::EditBox::Ptr E_height;
tgui::EditBox::Ptr E_width;
tgui::EditBox::Ptr E_numOfmines;
tgui::EditBox::Ptr PlayerName;

//Text for name and time
tgui::Label::Ptr name_time;

// Sounds
sf::SoundBuffer buffer;
sf::Music music;
sf::Sound sound;
set < pair<int, int> > minesCoordinates;

// Grid Buttons
tgui::Button::Ptr **button;

// Textures
tgui::Texture bar, Hbar, Cbar;
tgui::Texture tile, Htile, Ctile;
tgui::Texture tilenums[9];
tgui::Texture mine, Emine, flaggedMine, flag, QMark;
tgui::Texture Enter_Button, HEnter_Button;
tgui::Texture textbox;
tgui::Texture timer;
char my_visible[50][50];

vector <player> topPlayers(20); // Vector to hold the top players.
player mainPlayer = {}; //Struct to hold the data for the player.

bool lanched_Level;   // detect if the grid is created
int Gwidth = 8;   //Global width of the grid
int Gheight = 8;  //Global height of the grid
int GMinesnum = 10;// Global number of mines
int flgcntr = 0;
string gameplay_info = "";
bool iswon, finishedCurrentLevel=false;      // makes sure win function is called once


//time calc.
steady_clock::time_point clock_begin;
steady_clock::duration time_span;
long long second_cnt;

//End of global variables declarations

void mainWindowDisplay(float W_width, float W_height, std::string W_name)
{
	//Function that displays the window and detects GUI events

	window = new sf::RenderWindow(sf::VideoMode(W_width, W_height), W_name, sf::Style::Titlebar | sf::Style::Close);

	gui.setWindow(*window); // Create the gui and attach it to the window.
	//Main loop
	while (window->isOpen())
	{
		if (lanched_Level && !finishedCurrentLevel)
		{
			//side part of level
			auto clock_end = steady_clock::now();
			auto tMilliseconds = duration_cast <milliseconds> (clock_end - clock_begin).count(); //Counting milliseconds
			second_cnt = tMilliseconds / 1000;
			gameplay_info = "    " + std::to_string(flgcntr);
			name_time->setText(mainPlayer.name + "\n\n\n" + gameplay_info + "\n\n\n\n          " + std::to_string(second_cnt));
		}
		sf::Event event;
		while (window->pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window->close();
			if (lanched_Level)
			{
				// Loop to add flag or Qmark (?) when right clicked
				for (int i = 0; i < Gheight; i++)
				{
					for (int j = 0; j < Gwidth; j++)
					{

						if (button[i][j]->mouseOnWidget(event.mouseButton.x, event.mouseButton.y) && button[i][j]->isEnabled())
						{
							if (event.type == sf::Event::MouseButtonPressed&&event.mouseButton.button == sf::Mouse::Right)
							{
								clickTile(Gheight, Gwidth, i, j, 'F');
								getVisible(my_visible);
								if (my_visible[i + 1][j + 1] == 'F')
								{
									button[i][j]->getRenderer()->setNormalTexture(flag);
									button[i][j]->getRenderer()->setHoverTexture(flag);
									endGame(Gheight, Gwidth, GMinesnum);
									button[i][j]->disconnectAll();
									flgcntr++;
								}
								else if (my_visible[i + 1][j + 1] == '?')
								{
									button[i][j]->getRenderer()->setNormalTexture(QMark);
									button[i][j]->getRenderer()->setHoverTexture(QMark);
									button[i][j]->disconnectAll();
									flgcntr--;
								}
								else
								{
									button[i][j]->getRenderer()->setNormalTexture(tile);
									button[i][j]->getRenderer()->setHoverTexture(Htile);
									button[i][j]->connect("pressed", soundPlay, "sound", "sounds/click.flac", false);
									button[i][j]->connect("pressed", store, i, j, Gwidth, Gheight, 'O');
									button[i][j]->connect("pressed", endGame, Gheight, Gwidth, GMinesnum);
								}
							}
						}
					}

				}
			}

			if (getWin() && !iswon)
			{
				finishedLevel(Gwidth, Gheight, true);
				soundPlay("sound", "sounds/win.flac", false);
				iswon = true;
			}

			gui.handleEvent(event); // Pass the event to the widgets
		}
		window->clear();
		gui.draw(); // Draw all widgets
		window->display();
	}
}

void soundPlay(string type, string path, bool isLoop)
{
	// Function that plays sound based on :
	// type : music or sound
	// path : the path for the sound file
	// Loop the sound or play it once
	if (type == "Sound" || type == "sound")
	{
		if (!buffer.loadFromFile(path))
		{
			std::cout << "ERROR Playing sound \n";
		}

		sound.setBuffer(buffer);
		sound.play();
		sound.setLoop(isLoop);
	}

	else if (type == "Music" || type == "music")
	{
		if (!music.openFromFile(path))
			cout << "Error \n";
		music.play();
		music.setLoop(isLoop);
	}
}


void mainMenu()
{
	//The start function for the game
	// Displays the MainMenu elements

	//pre::keep the database
	readyScoreboard(topPlayers);


	// Load Textures from files
	bar.load("art/activebar.png");
	Hbar.load("art/hbar.png");
	Cbar.load("art/closedbar.png");
	Enter_Button.load("art/mainmenuitems/enter.png");
	HEnter_Button.load("art/mainmenuitems/Henter.png");
	tile.load("art/closedtile.png");
	Htile.load("art/hoveredtile.png");
	Ctile.load("art/emptytile.png");

	for (int i = 0; i < 9; i++)
	{
		string temp = ".png";
		tilenums[i].load("art/" + std::to_string(i) + temp);
	}

	mine.load("art/mine.png");
	Emine.load("art/explodedmine.png");
	flaggedMine.load("art/xmine.png");
	flag.load("art/flag.png");
	QMark.load("art/qmark.png");
	textbox.load("art/mainmenuitems/textbox.png");
	timer.load("art/mainmenuitems/timer.png");

	//Add background
	tgui::Picture::Ptr Bpicture = tgui::Picture::create("art/BackGround.png");
	Bpicture->setSize(tgui::bindMax(1024, windowWidth), tgui::bindMax(768, windowHeight));
	gui.add(Bpicture);

	// Add Menu Background
	tgui::Picture::Ptr M_picture = tgui::Picture::create("art/emptymenu.png");
	M_picture->setSize(windowWidth - 250, windowHeight - 150);
	M_picture->setPosition(windowWidth / 7.6, windowHeight / 7);
	M_picture->showWithEffect(tgui::ShowAnimationType::Fade, sf::milliseconds(800));
	Menu_Widgets.insert(Menu_Widgets.end(), M_picture);
	gui.add(M_picture);

	//Add Logo
	tgui::Picture::Ptr logo = tgui::Picture::create("art/logo.png");
	logo->setSize(windowWidth / 3, windowHeight / 7);
	logo->setPosition(windowWidth / 3, 90);
	logo->showWithEffect(tgui::ShowAnimationType::Fade, sf::milliseconds(800));
	Menu_Widgets.insert(Menu_Widgets.end(), logo);
	gui.add(logo);

	// Add checkbox
	tgui::CheckBox::Ptr c = tgui::CheckBox::create();
	c->setSize(40, 40);
	c->setPosition(20, 20);
	c->getRenderer()->setForegroundColor("Yellow");
	c->setText("Music");
	c->setFont("art/arcadeclassic.TTF");
	if (music.getStatus() == music.Stopped)            c->uncheck();
	else if (music.getStatus() == music.Playing)         c->check();
	c->connect("checked", [&]() { music.play(); });
	c->connect("unchecked", [&]() { music.stop(); });
	Menu_Widgets.insert(Menu_Widgets.end(), c);
	gui.add(c);

	// Add buttons
	tgui::Button::Ptr M_buttons[4];
	tgui::Texture txts[4];
	tgui::Texture Htxts[4];
	string temp = ".png";

	float v = 530;

	for (int i = 0; i < 4; i++)
	{
		txts[i].load("art/mainmenuitems/m" + std::to_string(i) + temp);
		Htxts[i].load("art/mainmenuitems/Hm" + std::to_string(i) + temp);
		M_buttons[i] = tgui::Button::create();
		M_buttons[i]->getRenderer()->setNormalTexture(txts[i]);
		M_buttons[i]->getRenderer()->setHoverTexture(Htxts[i]);
		M_buttons[i]->setFont("art/arcadeclassic.TTF");
		M_buttons[i]->setSize(500, 100);
		M_buttons[i]->setPosition(windowWidth / 3.8, windowHeight - v);
		v = v - 120;
		M_buttons[i]->showWithEffect(tgui::ShowAnimationType::Fade, sf::milliseconds(800));
		Menu_Widgets.insert(Menu_Widgets.end(), M_buttons[i]);
		gui.add(M_buttons[i]);
		M_buttons[i]->connect("pressed", soundPlay, "sound", "sounds/click.flac", false);
		M_buttons[i]->connect("pressed", disableWidgets);
	}
	M_buttons[0]->connect("pressed", nameInput);
	M_buttons[1]->connect("pressed", showScoreBoard);
	M_buttons[2]->connect("pressed", credits);
	M_buttons[3]->connect("pressed", [&]() { window->close(); });


}

void levelMenu()
{
	//Function that displays difficulty choices

	//Add Logo
	tgui::Picture::Ptr logo = tgui::Picture::create("art/logo.png");
	logo->setSize(windowWidth / 2, windowHeight / 6);
	logo->setPosition(windowWidth / 2, 0);
	Menu_Widgets.insert(Menu_Widgets.end(), logo);
	gui.add(logo);

	//Add difficulty Buttons
	tgui::Button::Ptr button[5];
	//Start position for buttons
	float s = 760;

	int level = 0;
	string temp = ".png";
	tgui::Texture dif[5];
	tgui::Texture Hdif[5];

	for (int i = 0; i < 5; i++)
	{
		dif[i].load("art/mainmenuitems/d" + std::to_string(i) + temp);
		Hdif[i].load("art/mainmenuitems/Hd" + std::to_string(i) + temp);
		button[i] = tgui::Button::create();
		button[i]->getRenderer()->setNormalTexture(dif[i]);
		button[i]->getRenderer()->setHoverTexture(Hdif[i]);
		button[i]->setSize(windowWidth / 3, windowHeight / 6);
		button[i]->setPosition(windowWidth / 6, windowHeight - s);
		button[i]->showWithEffect(tgui::ShowAnimationType::SlideFromLeft, sf::milliseconds(800));
		gui.add(button[i]);
		s -= 150;
		Menu_Widgets.insert(Menu_Widgets.end(), button[i]);
		button[i]->connect("pressed", soundPlay, "sound", "sounds/click.flac", false);
		button[i]->connect("pressed", disableWidgets);
		if (i != 4) button[i]->connect("pressed", levelSelect, level + 1);

		if (i == 4)
		{
			button[i]->connect("pressed", customLevel);
		}

		level++;

	}
	level = 0;

	mainPlayer.name = PlayerName->getText().toAnsiString();
	backButton();
}

void levelSelect(int level)
{
	//Function that specifies grid size based on Level selection

	switch (level)
	{
		case 1:
			//Easy_Level
			launchLevel(8, 8, 10);
			break;
		case 2:
			// Medium_Level
			launchLevel(10, 10, 20);
			break;

		case 3:
			//Hard_Level
			launchLevel(15, 15, 45);
			break;

		case 4:
			// Brutal_Level
			launchLevel(20, 20, 80);
			break;

		default:
			//Custom Level
			int width = 0;
			int height = 0;
			int numOfmines = 0;
			//Taking inputBoxes string and adding it to int
			std::stringstream inp_width(E_width->getText().toAnsiString());
			std::stringstream inp_height(E_height->getText().toAnsiString());
			std::stringstream inp_numOfmines(E_numOfmines->getText().toAnsiString());
			inp_width >> width;
			inp_height >> height;
			inp_numOfmines >> numOfmines;

			if (numOfmines < width*height && numOfmines > 0)
			{
				disableWidgets();
				launchLevel(width, height, numOfmines);
			}

			else
			{
				tgui::MessageBox::Ptr m = theme->load("MessageBox");
				m->setSize(400, 400);
				m->setPosition(200, 400);
				m->setText("Mines must be > 0 and < width*height");
				m->setTextSize(36);
				gui.add(m);
			}
			break;
	}
}

void launchLevel(int width, int height, int numOfmines)
{
	//Function that starts the Level and displays the grid tiles

	initVisibleToHash(height, width);
	randomiseMineCoordinates(minesCoordinates, numOfmines, height, width);
	placeMines(minesCoordinates);
	setValuesForGrid(height, width);
	iswon = false, finishedCurrentLevel = false;
	flgcntr = 0;
	clock_begin = steady_clock::now(); //record the start time

	float v = 0;
	float H = 0;
	button = new tgui::Button::Ptr*[height];

	for (int i = 0; i < height; i++)
	{
		button[i] = new tgui::Button::Ptr[width];
	}

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			button[i][j] = tgui::Button::create();
			button[i][j]->getRenderer()->setNormalTexture(tile);
			button[i][j]->getRenderer()->setHoverTexture(Htile);
			button[i][j]->getRenderer()->setDownTexture(Ctile);
			button[i][j]->setSize((windowWidth - 200) / (width), (windowHeight) / (height));
			button[i][j]->setPosition(windowWidth - windowWidth + H, windowHeight - windowHeight + v);
			button[i][j]->connect("pressed", soundPlay, "sound", "sounds/click.flac", false);
			button[i][j]->connect("pressed", store, i, j, width, height, 'O');
			button[i][j]->connect("pressed", endGame, height, width, numOfmines);
			button[i][j]->showWithEffect(tgui::ShowAnimationType::Fade, sf::milliseconds(800));
			gui.add(button[i][j]);
			Menu_Widgets.insert(Menu_Widgets.end(), button[i][j]);
			H += 824 / width;
		}

		v += 768 / height;
		H = 0;

	}

	Gwidth = width;
	Gheight = height;
	GMinesnum = numOfmines;
	lanched_Level = true;
	name_timeDisplay();
	backButton();
}

void customLevel()
{
	//Function that diplays and takes input for the custom Level

	//Add inputbox for width
	E_width = tgui::EditBox::create();
	E_width->getRenderer()->setNormalTexture(textbox);
	E_width->setSize(windowWidth / 5, windowHeight / 12);
	E_width->setPosition(windowWidth / 8, windowHeight / 6);
	E_width->setDefaultText("Width");
	E_width->showWithEffect(tgui::ShowAnimationType::SlideFromLeft, sf::milliseconds(800));
	gui.add(E_width);
	Menu_Widgets.insert(Menu_Widgets.end(), E_width);
	// Add inputbox for height
	E_height = tgui::EditBox::create();
	E_height->getRenderer()->setNormalTexture(textbox);
	E_height->setSize(windowWidth / 5, windowHeight / 12);
	E_height->setPosition((windowWidth / 8) * 3, windowHeight / 6);
	E_height->setDefaultText("Height");
	E_height->showWithEffect(tgui::ShowAnimationType::SlideFromLeft, sf::milliseconds(800));
	gui.add(E_height);
	Menu_Widgets.insert(Menu_Widgets.end(), E_height);
	//Add inputbox for numOfmines
	E_numOfmines = tgui::EditBox::create();
	E_numOfmines->getRenderer()->setNormalTexture(textbox);
	E_numOfmines->setSize(windowWidth / 5, windowHeight / 12);
	E_numOfmines->setPosition((windowWidth / 8) * 5, windowHeight / 6);
	E_numOfmines->setDefaultText("Mines");
	E_numOfmines->showWithEffect(tgui::ShowAnimationType::SlideFromLeft, sf::milliseconds(800));
	gui.add(E_numOfmines);
	Menu_Widgets.insert(Menu_Widgets.end(), E_numOfmines);

	//Add enter button
	tgui::Button::Ptr b = tgui::Button::create();
	b->getRenderer()->setNormalTexture(Enter_Button);
	b->getRenderer()->setHoverTexture(HEnter_Button);
	b->setSize(windowWidth / 2, windowHeight / 6);
	b->setPosition(windowWidth / 4.5, windowHeight / 6 + windowHeight / 6);
	Menu_Widgets.insert(Menu_Widgets.end(), b);
	b->connect("pressed", soundPlay, "sound", "sounds/click.flac", false);
	b->connect("pressed", levelSelect, 5);
	b->showWithEffect(tgui::ShowAnimationType::SlideFromLeft, sf::milliseconds(800));
	gui.add(b);
	backButton();

}

void disableWidgets()
{
	// Function that disable the previous widgets to create a different menu

	for (int i = 0; i<Menu_Widgets.size(); i++)
	{
		Menu_Widgets[i]->hide();
		gui.remove(Menu_Widgets[i]);
	}
	Menu_Widgets.clear();

}

void backButton()
{
	//Add back button
	tgui::Button::Ptr back_Button = tgui::Button::create();
	back_Button->getRenderer()->setNormalTexture(bar);
	back_Button->getRenderer()->setHoverTexture(Hbar);
	back_Button->getRenderer()->setDownTexture(Cbar);
	back_Button->setSize(windowWidth / 6, windowHeight / 12);
	back_Button->setText("MainMenu");
	back_Button->getRenderer()->setTextColor("White");
	back_Button->setFont("art/arcadeclassic.TTF");
	back_Button->setPosition(windowWidth - windowWidth / 6, windowHeight - windowHeight / 12);
	Menu_Widgets.insert(Menu_Widgets.end(), back_Button);
	back_Button->connect("pressed", soundPlay, "sound", "sounds/click.flac", false);
	back_Button->connect("pressed", disableWidgets);
	back_Button->connect("pressed", mainMenu);
	back_Button->showWithEffect(tgui::ShowAnimationType::Fade, sf::milliseconds(800));
	gui.add(back_Button);
}


void store(int i, int j, int width, int height, char operation)
{
	//Function that displays the grid to the tiles buttons

	clickTile(height, width, i, j, operation);
	getVisible(my_visible);
	for (int k = 0; k < height; k++)
	{
		for (int l = 0; l < width; l++)
		{
			if (my_visible[k + 1][l + 1] != '#')
			{
				if (my_visible[k + 1][l + 1] != '@'&&my_visible[k + 1][l + 1] != '+'&&my_visible[k + 1][l + 1] != 'F'&&my_visible[k + 1][l + 1] != '%')
				{
					button[k][l]->getRenderer()->setNormalTexture(tilenums[my_visible[k + 1][l + 1] - '0']);
					button[k][l]->disable();
				}
				else if (my_visible[k + 1][l + 1] == '@')
				{
					button[k][l]->getRenderer()->setNormalTexture(Emine);
					soundPlay("sound", "sounds/explode.flac", false);
					finishedLevel(width, height, false);
				}
				else if (my_visible[k + 1][l + 1] == '+')
				{
					button[k][l]->getRenderer()->setNormalTexture(mine);
					button[k][l]->disable();
				}
				else if (my_visible[k + 1][l + 1] == '%')
				{
					button[k][l]->getRenderer()->setNormalTexture(flaggedMine);
					button[k][l]->disable();
				}

			}
			else if (my_visible[k + 1][l + 1] == '#')
			{
				button[k][l]->getRenderer()->setNormalTexture(tile);
				button[k][l]->getRenderer()->setHoverTexture(Htile);

			}

		}
	}
}

void finishedLevel(int width, int height, bool won)
{
	//Function that detects and displays Losing and wining

	tgui::Label::Ptr txt;
	tgui::Button::Ptr again;


	// Add label text for win and lose
	txt = tgui::Label::create();
	txt->setSize(400, 400);
	txt->setTextSize(72);
	txt->setPosition(850, 200);
	txt->setFont("art/arcadeclassic.TTF");
	txt->setTextColor("White");

	//End Time & duration

	if (won)
	{
		txt->setText("You \n \n Win!");
		auto clock_end = steady_clock::now();
		auto tMilliseconds = duration_cast <milliseconds> (clock_end - clock_begin).count(); //Counting milliseconds

		//Calculating Minutes and Seconds.
		long long  tSeconds = tMilliseconds / 1000;
		tSeconds %= 60;

		//Assigning the values to the player's info in the struct.
		mainPlayer.milliSeconds = tMilliseconds;
		mainPlayer.seconds = tSeconds;
		topPlayers.push_back(mainPlayer);
		writeScoreboard(topPlayers);
	}
	else
	{
		txt->setText("You \n \n Lose!");
	}

	Menu_Widgets.insert(Menu_Widgets.end(), txt);
	gui.add(txt);

	// Add PlayAgain Button
	again = tgui::Button::create();
	again->getRenderer()->setNormalTexture(bar);
	again->getRenderer()->setHoverTexture(Hbar);
	again->getRenderer()->setDownTexture(Cbar);
	again->setSize(windowWidth / 6, windowHeight / 12);
	again->setText("PlayAgain!");
	again->getRenderer()->setTextColor("White");
	again->setFont("art/arcadeclassic.TTF");
	again->setPosition(windowWidth - windowWidth / 6, windowHeight - windowHeight / 4);
	Menu_Widgets.insert(Menu_Widgets.end(), again);
	again->connect("pressed", soundPlay, "sound", "sounds/click.flac", false);
	again->connect("pressed", [&]() { music.play(); });
	again->connect("pressed", disableWidgets);
	again->connect("pressed", levelMenu);
	gui.add(again);

	music.pause();
	finishedCurrentLevel = true;

	// Disable pressing for the remaining grid tiles
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			button[i][j]->disable();
		}
	}
}

void credits()
{
	// Add label text
	tgui::Label::Ptr text;
	text = tgui::Label::create();
	text->setSize(windowWidth - 200, windowHeight - 200);
	text->setTextSize(46);
	text->setPosition(windowWidth / 6, 10);
	text->setFont("art/arcadeclassic.TTF");
	text->setText("Developers  \nAhmed Elmayyah\t!Satharus !  \nAhmed Aboamra\t!ahmedabuamra !  \n "
								 "Andrew Awni\t!andrewawni ! \n AbdulRahman Yousry\t!slashdevo ! \n\n"
		//[Potentially]	 "Music \n Price of Failure  by  Perturbator \n Zero Gravity  by  Dynatron "
	);

	Menu_Widgets.insert(Menu_Widgets.end(), text);
	backButton();
	gui.add(text);
}

void nameInput()
{
	//Add inputbox for width
	PlayerName = tgui::EditBox::create();
	PlayerName->getRenderer()->setNormalTexture(textbox);
	PlayerName->setSize(windowWidth / 2, windowHeight / 12);
	PlayerName->setPosition(windowWidth / 4, windowHeight / 6);
	PlayerName->setDefaultText("Name....");
	PlayerName->showWithEffect(tgui::ShowAnimationType::SlideFromLeft, sf::milliseconds(800));
	gui.add(PlayerName);
	Menu_Widgets.insert(Menu_Widgets.end(), PlayerName);

	//Add enter button
	tgui::Button::Ptr b = tgui::Button::create();
	b->getRenderer()->setNormalTexture(Enter_Button);
	b->getRenderer()->setHoverTexture(HEnter_Button);
	b->setSize(windowWidth / 2, windowHeight / 6);
	b->setPosition(windowWidth / 4.5, windowHeight / 6 + windowHeight / 6);
	Menu_Widgets.insert(Menu_Widgets.end(), b);
	b->connect("pressed", soundPlay, "sound", "sounds/click.flac", false);
	b->connect("pressed", disableWidgets);
	b->connect("pressed", levelMenu);
	b->showWithEffect(tgui::ShowAnimationType::SlideFromLeft, sf::milliseconds(800));
	gui.add(b);

	backButton();
}

void showScoreBoard()
{
	//A function that prints the scoreboard to the player.
	readyScoreboard(topPlayers);

	// Add label text
	tgui::Label::Ptr Dtext;
	Dtext = tgui::Label::create();
	Dtext->setSize(600, 60);
	Dtext->setTextStyle(sf::Text::Style::Bold);
	Dtext->setTextStyle(sf::Text::Style::Underlined);
	Dtext->setTextSize(24);
	Dtext->setPosition(100, 10);
	string top = "Name                                                                     Time";
	Dtext->setText(top);
	Menu_Widgets.insert(Menu_Widgets.end(), Dtext);
	gui.add(Dtext);

	float v = 70;
	for (int i = 0; i < 10; i++)
	{
		string name = topPlayers[i].name;
		string score = std::to_string(topPlayers[i].milliSeconds / 1000);
		tgui::Label::Ptr text[20];
		text[i] = tgui::Label::create();
		text[i]->setSize(500, 60);
		text[i]->setTextColor("Black");
		text[i]->setTextStyle(sf::Text::Style::Bold);
		text[i]->setTextSize(18);
		text[i]->setPosition(100, v);
		Menu_Widgets.insert(Menu_Widgets.end(), text[i]);
		gui.add(text[i]);
		text[i]->setText(name);

		text[i+10] = tgui::Label::create();
		text[i + 10]->setSize(500, 60);
		text[i + 10]->setTextColor("Black");
		text[i + 10]->setTextStyle(sf::Text::Style::Bold);
		text[i + 10]->setTextSize(18);
		text[i + 10]->setPosition(600, v);
		Menu_Widgets.insert(Menu_Widgets.end(), text[i + 10]);
		gui.add(text[i + 10]);
		text[i + 10]->setText(score);

		v += 60;
	}
	backButton();
}

void name_timeDisplay()
{
	//A function that displays the name and timer for the user during the game

	// Add label text for Name and time
	name_time = tgui::Label::create();
	name_time->setSize(400, 400);
	name_time->setTextSize(15);
	name_time->setPosition(835, 10);
	name_time->setTextStyle(sf::Text::Style::Bold);
	name_time->setTextColor("White");
	Menu_Widgets.insert(Menu_Widgets.end(), name_time);
	gui.add(name_time);

	tgui::Picture::Ptr flg = tgui::Picture::create(flag);
	flg->setSize(40, 40);
	flg->setPosition(880, 50);
	Menu_Widgets.insert(Menu_Widgets.end(), flg);
	gui.add(flg);

	tgui::Picture::Ptr time = tgui::Picture::create(timer);
	time->setSize(50, 25);
	time->setPosition(877, 123);
	Menu_Widgets.insert(Menu_Widgets.end(), time);
	gui.add(time);
}
