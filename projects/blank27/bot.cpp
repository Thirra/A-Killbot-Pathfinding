#include "bot.h"
#include "time.h"

extern "C"
{
	BotInterface27 BOT_API *CreateBot27()
	{
		return new Blank();
	}
}

Blank::Blank()
{
	m_rand(time(0) + (int)this);
}

Blank::~Blank()
{
}

void Blank::init(const BotInitialData &initialData, BotAttributes &attrib)
{
	m_initialData = initialData;
	attrib.health = 1.0;
	attrib.motor = 0.2;
	attrib.weaponSpeed = 0.5;
	attrib.weaponStrength = 1.0;
	//dir.set(1, 0);
	//m_map.init(initialData.mapData.width, initialData.mapData.height);
	path.mapData = m_initialData.mapData;
	//targetPosition.set(30.5, 30.5);
	path.initStartDirection = false;
	targetPositions.clear();
	targetPositions.push_back(kf::Vector2(30.5, 30.5));
	targetPositions.push_back(kf::Vector2(1.5, 1.5));
	targetPositions.push_back(kf::Vector2(30.5, 1.5));
	targetPositions.push_back(kf::Vector2(1.5, 30.5));
	targetPositions.push_back(kf::Vector2(11.5, 16.5));
	targetPositions.push_back(kf::Vector2(15.5, 13.5));

	int randIndex = rand() % 6;
	targetPosition = targetPositions[randIndex];
}

void Blank::update(const BotInput &input, BotOutput27 &output)
{
	//output.moveDirection = dir;
	output.motor = 1.0;
	output.lookDirection.set(output.moveDirection);
	output.action = BotOutput::scan;
	bool enemySpotted = false;

	//First update 
	if (updateCount == 0)
	{
		output.lines.clear();
		output.text.clear();
	}

	botPosition = input.position;

	path.FindPath(input.position, targetPosition);
	kf::Vector2 next = path.NextPathPosition(input.position);
	output.moveDirection = next - input.position;

	if (path.pathToDirection.size()==0)
	{
		path.initStartDirection = false;
		int randIndex = rand() % 6;
		targetPosition = targetPositions[randIndex];
	}

	Line l;
	l.start = input.position;
	l.end = next;
	l.r = 1;
	l.g = 1;
	l.b = 1;
	output.lines.push_back(l);

	int dummy = 0;

	if (input.scanResult.size() > 0)
	{
		for (int index = 0; index < input.scanResult.size(); ++index)
		{
			if (input.scanResult[index].type == VisibleThing::e_robot)
			{
				currentEnemyPosition = input.scanResult[index].position;
				enemySpotted = true;

				break;
			}
		}
	}

	if (enemySpotted)
	{
		kf::Vector2 estimatedEnemyPosition = currentEnemyPosition;
		if (lastEnemyUpdateCount > -1)
		{
			kf::Vector2 delta = currentEnemyPosition - lastEnemyPosition;
			estimatedEnemyPosition = currentEnemyPosition + (delta / (updateCount - lastEnemyUpdateCount)) * 5;
			Line l;
			l.start = currentEnemyPosition;
			l.end = estimatedEnemyPosition;
			l.r = 1;
			l.g = 1;
			l.b = 1;
			output.lines.push_back(l);
		}
		// Shooting
		output.lookDirection = estimatedEnemyPosition - input.position;
		output.action = BotOutput::shoot;
		enemyOutOfSight = true;
	}

	else 
	{
		output.action = BotOutput::scan;
	}

	// Remember values for the next update.
	ourLastPosition = input.position;
	if (enemySpotted)
	{
		lastEnemyUpdateCount = updateCount;
		lastEnemyPosition = currentEnemyPosition;
	}
	updateCount++;
}

void Blank::result(bool won)
{
}

void Blank::bulletResult(bool hit)
{

}
