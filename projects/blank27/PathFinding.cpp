#include "PathFinding.h"
#include "Node.h"

PathFinding::PathFinding()
{
	initStartDirection = false;
	foundDirection = false;
}


PathFinding::~PathFinding()
{

}

void PathFinding::FindPath(kf::Vector2 currentPosition, kf::Vector2 targetPosition)
{
	if (initStartDirection == false)
	{
		//Looping through each list
		for (int index = 0; index < openList.size(); index++)
		{
			delete openList[index];
		}
		openList.clear();

		for (int index = 0; index < visitedList.size(); index++)
		{
			delete visitedList[index];
		}
		visitedList.clear();

		for (int index = 0; index < pathToDirection.size(); index++)
		{
			delete pathToDirection[index];
		}
		pathToDirection.clear();

		//Initialise start
		Node start;
		start.xCoord = currentPosition.x;
		start.yCoord = currentPosition.y;

		//Initialize direction
		//targetPosition.set(15, 15);

		Node direction;
		direction.xCoord = targetPosition.x;
		direction.yCoord = targetPosition.y;

		StartandDirection(start, direction);
		initStartDirection = true;
	}

	if (initStartDirection == true)
	{
		ContinuePath();
	}
}

void PathFinding::StartandDirection(Node start, Node direction)
{
	startNode = new Node(start.xCoord, start.yCoord, NULL);
	directionNode = new Node(direction.xCoord, direction.yCoord, &direction);

	startNode->g = 0;
	startNode->h = startNode->ManhattanDistance(directionNode);
	startNode->parent = 0;

	openList.push_back(startNode);
}

void PathFinding::PathOpened(int x, int y, float newC, Node *parent)
{
	/*if (tile.wall == true)
	{
	return;
	}*/
	if (mapData.data[x + y*mapData.width].wall)
	{
		return;
	}

	int id = y * worldSize + x;
	for (int index = 0; index < visitedList.size(); index++)
	{
		//Check on the visited list, and say if that cell is already in the visited list, then just return it
		if (id == visitedList[index]->id)
		{
			return;
		}
	}

	//Or next cell
	Node* newChild = new Node(x, y, parent);
	newChild->g = newC;
	newChild->h = parent->ManhattanDistance(directionNode);

	//set the int to unsigned for errors or warnings
	for (int index = 0; index < openList.size(); index++)
	{
		if (id == openList[index]->id)
		{
			float newF = newChild->g + newC + openList[index]->h;

			if (openList[index]->f() > newF)
			{
				openList[index]->g = newChild->g + newC;
				openList[index]->parent = newChild;
			}
			else //if the newF is not better than the one in the open list
			{
				delete newChild;
				return;
			}
		}
	}

	openList.push_back(newChild);
}

Node* PathFinding::GetNextNode()
{
	float bestF = 999999.0f;
	int cellIndex = -1;
	Node* nextCell = NULL;

	for (int index = 0; index < openList.size(); index++)
	{
		//GetF is always going to be smaller than bestF at the beginning
		if (openList[index]->f() < bestF)
		{
			bestF = openList[index]->f();
			cellIndex = index;
		}
	}

	if (cellIndex >= 0)
	{
		nextCell = openList[cellIndex];
		visitedList.push_back(nextCell);
		openList.erase(openList.begin() + cellIndex);
	}

	return nextCell;
}

void PathFinding::ContinuePath()
{
	//To make pathfinding faster:
	//for(int i = 0; i < 4; i++)

	//If the open list is empty, then there's no reason to go through here

	if (openList.empty())
	{
		return;
	}
	while (!openList.empty())
	{
		Node* currentNode = GetNextNode();

		//If the current cell has reached the goal cell
		if (currentNode->id == directionNode->id)
		{
			directionNode->parent = currentNode->parent;

			Node* getPath;

			//It will push back the shortest path that is in the goal cell, the for loop is going through the cells and checking which is the shortest path
			for (getPath = directionNode; getPath != NULL; getPath = getPath->parent)
			{
				pathToDirection.push_back(new kf::Vector2(getPath->xCoord * 1, getPath->yCoord));
			}

			foundDirection = true;
			return;
		}

		else
		{
			//Calculate the right side
			PathOpened(currentNode->xCoord + 1, currentNode->yCoord, currentNode->g + 1, currentNode);

			//Calculate the left side
			PathOpened(currentNode->xCoord - 1, currentNode->yCoord, currentNode->g + 1, currentNode);

			//Calculate the up side
			PathOpened(currentNode->xCoord, currentNode->yCoord + 1, currentNode->g + 1, currentNode);

			//Calculate the down side
			PathOpened(currentNode->xCoord, currentNode->yCoord - 1, currentNode->g + 1, currentNode);

			for (int index = 0; index < openList.size(); index++)
			{
				if (currentNode->id == openList[index]->id)
				{
					openList.erase(openList.begin() + index);
				}
			}
		}
	}
}

kf::Vector2 PathFinding::NextPathPosition(kf::Vector2 botPosition)
{
	int index = 1;

	kf::Vector2 nextPosition;
	nextPosition.x = pathToDirection[pathToDirection.size() - index]->x + 0.5;
	nextPosition.y = pathToDirection[pathToDirection.size() - index]->y + 0.5;

	//position = AI's position, wherever you're going to use pathfinding to
	//kf::Vector2 distance = nextPosition - position;
	kf::Vector2 distance = nextPosition - botPosition;

	//if (index < pathToDirection.size())
	{
		//If the AI's position has touched our first position in the shortest path, radius = AI's object radius, when you want to implement it you need to set it
		if (distance.length() < radius)
			//_ai.radius
		{
			//Then erase it
			pathToDirection.erase(pathToDirection.end() - index);
		}
	}

	return nextPosition;
}
