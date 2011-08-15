#include <iostream>
#include <time.h>
#include <math.h>
#include <vector>
#include <algorithm>
#ifdef _WIN32
#include <conio.h>
#endif

#include <boost/filesystem.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/lexical_cast.hpp>

#include "Map.h"
#include "Tile.h"
#include "Game.h"
#include "Animal.h"
#include "Vector2.h"
#include "Matrix3x3.h"
#include "keyboard.h"
#include "UIText.h"
#include "UIContainer.h"
#include "UIScoller.h"
#include "UIList.h"
#include "UISpacer.h"
#include "UISelector.h"
#include "Destination.h"
#include "FormattedFile.h"
#include "FormattedFileIterator.h"
#include "Debug.h"

using namespace boost::filesystem;
using boost::lexical_cast;
using boost::algorithm::iequals;

#ifndef M_PI
#define M_PI 3.1415926535897932384626433832795
#endif

extern "C"
{
	#include "curses.h"
}

#ifdef _WIN32
#include <Windows.h>
inline void msleep(int ms)
{
	Sleep(ms);
}
#else
#include <unistd.h>
inline void msleep(int ms)
{
	usleep(ms * 1000);
}
#endif

using namespace std;

typedef struct _folder_entry
{
	int index;
	bool isDir;
	string filename;
	string formattedName;

	_folder_entry(int i, bool d, string f, string ff)
	{
		index = i;
		isDir = d;
		filename = f;
		formattedName = ff;
	}
} folder_entry;

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

int windowWidth = 100;
int windowHeight = 40;

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

	initscr();
	curs_set(0);
	init_colours();

	mainMenuWnd = newwin(windowHeight, windowWidth, 0, 0);
	gameWnd = newwin(windowHeight, windowWidth, 0, 0);

	cbreak();
	noecho();
	keypad(gameWnd, true);
	keypad(mainMenuWnd, true);

	bool paused = false;
	bool inMainMenu = true;
	int menuLevel = 0;

	UIList mainMenu;
	mainMenu.setWindow(mainMenuWnd);

	mainMenu.setX(1);
	mainMenu.setY(1);

	UIText title;
	title << "<15>Alan Lawrey's Thesis project 2011</>";

	UIText mainItem1;
	mainItem1 << "<12>1</>: Load world.\n<12>2</>: Quit.";
	
	UIList mainItem2;
	mainItem2.setVisible(false);

	UIText mainItem2Title;
	mainItem2Title << "<15>Select a world to load.</>";
	mainItem2.addChild(mainItem2Title);

	UIText currentPathText;
	currentPathText << "Error";
	mainItem2.addChild(currentPathText);

	mainItem2.addChild(new UISpacer());

	UISelector mainItem2FileList;
	mainItem2FileList.setSelectionIndex(0);
	mainItem2FileList.setMaxHeight(windowHeight - 7);
	mainItem2.addChild(mainItem2FileList);
	
	mainMenu.addChild(title);
	mainMenu.addChild(new UISpacer());
	mainMenu.addChild(mainItem1);
	mainMenu.addChild(mainItem2);

	path currentPath(initial_path<path>());
	currentPath = system_complete(path("."));

	bool loadFilelist = true;
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

				switch(menuLevel)
				{
				default:
				case 0:
					if(c == '1')
					{
						menuLevel = 1;
					}
					else if(c == '2' || c == 'q')
					{
						quit = true;
					}
					break;
				case 1:
					if (c == 27)
					{
						menuLevel = 0;
						break;
					}
					if (c == 259)
					{
						mainItem2FileList.scrollSelection(-1);
					}
					else if (c == 258)
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

			switch(menuLevel)
			{
			default:
			case 0:
				mainItem1.setVisible(true);
				mainItem2.setVisible(false);
				break;
			case 1:
				mainItem1.setVisible(false);
				mainItem2.setVisible(true);

				if(loadFilelist)
				{
					loadFilelist = false;
					
					directory_iterator end_iter;

					currentPathText.clearText();
					currentPathText << "<15>Current path:</> " << currentPath.external_directory_string();

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
