#ifndef BOT_H
#define BOT_H
#include "bot_interface.h"
#include "kf/kf_random.h"
#include "pathfinding.h"

//class Pathfinding;

#ifdef BOT_EXPORTS
#define BOT_API __declspec(dllexport)
#else
#define BOT_API __declspec(dllimport)
#endif

class Blank :public BotInterface27
{
public:
	Blank();
	virtual ~Blank();
	virtual void init(const BotInitialData &initialData, BotAttributes &attrib);
	virtual void update(const BotInput &input, BotOutput27 &output);
	virtual void result(bool won);
	virtual void bulletResult(bool hit);

	kf::Xor128 m_rand;
	BotInitialData m_initialData;
	kf::Vector2 dir;
	//Map m_map;
	PathFinding path;
	kf::Vector2 botPosition;

	kf::Vector2 targetPosition;
	std::vector<kf::Vector2> targetPositions;

	//Edits made through the Cody bot
	kf::Vector2		ourLastPosition;
	kf::Vector2		moveTowards;
	float			scanAngle;
	kf::Vector2		currentEnemyPosition;
	kf::Vector2		lastEnemyPosition;
	kf::Vector2		sLastEnemyPosition;
	int				lastEnemyUpdateCount;	//The update count when we last saw an enemy
	int				updateCount;			//The current update count (incremented by 1 every update)
	bool			enemyOutOfSight;
};


#endif