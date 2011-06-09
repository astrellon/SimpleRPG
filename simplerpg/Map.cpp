#include "Map.h"


Map::Map(int width, int height)
{
	mWidth = width;
	mHeight = height;

	mMapData = new AStarNode*[width];
	for(int x = 0; x < width; x++)
	{
		mMapData[x] = new AStarNode[height];
		for(int y = 0; y < height; y++)
		{
			int tile = 2;
			if(y == 0)
				tile = 1;
			mMapData[x][y].tile = Tile::getTile(tile);
			mMapData[x][y].position.x = x;
			mMapData[x][y].position.y = y;
		}
	}
}

Map::~Map(void)
{
	for(int x = 0; x < mWidth; x++)
		delete mMapData[x];

	delete mMapData;
}

bool compare(AStarNode *n1, AStarNode *n2)
{
	return n1->f < n2->f;
}

bool listContains(vector<AStarNode *> *list, AStarNode *node)
{
	bool found = false;
	for(vector<AStarNode *>::iterator iter = list->begin(); iter != list->end(); iter++)
	{
		if((*iter) == node)
		{
			found = true;
			break;
		}
	}
	return found;
}

inline bool Map::checkNeighbor(vector<AStarNode *> *nodes, int x, int y)
{
	if(x >= 0 && x < mWidth && y >= 0 && y < mHeight)
	{
		AStarNode *node = &mMapData[x][y];
		if(node->tile->getPassable())
		{
			nodes->push_back(node);
			return true;
		}
	}
	return false;
}


vector<AStarNode *> *Map::getNeighbors(Vector2 position)
{
	vector<AStarNode *> *nodes = new vector<AStarNode *>();

	int posX = (int)position.x;
	int posY = (int)position.y;

	bool left = checkNeighbor(nodes, posX - 1, posY);
	bool top = checkNeighbor(nodes, posX, posY - 1);
	bool right = checkNeighbor(nodes, posX + 1, posY);
	bool bottom = checkNeighbor(nodes, posX, posY + 1);

	if (left && top)
		checkNeighbor(nodes, posX - 1, posY - 1);
	
	if (top && right)
		checkNeighbor(nodes, posX + 1, posY - 1);

	if (bottom && left)
		checkNeighbor(nodes, posX - 1, posY + 1);

	if (bottom && right)
		checkNeighbor(nodes, posX + 1, posY + 1);

	/*for(int x = -1; x <= 1; x++)
	{
		int xx = (int)position.x + x;
		for(int y = -1; y <= 1; y++)
		{
			if(x == 0 && y == 0)
				continue;

			int yy = (int)position.y + y;
			if(xx >= 0 && xx < mWidth && yy >= 0 && yy < mHeight)
			{
				AStarNode *node = &mMapData[xx][yy];
				if(node->tile->getPassable())
				{
					nodes->push_back(node);
				}
			}
		}
	}*/

	return nodes;
}

double manhattanDistance(Vector2 p1, Vector2 p2)
{
	return abs(p1.x - p2.x) + abs(p1.y - p2.y);
}

vector<Vector2> *Map::search(Vector2 start, Vector2 end)
{
	vector<AStarNode *> openList;

	if (start.x < 0 || start.x >= mWidth ||
		start.y < 0 || start.y >= mHeight)
	{
		return new vector<Vector2>();
	}

	for(int x = 0; x < mWidth; x++)
	{
		for(int y = 0; y < mHeight; y++)
		{
			mMapData[x][y].reset();
		}
	}

	openList.push_back(&mMapData[(int)start.x][(int)start.y]);

	AStarNode *endNode = &mMapData[(int)end.x][(int)end.y];
	endNode->parent = NULL;

	vector<AStarNode *> closedList;

	while(!openList.empty())
	{
		AStarNode *node = openList.front();
		
		if(node == endNode)
		{
			// Complete
			return getPath(node);
		}
		else
		{
			openList.erase(openList.begin());
			closedList.push_back(node);

			vector<AStarNode *> *neighbors = getNeighbors(node->position);
			for(vector<AStarNode *>::iterator iter = neighbors->begin(); iter != neighbors->end(); iter++)
			{
				AStarNode *n = *iter;
				if (!listContains(&openList, n) &&
					!listContains(&closedList, n))
				{
					n->g += node->g;

					n->f = n->g + manhattanDistance(n->position, endNode->position);

					n->parent = node;
					openList.push_back(n);
				}
			}

			delete neighbors;
			sort(openList.begin(), openList.end(), compare);
		}
	}
	// NO PATH! D:
	return new vector<Vector2>();
}

vector<Vector2> *Map::getPath(AStarNode *node)
{
	vector<Vector2> *path = new vector<Vector2>();

	while(node != NULL)
	{
		path->insert(path->begin(), node->position);
		node = node->parent;
	}

	return path;
}

void Map::renderMap(Rect rect, WINDOW *wnd)
{
	int xstart = 0;
	if (rect.getX() > 0)
		xstart = rect.getX();

	if (xstart > mWidth)
		xstart = mWidth - 1;

	int xfinish = rect.getWidth() + rect.getX();
	if (xfinish > mWidth)
		xfinish = mWidth;

	int ystart = 0;
	if (rect.getY() > 0)
		ystart = rect.getY();

	if (ystart > mHeight)
		ystart = mHeight - 1;

	int yfinish = rect.getHeight() + rect.getY();
	if (yfinish > mHeight)
		yfinish = mHeight;

	if (xstart - rect.getX() > rect.getWidth() ||
		xstart - rect.getX() < 0 ||
		ystart - rect.getY() > rect.getHeight() ||
		ystart - rect.getY() < 0)
		return;

	for(int x = xstart; x < xfinish; x++)
	{
		for(int y = ystart; y < yfinish; y++)
		{
			getTile(x, y)->pixel.render(wnd, x - rect.getX(), y - rect.getY());
		}
	}
}