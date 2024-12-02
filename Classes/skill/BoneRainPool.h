#pragma once
#include "cocos2d.h"
#include "main/Common.h"
#include "main/Constants.h"
#include "skill/BoneRain.h"

using namespace constants;
using namespace common;
using namespace cocos2d;
class BoneRainPool
{
private:
	std::vector<BoneRain*> pool;
public:
	BoneRainPool(b2World* world, Scene* scene, unordered_map<b2Body*, Sprite*>* bodyToSpriteMap, int poolSize);
	~BoneRainPool();
	BoneRain* getFromPool();
};

