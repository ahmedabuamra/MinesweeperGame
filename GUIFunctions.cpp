#include "GUIFunctions.h"

/* 
Global vars
*/

// Create Window and gui 
sf::RenderWindow* window;
tgui::Gui gui;

auto theme = tgui::Theme::create("widgets/Black.txt");
auto windowWidth = tgui::bindWidth(gui);     // width of the window 
auto windowHeight = tgui::bindHeight(gui);   // height of the window
std::vector<tgui::Widget::Ptr> Menu_Widgets; // Storing all created Gui widgets
// Input Boxes
tgui::EditBox::Ptr E_height;
tgui::EditBox::Ptr E_width;
tgui::EditBox::Ptr E_numOfmines;
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
tgui::Texture mine, Emine, flag,QMark;

char my_visible[50][50];
bool rc;   // detect if the grid is created 
int Gwidth = 8;   //Global width of the grid 
int Gheight = 8;  //Global height of the grid 



void MainWindowDisplay(float W_width, float W_height, std::string W_name)
{
	//Function that displays the window and detects GUI events

	 window=new sf::RenderWindow( sf::VideoMode(W_width,W_height), W_name );

	gui.setWindow(*window); // Create the gui and attach it to the window.
						   // main loop
	while (window->isOpen())
	{
		sf::Event event;
		while (window->pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window->close();

			// Loop to add flag or Qmark (?) when right clicked 
			for (int i = 0; i < Gheight; i++)
			{
				for (int j = 0; j < Gwidth; j++)
				{
					if (rc)
					{
						if (button[i][j]->mouseOnWidget(event.mouseButton.x, event.mouseButton.y) && button[i][j]->isEnabled())
						{
							if (event.type == sf::Event::MouseButtonPressed&&event.mouseButton.button == sf::Mouse::Right)
							{
								clickTile(Gheight, Gwidth, i, j, 'F');
								getVisiable(my_visible);
								if (my_visible[i + 1][j + 1] == 'F')
								{
									button[i][j]->getRenderer()->setNormalTexture(flag);
									button[i][j]->getRenderer()->setHoverTexture(flag);
								}
								else if (my_visible[i + 1][j + 1] == '?')
								{
									button[i][j]->getRenderer()->setNormalTexture(QMark);
									button[i][j]->getRenderer()->setHoverTexture(QMark);
								}
								else
								{
									button[i][j]->getRenderer()->setNormalTexture(tile);
									button[i][j]->getRenderer()->setHoverTexture(Htile);
								}
							}
						}
					}
					if (GetWin())
					{
						FinishedLevel(Gwidth,Gheight,true);
					}
				}
			}

			gui.handleEvent(event); // Pass the event to the widgets
		}

		window->clear();
		gui.draw(); // Draw all widgets
		window->display();
	}
}

void SoundPlay(string type , string path,bool isLoop)
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


void MainMenu()
{
	//The start function for the game 
	// Displays the MainMenu elements 

	// Load Textures from files 
	bar.load("art/activebar.png");
	Hbar.load("art/hbar.png");
	Cbar.load("art/closedbar.png");
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
	flag.load("art/flag.png");
	QMark.load("art/qmark.png");

	//Add background
	tgui::Picture::Ptr Bpicture = tgui::Picture::create("art/BackGround.png");
	Bpicture->setSize(tgui::bindMax(1024, windowWidth), tgui::bindMax(768, windowHeight));
	gui.add(Bpicture);

	// Add Menu Background
	tgui::Picture::Ptr M_picture = tgui::Picture::create("art/emptymenu.png");
	M_picture->setSize(windowWidth-250,windowHeight-150);
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
	else if (music.getStatus()==music.Playing)         c->check();
	c->connect("checked", [&]() { music.play(); });
	c->connect("unchecked", [&]() { music.stop(); });
	Menu_Widgets.insert(Menu_Widgets.end(), c);
	gui.add(c);

	// Add buttons 
	tgui::Button::Ptr M_buttons[4];
	string txts[4] = { "NewGame","ScoreBoard","Credits","Exit" };
	float v = 530;
	for (int i = 0; i < 4; i++)
	{
		M_buttons[i] = tgui::Button::create();
		M_buttons[i]->getRenderer()->setNormalTexture(Hbar);
		M_buttons[i]->getRenderer()->setHoverTexture(bar);
		M_buttons[i]->getRenderer()->setDownTexture(Hbar);
		M_buttons[i]->setFont("art/arcadeclassic.TTF");
		M_buttons[i]->getRenderer()->setTextColor("White");
		M_buttons[i]->setTextSize(48);
		M_buttons[i]->setSize(500, 100);
		M_buttons[i]->setPosition(windowWidth / 3.8, windowHeight  - v);
		v =v-120;
		M_buttons[i]->setText(txts[i]);
		M_buttons[i]->showWithEffect(tgui::ShowAnimationType::Fade, sf::milliseconds(800));
		Menu_Widgets.insert(Menu_Widgets.end(), M_buttons[i]);
		gui.add(M_buttons[i]);
		M_buttons[i]->connect("pressed", SoundPlay, "sound", "sounds/click.flac", false);
		M_buttons[i]->connect("pressed", DisableWidgets);
	}
	M_buttons[0]->connect("pressed", LevelMenu);
	M_buttons[2]->connect("pressed", Credits);
	M_buttons[3]->connect("pressed", [&]() { window->close(); });
	

}

void LevelMenu()
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
	std::string dif[5] = { "Easy","Medium","Hard","Brutal","Custom" };
	for (int i = 0; i<5; i++)
	{
		button[i] = tgui::Button::create();
		button[i]->getRenderer()->setNormalTexture(bar);
		button[i]->getRenderer()->setHoverTexture(Hbar);
		button[i]->getRenderer()->setDownTexture(Cbar);
		button[i]->setSize(windowWidth / 3, windowHeight / 6);
		button[i]->setPosition(windowWidth / 6, windowHeight - s);
		button[i]->setText(dif[level]);
		button[i]->setTextSize(60);
		button[i]->setFont("art/arcadeclassic.TTF");
		button[i]->showWithEffect(tgui::ShowAnimationType::SlideFromLeft, sf::milliseconds(800));
		gui.add(button[i]);
		s -= 150;
		Menu_Widgets.insert(Menu_Widgets.end(), button[i]);
		button[i]->connect("pressed", SoundPlay,"sound", "sounds/click.flac",false);
		button[i]->connect("pressed", DisableWidgets);
		if (i != 4) button[i]->connect("pressed", Level_Select, level + 1);

		if (i == 4)
		{
			button[i]->connect("pressed", CustomLevel);
		}

		level++;

	}
	level = 0;
	Back_Button();
}
void Level_Select(int level)
{
	//Function that specifies grid size based on Level selection

	switch (level)
	{
	case 1:
		//Easy_Level
		Launch_Level(8, 8, 10);
		break;
	case 2:
		// Medium_Level
		Launch_Level(10, 10, 20);
		break;

	case 3:
		//Hard_Level
		Launch_Level(15, 15, 45);
		break;

	case 4:
		// Brutal_Level
		Launch_Level(20, 20, 80);
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
		Launch_Level(width, height, numOfmines);
		break;
	}

}

void Launch_Level(int width, int height, int numOfmines)
{
	//Function that starts the Level and displays the grid tiles 

	initVisibleToHash(height, width);
	randomiseMineCoordinates(minesCoordinates, numOfmines, height, width);
	placeMines(minesCoordinates);
	setValuesForGrid(height, width);
	


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
			button[i][j]->connect("pressed", SoundPlay, "sound", "sounds/click.flac", false);
			button[i][j]->connect("pressed", Store, i, j, width, height, 'O');
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
	rc = true;
	Back_Button();
}

void CustomLevel()
{
	//Function that diplays and takes input for the custom Level

	//Add inputbox for width
	E_width = theme->load("EditBox");
	E_width->setSize(windowWidth / 5, windowHeight / 12);
	E_width->setPosition(windowWidth / 8, windowHeight / 6);
	E_width->setDefaultText("width");
	E_width->showWithEffect(tgui::ShowAnimationType::SlideFromLeft, sf::milliseconds(800));
	gui.add(E_width);
	Menu_Widgets.insert(Menu_Widgets.end(), E_width);
	// Add inputbox for height
	E_height = theme->load("EditBox");
	E_height->setSize(windowWidth / 5, windowHeight / 12);
	E_height->setPosition((windowWidth / 8) * 3, windowHeight / 6);
	E_height->setDefaultText("Height");
	E_height->showWithEffect(tgui::ShowAnimationType::SlideFromLeft, sf::milliseconds(800));
	gui.add(E_height);
	Menu_Widgets.insert(Menu_Widgets.end(), E_height);
	//Add inputbox for numOfmines 
	E_numOfmines = theme->load("EditBox");
	E_numOfmines->setSize(windowWidth / 5, windowHeight / 12);
	E_numOfmines->setPosition((windowWidth / 8) * 5, windowHeight / 6);
	E_numOfmines->setDefaultText("mines");
	E_numOfmines->showWithEffect(tgui::ShowAnimationType::SlideFromLeft, sf::milliseconds(800));
	gui.add(E_numOfmines);
	Menu_Widgets.insert(Menu_Widgets.end(), E_numOfmines);

	//Add enter button
	tgui::Button::Ptr b = tgui::Button::create();
	b->getRenderer()->setNormalTexture(bar);
	b->getRenderer()->setHoverTexture(Hbar);
	b->getRenderer()->setDownTexture(Cbar);
	b->setSize(windowWidth / 2, windowHeight / 6);
	b->setText("Enter");
	b->setTextSize(54);
	b->setFont("art/arcadeclassic.TTF");
	b->setPosition(windowWidth / 4.5, windowHeight / 6 + windowHeight / 6);
	Menu_Widgets.insert(Menu_Widgets.end(), b);
	b->connect("pressed", SoundPlay, "sound", "sounds/click.flac", false);
	b->connect("pressed", DisableWidgets);
	b->connect("pressed", Level_Select, 5);
	b->showWithEffect(tgui::ShowAnimationType::SlideFromLeft, sf::milliseconds(800));
	gui.add(b);
	Back_Button();

}

void DisableWidgets()
{
	// Function that disable the previous widgets to create a different menu

	for (int i = 0; i<Menu_Widgets.size(); i++)
	{
		Menu_Widgets[i]->hide();
		gui.remove(Menu_Widgets[i]);
	}
	Menu_Widgets.clear();

}

void Back_Button()
{
	//Add back button
	tgui::Button::Ptr back_Button = tgui::Button::create();
	back_Button->getRenderer()->setNormalTexture(bar);
	back_Button->getRenderer()->setHoverTexture(Hbar);
	back_Button->getRenderer()->setDownTexture(Cbar);
	back_Button->setSize(windowWidth / 6, windowHeight / 12);
	back_Button->setText("MainMenu");
	back_Button->setFont("art/arcadeclassic.TTF");
	back_Button->setPosition(windowWidth - windowWidth / 6, windowHeight - windowHeight / 12);
	Menu_Widgets.insert(Menu_Widgets.end(), back_Button);
	back_Button->connect("pressed", SoundPlay, "sound", "sounds/click.flac", false);
	back_Button->connect("pressed", DisableWidgets);
	back_Button->connect("pressed", MainMenu);
	back_Button->showWithEffect(tgui::ShowAnimationType::Fade, sf::milliseconds(800));
	gui.add(back_Button);
}


void Store(int i, int j, int width, int height, char operation)
{
	//Function that displays the grid to the tiles buttons

	clickTile(height, width, i, j, operation);
	getVisiable(my_visible);
	for (int k = 0; k < height; k++)
	{
		for (int l = 0; l < width; l++)
		{
			if (my_visible[k + 1][l + 1] != '#')
			{
				if (my_visible[k + 1][l + 1] != '@'&&my_visible[k + 1][l + 1] != '+'&&my_visible[k + 1][l + 1] != 'F')
				{
					button[k][l]->getRenderer()->setNormalTexture(tilenums[my_visible[k + 1][l + 1] - '0']);
					button[k][l]->disable();
				}
				else if (my_visible[k + 1][l + 1] == '@')
				{
					button[k][l]->getRenderer()->setNormalTexture(Emine);
					SoundPlay("sound", "sounds/explode.flac", false);
					FinishedLevel(width, height, false);
				}
				else if (my_visible[k + 1][l + 1] == '+')
				{
					button[k][l]->getRenderer()->setNormalTexture(mine);
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

void FinishedLevel(int width, int height, bool won)
{
	//Function that detects and displays Losing and wining 

	tgui::Label::Ptr txt;
	tgui::Button::Ptr again;

		// Add label text for win and lose
		txt = tgui::Label::create();
		txt->setSize(400, 400);
		txt->setTextSize(72);
		txt->setPosition(850, 100);
		txt->setFont("art/arcadeclassic.TTF");
		txt->setTextColor("White");

		if (won)
			txt->setText("You \n \n Win!");
		else
			txt->setText("You \n \n Lose!");

		Menu_Widgets.insert(Menu_Widgets.end(), txt);
		gui.add(txt);

		// Add PlayAgain Button 
		again = tgui::Button::create();
		again->getRenderer()->setNormalTexture(bar);
		again->getRenderer()->setHoverTexture(Hbar);
		again->getRenderer()->setDownTexture(Cbar);
		again->setSize(windowWidth / 6, windowHeight / 12);
		again->setText("PlayAgain!");
		again->setFont("art/arcadeclassic.TTF");
		again->setPosition(windowWidth - windowWidth / 6, windowHeight - windowHeight / 4);
		Menu_Widgets.insert(Menu_Widgets.end(), again);
		again->connect("pressed", SoundPlay, "sound", "sounds/click.flac", false);
		again->connect("pressed", DisableWidgets);
		again->connect("pressed", LevelMenu);
		gui.add(again);

		// Disable pressing for the remaining grid tiles
		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				button[i][j]->disable();
			}
		}
}

void Credits()
{
	// Add label text 
	tgui::Label::Ptr text;
	text = tgui::Label::create();
	text->setSize(windowWidth-200, windowHeight-200);
	text->setTextSize(48);
	text->setPosition(windowWidth/6, 10);
	text->setFont("art/arcadeclassic.TTF");
	text->setText("Backend  Development  \nAhmed Elmayyah\t!Satharus !  \nAhmed Aboamra\t!ahmedabuamra !  \n \n GUI   and   Art \n Andrew Awni\t!andrewawni ! \n AbdulRahman Yousry\t!slashdevo ! \n\n"	);

	Menu_Widgets.insert(Menu_Widgets.end(), text);
	Back_Button();
	gui.add(text);
}

