#pragma once
#include "cocos2d.h"
#include "main/Constants.h"
#include "Gem.h"

using namespace constants;
using namespace cocos2d;
class GemPool
{
private:
	std::vector<Gem*> pool;
public:
	GemPool(b2World* world, Scene* scene, unordered_map<b2Body*, Sprite*>* bodyToSpriteMap, int poolSize);
	~GemPool();
	Gem* getFromPool();
};

