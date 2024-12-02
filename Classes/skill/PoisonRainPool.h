#pragma once
#include "cocos2d.h"
#include "main/Common.h"
#include "main/Constants.h"
#include "PoisonRain.h"

using namespace constants;
using namespace common;
using namespace cocos2d;
class PoisonRainPool
{
private:
	std::vector<PoisonRain*> pool;
public:
	PoisonRainPool(b2World* world, Scene* scene, unordered_map<b2Body*, Sprite*>* bodyToSpriteMap, int poolSize);
	~PoisonRainPool();
	PoisonRain* getFromPool();
};

