#pragma once
#include "cocos2d.h"
#include "main/Common.h"
#include "main/Constants.h"
#include "skill/SlashEnemy.h"

using namespace constants;
using namespace common;
using namespace cocos2d;
class SlashEnemyPool
{
private:
	std::vector<SlashEnemy*> pool;
public:
	SlashEnemyPool(b2World* world, Scene* scene, int poolSize);
	~SlashEnemyPool();
	SlashEnemy* getFromPool();
};

