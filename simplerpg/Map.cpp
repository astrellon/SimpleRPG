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
			mMapData[x][y].position.x = (float)x;
			mMapData[x][y].position.y = (float)y;
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

inline bool Map::checkNeighbor(vector<AStarNode *> *nodes, const int &x, const int &y)
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


vector<AStarNode *> *Map::getNeighbors(Vector2f position)
{
	vector<AStarNode *> *nodes = new vector<AStarNode *>();

	int posX = (int)position.x;
	int posY = (int)position.y;

	bool left =		checkNeighbor(nodes, posX - 1,	posY);
	bool top =		checkNeighbor(nodes, posX,		posY - 1);
	bool right =	checkNeighbor(nodes, posX + 1,	posY);
	bool bottom =	checkNeighbor(nodes, posX,		posY + 1);

	if (left && top)
		checkNeighbor(nodes, posX - 1, posY - 1);
	
	if (top && right)
		checkNeighbor(nodes, posX + 1, posY - 1);

	if (bottom && left)
		checkNeighbor(nodes, posX - 1, posY + 1);

	if (bottom && right)
		checkNeighbor(nodes, posX + 1, posY + 1);

	return nodes;
}

double manhattanDistance(const Vector2f &p1, const Vector2f &p2)
{
	return abs(p1.x - p2.x) + abs(p1.y - p2.y);
}

vector<Vector2f> *Map::search(const Vector2i &start, const Vector2i &end)
{
	vector<AStarNode *> openList;

	if (start.x < 0 || start.x >= mWidth ||
		start.y < 0 || start.y >= mHeight)
	{
		return NULL;
	}

	for(int x = 0; x < mWidth; x++)
	{
		for(int y = 0; y < mHeight; y++)
		{
			mMapData[x][y].reset();
		}
	}

	openList.push_back(&mMapData[start.x][start.y]);

	AStarNode *endNode = &mMapData[end.x][end.y];
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
	return NULL;
}

vector<Vector2f> *Map::getPath(AStarNode *node)
{
	vector<Vector2f> *path = new vector<Vector2f>();

	while(node != NULL)
	{
		//path->insert(path->begin(), node->position);
		path->push_back(node->position);
		node = node->parent;
	}

	reverse(path->begin(), path->end());

	return path;
}

void Map::renderMap(Rect &rect, WINDOW *wnd)
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