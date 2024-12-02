#pragma once
#include "cocos2d.h"
#include "main/Common.h"
#include "main/Constants.h"
#include "player/skill/Eagle.h"

using namespace constants;
using namespace common;
using namespace cocos2d;
class EaglePool
{
private:
	std::vector<Eagle*> pool;
public:
	EaglePool(b2World* world, Scene* scene, unordered_map<b2Body*, Sprite*>* bodyToSpriteMap, int poolSize);
	~EaglePool();
	Eagle* getFromPool();
};

