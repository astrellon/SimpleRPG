#include "main.h"

bool sortFolders(folder_entry i, folder_entry j)
{
	if(i.isDir && !j.isDir)
		return true;
	if(!i.isDir && j.isDir)
		return false;

	return strcmp(i.filename.c_str(), j.filename.c_str()) < 0;
}

void init_colours()
{
	start_color();
	for(int i = 0; i < 8; i++)
	{
		init_pair(i + 1, i, COLOR_BLACK);
	}
	init_pair(9, 0, 8);
}

WINDOW *gameWnd = NULL;
WINDOW *mainMenuWnd = NULL;

int windowWidth = 80;
int windowHeight = 25;

double freq;
LARGE_INTEGER updateTime, frequency;

void loadConfig(string filename)
{
	FormattedFileIterator configIter(filename, true);
	if(!configIter.isOpen())
	{
		return;
	}

	while(!configIter.atEnd())
	{
		string line = *configIter; ++configIter;
		if(iequals(line, "size"))
		{
			windowWidth  = lexical_cast<int>(*configIter); ++configIter;
			windowHeight = lexical_cast<int>(*configIter); ++configIter;
		}
		else
		{
			clog << "Unknown config property '" << line << '\'' << endl;
		}
	}
}

Game *startGame(string filename)
{
	Game *game = new Game(windowWidth, windowHeight);
	game->loadMap(filename);
	if(game->getMap() == NULL)
	{
		cerr << "Unable to load map!" << endl << "Exiting program, press enter." << endl;
		getchar();
		return NULL;
	}

	// Start game timer.
	QueryPerformanceCounter(&updateTime);

	return game;
}

void distTest()
{
	ofstream file("distTest.txt");

	int n = 10000;
	for(int i = 0; i < n; i++)
	{
		file << math::nextDist(-5, 2.5, 0) << endl;
	}

	file.close();
}

void threatTest(UIText &threatTester)
{
	threatTester.clearText();

	Animal wolf(NULL);
	Animal bear(NULL);
	Animal rabbit(NULL);
	Animal horse(NULL);
	Animal mouse(NULL);

	wolf.setSpecies("Wolf");
	wolf.setStrength(16.0f);
	wolf.setSize(1.2f);
	wolf.setDiet(0.8f);

	bear.setSpecies("Bear");
	bear.setStrength(22.0f);
	bear.setSize(3.0f);
	bear.setDiet(0.85f);

	rabbit.setSpecies("Rabbit");
	rabbit.setStrength(4.0f);
	rabbit.setSize(0.3f);
	rabbit.setDiet(0.1f);

	horse.setSpecies("Horse");
	horse.setStrength(16.0f);
	horse.setSize(3.0f);
	horse.setDiet(0.05f);

	mouse.setSpecies("Mouse");
	mouse.setStrength(2.0f);
	mouse.setSize(0.1f);
	mouse.setDiet(0.2f);

	vector<Animal *> animals;
	animals.push_back(&wolf);
	animals.push_back(&bear);
	animals.push_back(&rabbit);
	animals.push_back(&horse);
	animals.push_back(&mouse);
	
	for(int i = 0; i < (int)animals.size(); i++)
	{
		Animal *self = animals[i];
		threatTester << "\n<15>Threat levels for " << self->getSpecies() << "</>\n";
		for(int j = 0; j < (int)animals.size(); j++)
		{
			Animal *other = animals[j];
			float threat = self->getEntityThreat(other);
			threatTester << "Threat from " << other->getSpecies() << "\t<12>" << threat << "</>\n";
		}
	}
}

void resizeScreen(int width, int height)
{
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	SMALL_RECT displayArea = {0, 0, width - 1, height - 1};
	COORD si = {width, height};

	SetConsoleScreenBufferSize(hOut, si);
	SetConsoleWindowInfo(hOut, true, &displayArea);

	windowWidth = width;
	windowHeight = height;

	clog << "Setting window size to " << width << ", " << height << endl;
}

int main(int argc, char **argv)
{
	ofstream logfile("logfile.log");
	clog.rdbuf(logfile.rdbuf());

	loadConfig("config.ini");

	// If set from the command line, that file will be used to directly load
	// a map.
	string loadFile = "";

	if(argc >= 2)
	{
		int i = 1;
		while(i < argc)
		{
			string cmd = argv[i];
			if(cmd[0] == '-')
			{
				i++;
				if(iequals(cmd, "-s") || iequals(cmd, "-size"))
				{
					if(i + 1 >= argc)
					{
						clog << "Not enough arguments for size command." << endl;
						break;
					}

					windowWidth = lexical_cast<int>(argv[i]);
					windowHeight = lexical_cast<int>(argv[i + 1]);
					i += 2;
				}
			}
			else
			{
				loadFile = argv[i];
			}
		}
	}

	resizeScreen(windowWidth, windowHeight);
	
	srand( (unsigned int)time(NULL) );

	Tile::registerDefaults();

	// Setup performance checking, used to determine time difference
	// between physics thread runs.
	QueryPerformanceFrequency( &frequency );
	freq = (double)frequency.QuadPart / 1000.0;
	
	Game *game;

	// Initialise the curses library.
	initscr();
	// Disables the cursor.
	curs_set(0);
	// Initialises the colour pairs that we intend to use.
	// Currently it is all colours with a black background.
	init_colours();

	mainMenuWnd = newwin(windowHeight, windowWidth, 0, 0);
	gameWnd = newwin(windowHeight, windowWidth, 0, 0);

	// Window size check, as if the window sizes are too big for the current screen or are invalid
	// in some other way newwin returns NULL. This will then attempt to use default screen sizes.
	// Should that also fail the program exits. This should hopefully never happen but it might.
	if(mainMenuWnd == NULL || gameWnd == NULL)
	{
		clog << "A window size of (" << windowWidth << ", " << windowHeight << ") is either too large or invalid." << endl;
		clog << "Defaulting to (80, 25)" << endl;
		resizeScreen(80, 25);

		mainMenuWnd = newwin(windowHeight, windowWidth, 0, 0);
		gameWnd = newwin(windowHeight, windowWidth, 0, 0);

		if(mainMenuWnd == NULL || gameWnd == NULL)
		{
			clog << "Default window sizes have also failed, quitting program :(" << endl;
			cout << "Unable to create a curses window, quitting program." << endl;
			logfile.close();
			return -1;
		}
	}

	// User input is immediately available to program.
	cbreak();
	// User input is not echoed.
	noecho();

	// Enables use of special characters as input such as arrow keys.
	keypad(gameWnd, true);
	keypad(mainMenuWnd, true);

	bool paused = false;
	bool inMainMenu = true;
	//int menuLevel = 0;

	// Main menu is setup to be a list and currently each different
	// menu screen is a different item that is disabled when not
	// in use. This should allow for easily changing between menu levels
	// and when disabled should take up minimal processing to skip.
	UIMenu mainMenu;
	mainMenu.setWindow(mainMenuWnd);
	mainMenu.setX(1);
	mainMenu.setY(1);

	mainMenu.addChild(new UIText("<15>Alan Lawrey's Thesis project 2011</>\n"));

	mainMenu.setMenuItem(0, new UIText("<12>1</>: Load world.\n<12>2</>: Output rejection samplying test.\n<12>3</>: Threat tester.\n<12>q</>: Quit."));

	UIList mainItem2;
	UIText mainItem2Title;
	mainItem2Title << "<15>Select a world to load.</>";
	mainItem2.addChild(mainItem2Title);

	UIText currentPathText;
	currentPathText << "Error";
	mainItem2.addChild(currentPathText);

	UISelector mainItem2FileList;
	mainItem2FileList.setSelectionIndex(0);
	mainItem2FileList.setMaxHeight(windowHeight - 7);
	mainItem2.addChild(mainItem2FileList);

	mainMenu.setMenuItem(1, mainItem2);

	mainMenu.setMenuItem(2, new UIText("<12>1</>: Really quit?\n<12>Any</>: Return to main."));
	mainMenu.setMenuItem(3, new UIText("<12>1</>: Write out distrubution test file.\n<12>Any</>: Go back."));
	
	UIList mainItem5;
	mainItem5.addChild(new UIText("<12>1</>: Test threat levels.\n<12>Any</>: Go back."));
	UIText threatTester;
	mainItem5.addChild(threatTester);
	mainMenu.setMenuItem(4, mainItem5);

	path currentPath(initial_path<path>());
	currentPath = system_complete(path("."));

	bool loadFilelist = true;
	// Stores an array of the entries in the current folder being viewed.
	vector<folder_entry> folderEntries;

	UIText pausedText("<12>*PAUSED*</>");
	pausedText.setX(1);
	pausedText.setWindow(gameWnd);

	if(loadFile.size() > 1)
	{
		inMainMenu = false;
		game = startGame(loadFile);
	}

	while(true)
	{
		if(inMainMenu)
		{
			bool quit = false;
			while(_kbhit())
			{
				int c = wgetch(mainMenuWnd);

				switch(mainMenu.getMenuLevel())
				{
				default:
				case 0:
					if(c == '1')
					{
						mainMenu.setMenuLevel(1);
					}
					else if(c == 'q')
					{
						mainMenu.setMenuLevel(2);
					}
					else if(c == '2')
					{
						mainMenu.setMenuLevel(3);
					}
					else if(c == '3')
					{
						mainMenu.setMenuLevel(4);
					}
					else if
					(c == 27)
					{
						quit = true;
					}
					break;
				case 1:
					if (c == 27)
					{
						mainMenu.goBack();
						break;
					}
					if (c == 259 || c == 'w' || c == '1')
					{
						mainItem2FileList.scrollSelection(-1);
					}
					else if (c == 258 || c == 's' || c == '2')
					{
						mainItem2FileList.scrollSelection(1);
					}
					// Enter or numpad enter.
					else if (c == 10 || c == 459)
					{
						folder_entry *entry = &folderEntries[mainItem2FileList.getSelectionIndex()];
						if(entry->index == 0)
						{
							string pathString = currentPath.directory_string();
							int firstIndex = pathString.find('\\');
							int lastIndex = pathString.find_last_of('\\');
							if(firstIndex < (int)pathString.size() - 1 )
							{
								if(lastIndex == firstIndex)
								{
									lastIndex++;
								}
								string sub = pathString.substr(0, lastIndex);
								currentPath = sub;
								loadFilelist = true;
							}
						}
						else
						{
							currentPath /= entry->filename;
							if(is_directory(currentPath))
							{
								loadFilelist = true;
							}
							else
							{
								inMainMenu = false;
								string pp = currentPath.file_string();
								game = startGame(currentPath.filename());
							}
						}
					}

					break;
				case 2:
					if(c == '1' || c == 'q')
					{
						quit = true;
					}
					else
					{
						mainMenu.goBack();
					}
					break;
				case 3:
					if(c == '1')
					{
						distTest();
					}
					else
					{
						mainMenu.goBack();
					}
					break;
				case 4:
					if(c == '1')
					{
						threatTest(threatTester);
					}
					else
					{
						mainMenu.goBack();
					}
				}
			}

			wclear(mainMenuWnd);

			if(!inMainMenu)
			{
				wrefresh(mainMenuWnd);
				continue;
			}
			if(quit)
				break;

			switch(mainMenu.getMenuLevel())
			{
			default:
				break;
			case 1:
				if(loadFilelist)
				{
					loadFilelist = false;
					
					directory_iterator end_iter;

					currentPathText.clearText();
					currentPathText << "<15>Current path:</> " << currentPath.external_directory_string() << '\n';

					folderEntries.clear();
					folderEntries.push_back(folder_entry(0, true, "..", ".."));

					int i = 1;
					for(directory_iterator dir_iter(currentPath); dir_iter != end_iter; ++dir_iter)
					{
						string ext = extension(*dir_iter);					
						if(is_directory(*dir_iter) || iequals(ext, ".txt") || iequals(ext, ".map"))
						{
							string filename = (*dir_iter).filename();

							string formatted = filename;
							bool dir = false;
							if(is_directory(*dir_iter))
							{
								dir = true;
								formatted = "<12>" + filename + "</>";
							}

							folderEntries.push_back(folder_entry(i++, dir, filename, formatted));
						}
					}
					
					mainItem2FileList.removeAllChildren(true);
					mainItem2FileList.setSelectionIndex(0);

					sort(folderEntries.begin() + 1, folderEntries.end(), sortFolders);

					for(vector<folder_entry>::iterator iter = folderEntries.begin(); iter != folderEntries.end(); iter++)
					{
						UIText *entry = new UIText();
						*entry << (*iter).formattedName;
						mainItem2FileList.addChild(*entry);
					}
				}
				break;

			}

			mainMenu.render();

			wrefresh(mainMenuWnd);

			msleep(40);
		}
		else if(game == NULL)
		{
			break;
		}
		else
		{
			while(_kbhit())
			{
				int c = wgetch(gameWnd);

				if(c == 's')
				{
					game->saveMap("test.out");
				}
				game->keyActions(c);
			}

			if(!game->getGameRunning())
			{
				break;
			}

			LARGE_INTEGER currentTime;
			QueryPerformanceCounter(&currentTime);
			// Get the time since this thread last executed and convert into
			// seconds (from milliseconds).
			float dt = (float)(((double)currentTime.QuadPart - (double)updateTime.QuadPart) / freq);
			updateTime = currentTime;
			dt *= 0.001f;

			if(!paused && !game->getGamePaused())
			{
				game->update(dt);
			}

			wclear(gameWnd);
			game->render(gameWnd);

			if(paused || game->getGamePaused())
			{
				pausedText.render(false);
			}

			QueryPerformanceCounter(&currentTime);
			dt = (float)(((double)currentTime.QuadPart - (double)updateTime.QuadPart) / freq);
			updateTime = currentTime;

			float sleepTime = 40.0f - dt;
			if (sleepTime < 10.0f)
				sleepTime = 10.0f;

			wrefresh(gameWnd);
			msleep((int)round(sleepTime));
		}
	}

	delwin(gameWnd);
	delwin(mainMenuWnd);
	endwin();
	
	if (logfile.is_open())
	{
		logfile.close();
	}
	
	cout << "Thanks for playing!" << endl;

	return 0;
}
