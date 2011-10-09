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

	game->loadNameList("names.txt");

	//vector<Vector2f> *path = game->getMap()->search(Vector2i(97, 36), Vector2i(98, 35));

	// Start game timer.
	QueryPerformanceCounter(&updateTime);

	return game;
}

void distTest(UIText &text)
{
	string filename = "distTest.txt";
	ofstream file(filename);

	if(!file.is_open())
	{
		text << "Unable to open normal distribution testing log file '<12>" << filename << "'</>\n";
		return;
	}

	int n = 10000;
	for(int i = 0; i < n; i++)
	{
		file << math::nextDist(-5, 2.5, 0) << endl;
	}

	file.close();

	text << "\nOutputted <12>" << n << "</> normal distribution tests to '<12>" << filename << "</>'\n";
}

void rouletteTest(UIText &text)
{
	string filename = "rouletteTest.txt";
	ofstream file(filename);

	if(!file.is_open())
	{
		text << "Unable to open roulette distribution testing log file '<12>" << filename << "'</>\n";
		return;
	}

	vector<float> probs;
	probs.push_back(2.0f);
	probs.push_back(1.1f);
	probs.push_back(1.1f);
	probs.push_back(3.5f);
	probs.push_back(2.7f);
	probs.push_back(4.3f);
	probs.push_back(5.1f);
	probs.push_back(1.5f);
	probs.push_back(1.8f);
	probs.push_back(4.0f);

	int n = 10000;
	for(int i = 0; i < n; i++)
	{
		file << math::nextRoulette(probs) << endl;
	}

	file.close();

	text << "\nOutputted <12>" << n << "</> roulette distribution tests to '<12>" << filename << "</>'\n";
}

void breedTest(UIText &breedTester)
{
	Animal parent1(NULL);
	parent1.setDiet(0.5f);
	parent1.setStrength(4);
	parent1.setDexterity(18);
	parent1.setIntelligence(6);
	parent1.setAggression(0.1f);
	parent1.setAttackRate(1.2f);
	parent1.setSpecies("Rabbit");
	
	Animal parent2(NULL);
	parent2.setDiet(0.6f);
	parent2.setStrength(3.5);
	parent2.setDexterity(19);
	parent2.setIntelligence(6.5);
	parent2.setAggression(0.08f);
	parent2.setAttackRate(1.5f);
	parent2.setSpecies("Rabbit");

	string filename = "breedTest.txt";

	FormattedFile file(filename);
	if(!file.isOpen())
	{
		breedTester << "Unable to open breed testing log file '<12>" << filename << "'</>\n";
		return;
	}

	file << "// Breed tester file\n";
	file << "// Parent 1\n";
	parent1.saveToFile(file);
	file << "// Parent 2\n";
	parent2.saveToFile(file);

	breedTester << "\nWorking...\n";

	int n = 100;

	int k = 0;
	for(int i = 0; i < n; i++)
	{
		vector<Animal *> children;
		Animal::breed(children, &parent1, &parent2);

		for(vector<Animal *>::iterator iter = children.begin(); iter != children.end(); ++iter)
		{
			file << "// Child " << k++ << '\n';
			(*iter)->saveToFile(file);
			delete *iter;
		}
	}

	file.closeFile();

	breedTester << "\nOutputted <12>" << n << "</> breed tests to '<12>" << filename << "</>'\n";
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

	// Main menu is setup to be a list and currently each different
	// menu screen is a different item that is disabled when not
	// in use. This should allow for easily changing between menu levels
	// and when disabled should take up minimal processing to skip.
	UIMenu mainMenu;
	mainMenu.setWindow(mainMenuWnd);
	mainMenu.setX(1);
	mainMenu.setY(1);

	mainMenu.addChild(new UIText("<15>Alan Lawrey's Thesis project 2011</>\n"));

	mainMenu.setMenuItem(0, new UIText("<12>1</>: Load world.\n"
		"<12>2</>: Output rejection samplying test.\n"
		"<12>3</>: Threat tester.\n"
		"<12>4</>: Breed tester.\n"
		"<12>q</>: Quit."));

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

	UIText randomTester("<12>1</>: Write normal distrubution test file.\n"
		"<12>2</>: Write roulette distrubution test file.\n"
		"<12>Any</>: Go back.");
	mainMenu.setMenuItem(3, randomTester);
	
	UIList mainItem5;
	mainItem5.addChild(new UIText("<12>1</>: Test threat levels.\n<12>Any</>: Go back."));
	UIText threatTester;
	mainItem5.addChild(threatTester);
	mainMenu.setMenuItem(4, mainItem5);

	UIText breedTester("<12>1</>: Test breeding.\n<12>Any</>: Go back.");
	mainMenu.setMenuItem(5, breedTester);

	path currentPath(initial_path<path>());
	currentPath = system_complete(path("."));

	bool loadFilelist = true;
	// Stores an array of the entries in the current folder being viewed.
	vector<folder_entry> folderEntries;

	UIText pausedText("<12>*PAUSED*</>");
	pausedText.setX(1);
	pausedText.setWindow(gameWnd);

	bool skipRenders = false;
	int skipCounter = 0;

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
					else if(c == '4')
					{
						mainMenu.setMenuLevel(5);
					}
					else if(c == 27)
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
					else if(c >= 'a' && c <= 'z')
					{
						mainItem2FileList.selectNextItem(c);
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
						distTest(randomTester);
					}
					else if(c == '2')
					{
						rouletteTest(randomTester);
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
					break;
				case 5:
					if(c == '1')
					{
						breedTest(breedTester);
					}
					else
					{
						mainMenu.goBack();
					}
					break;
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
						if(is_directory(*dir_iter) || iequals(ext, ".txt") || iequals(ext, ".out") || iequals(ext, ".map"))
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

					for(vector<folder_entry>::iterator iter = folderEntries.begin(); iter != folderEntries.end(); ++iter)
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
			bool error = false;
			try
			{
				if(!paused && !game->getGamePaused())
				{
					if(game->getTimeScale() > 1)
					{
						skipRenders = true;
						for(int i = 0; i < game->getTimeScale(); i++)
						{
							game->update(0.04f);
						}
					}
					else
					{
						skipRenders = false;
						game->update(dt);
					}
				}
			}
			catch(...)
			{
				error = true;
				clog << "There was an exception running the game." << endl;
				game->saveMap("exception.map");
				break;
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

			skipRenders = false;

			if(!skipRenders)
			{
				wrefresh(gameWnd);
				msleep((int)round(sleepTime));
			}
			else
			{
				skipCounter--;
				if (skipCounter < 0)
				{
					wrefresh(gameWnd);
					skipCounter = 4;
				}
			}
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
