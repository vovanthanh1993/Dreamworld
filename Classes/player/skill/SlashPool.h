#pragma once
#include "cocos2d.h"
#include "main/Common.h"
#include "main/Constants.h"
#include "player/skill/Slash.h"

using namespace constants;
using namespace common;
using namespace cocos2d;
class SlashPool
{
private:
	std::vector<Slash*> pool;
public:
	SlashPool(b2World* world, Scene* scene, int poolSize);
	~SlashPool();
	Slash* getFromPool();
};

