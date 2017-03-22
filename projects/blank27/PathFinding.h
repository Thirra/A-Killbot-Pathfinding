#pragma once
#include "kf/kf_vector2.h"
#include "bot_interface.h"
//#include "bot.h"
#include "Node.h"
#include <vector>

class PathFinding
{
public:
	PathFinding();
	~PathFinding();

	void FindPath(kf::Vector2 currentPosition, kf::Vector2 targetPosition);
	kf::Vector2 NextPathPosition(kf::Vector2 botPosition);

	void ClearOpenList()
	{
		openList.clear();
	}

	void ClearVisitedList()
	{
		visitedList.clear();
	}

	void ClearPathToDirection()
	{
		pathToDirection.clear();
	}

	bool initStartDirection;
	bool foundDirection;
	MapData mapData;

	std::vector<kf::Vector2*> pathToDirection;

private:
	void StartandDirection(Node start, Node direction);
	void PathOpened(int x, int y, float newC, Node *parent);
	Node* GetNextNode();
	void ContinuePath();

	Node *startNode;
	Node *directionNode;
	std::vector<Node*> openList;
	std::vector<Node*> visitedList;

	Tile tile;
	//	Blank blankBot;

	float radius = 0.5f;
};
