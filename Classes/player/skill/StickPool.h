#pragma once
#include "cocos2d.h"
#include "main/Common.h"
#include "main/Constants.h"
#include "player/skill/Stick.h"

using namespace constants;
using namespace common;
using namespace cocos2d;
class StickPool
{
private:
	std::vector<Stick*> pool;
public:
	StickPool(b2World* world, Scene* scene, unordered_map<b2Body*, Sprite*>* bodyToSpriteMap, int poolSize);
	~StickPool();
	Stick* getFromPool();
};

