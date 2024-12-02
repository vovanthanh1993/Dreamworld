#pragma once
#include "cocos2d.h"
#include "main/Common.h"
#include "main/Constants.h"
#include "StoneBall.h"

using namespace constants;
using namespace common;
using namespace cocos2d;
class StoneBallPool
{
private:
	std::vector<StoneBall*> pool;
public:
	StoneBallPool(b2World* world, Scene* scene, unordered_map<b2Body*, Sprite*>* bodyToSpriteMap, int poolSize);
	~StoneBallPool();
	StoneBall* getFromPool();
};

