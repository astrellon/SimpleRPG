#include <iostream>
#include <time.h>
#include <math.h>
#include <vector>
#include <algorithm>
#ifdef _WIN32
#include <conio.h>
#endif

#include <boost\filesystem.hpp>

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

using namespace boost::filesystem;

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

WINDOW *gameWnd;
WINDOW *mainMenuWnd;

Game *startGame(string filename)
{
	Game *game = new Game();
	game->loadMap(filename);
	if(game->getMap() == NULL)
	{
		cerr << "Unable to load map!" << endl << "Exiting program, press enter." << endl;
		getchar();
		return NULL;
	}

	return game;
}

int main()
{
	ofstream logfile("logfile.log");
	cout.rdbuf(logfile.rdbuf());

	srand( (unsigned int)time(NULL) );

	Tile::registerDefaults();
	
	Game *game;

	initscr();
	curs_set(0);
	init_colours();

	mainMenuWnd = newwin(25, 80, 0, 0);
	gameWnd = newwin(25, 80, 0, 0);
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

	mainItem2.addChild(UISpacer());

	UISelector mainItem2FileList;
	mainItem2FileList.setSelectionIndex(0);
	mainItem2FileList.setMaxHeight(18);
	mainItem2.addChild(mainItem2FileList);
	
	mainMenu.addChild(title);
	mainMenu.addChild(UISpacer());
	mainMenu.addChild(mainItem1);
	mainMenu.addChild(mainItem2);

	path currentPath(initial_path<path>());
	currentPath = system_complete(path("."));

	bool loadFilelist = true;
	vector<folder_entry> folderEntries;

	UIText pausedText("<12>*PAUSED*</>");
	pausedText.setX(1);
	pausedText.setWindow(gameWnd);

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
					else if(c == '2')
					{
						quit = true;
					}
					break;
				case 1:
					if (c == 259)
					{
						mainItem2FileList.scrollSelection(-1);
					}
					else if (c == 258)
					{
						mainItem2FileList.scrollSelection(1);
					}
					else if (c == 10)
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
								stringstream ss;
								ss << "<12>" << filename << "</>";
								formatted = ss.str();
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

				if(c == 'p' || c == ' ')
					paused = !paused;

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

			if(!paused && !game->getCursorMode() && !game->getGamePaused())
			{
				game->update(0.04f);
			}

			wclear(gameWnd);
			game->render(gameWnd);

			if(paused || game->getCursorMode() || game->getGamePaused())
			{
				pausedText.render(false);
			}

			wrefresh(gameWnd);
			msleep(40);
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
