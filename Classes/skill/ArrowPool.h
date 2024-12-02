#pragma once
#include "cocos2d.h"
#include "main/Common.h"
#include "main/Constants.h"
#include "skill/Arrow.h"

using namespace constants;
using namespace common;
using namespace cocos2d;
class ArrowPool
{
private:
	std::vector<Arrow*> pool;
public:
	ArrowPool(b2World* world, Scene* scene, unordered_map<b2Body*, Sprite*>* bodyToSpriteMap, int poolSize);
	~ArrowPool();
	Arrow* getFromPool();
};

