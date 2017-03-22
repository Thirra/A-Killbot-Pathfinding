#pragma once
#include "kf/kf_vector2.h"
#include <math.h>

#define worldSize 32

typedef kf::Vector2T<int> NodePos;

class Node
{
public:
	int c;
	int g;
	int h;
	int f()
	{
		return g + h;
	}

	Node *parent;
	int xCoord;
	int yCoord;
	int id;

	Node() : parent(0)
	{

	}

	Node(int x, int y, Node *_parent = 0)
		: xCoord(x), yCoord(y), parent(_parent), id(y * worldSize + x), g(0), h(0)
	{

	};

	float ManhattanDistance(Node *endNode)
	{
		float x = (float)(fabs((float)(this->xCoord - endNode->xCoord)));
		float y = (float)(fabs((float)(this->yCoord - endNode->yCoord)));

		return x + y;
	}


};

