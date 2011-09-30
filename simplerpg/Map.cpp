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

	mNodeUseCounter = 0;
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
	for(vector<AStarNode *>::iterator iter = list->begin(); iter != list->end(); ++iter)
	{
		if((*iter) == node)
		{
			found = true;
			break;
		}
	}
	return found;
}

inline bool Map::checkNeighbor(const int &x, const int &y)
{
	if(x >= 0 && x < mWidth && y >= 0 && y < mHeight)
	{
		AStarNode *node = &mMapData[x][y];
		if(node->tile->getPassable())
		{
			mNeighbors.push_back(node);
			return true;
		}
	}
	return false;
}

void Map::getNeighbors(Vector2f position)
{
	int posX = (int)position.x;
	int posY = (int)position.y;

	bool left =		checkNeighbor(posX - 1,	posY);
	bool top =		checkNeighbor(posX,		posY - 1);
	bool right =	checkNeighbor(posX + 1,	posY);
	bool bottom =	checkNeighbor(posX,		posY + 1);

	if (left && top)
		checkNeighbor(posX - 1, posY - 1);
	
	if (top && right)
		checkNeighbor(posX + 1, posY - 1);

	if (bottom && left)
		checkNeighbor(posX - 1, posY + 1);

	if (bottom && right)
		checkNeighbor(posX + 1, posY + 1);
}

double manhattanDistance(const Vector2f &p1, const Vector2f &p2)
{
	return abs(p1.x - p2.x) + abs(p1.y - p2.y);
}

vector<Vector2f> *Map::search(const Vector2i &start, const Vector2i &end)
{
	if (start.x < 0 || start.x >= mWidth ||
		start.y < 0 || start.y >= mHeight)
	{
		return NULL;
	}

	mOpenList.clear();
	mClosedList.clear();

	mNodeUseCounter++;

	mOpenList.push_back(&mMapData[start.x][start.y]);

	AStarNode *endNode = &mMapData[end.x][end.y];
	endNode->parent = NULL;

	while(!mOpenList.empty())
	{
		AStarNode *node = mOpenList.front();
		if(node->useCounter < mNodeUseCounter)
		{
			node->g = 0;
			node->useCounter = mNodeUseCounter;
			node->parent = NULL;
		}
		
		if(node == endNode)
		{
			// Complete
			return getPath(node);
		}
		else
		{
			mOpenList.erase(mOpenList.begin());
			mClosedList.push_back(node);

			getNeighbors(node->position);
			for(vector<AStarNode *>::iterator iter = mNeighbors.begin(); iter != mNeighbors.end(); ++iter)
			{
				AStarNode *n = *iter;
				if (!listContains(&mOpenList, n) &&
					!listContains(&mClosedList, n))
				{
					if(n->useCounter < mNodeUseCounter)
					{
						n->g = 0;
						n->useCounter = mNodeUseCounter;
					}
					n->g += node->g;

					n->f = n->g + manhattanDistance(n->position, endNode->position);

					n->parent = node;
					mOpenList.push_back(n);
				}
			}

			mNeighbors.clear();
			sort(mOpenList.begin(), mOpenList.end(), compare);
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

	int offX = rect.getX();
	int offY = rect.getY();

	for(int x = xstart; x < xfinish; x++)
	{
		int xx = x - offX;
		for(int y = ystart; y < yfinish; y++)
		{
			mMapData[x][y].tile->pixel.render(wnd, xx, y - offY);
		}
	}
}