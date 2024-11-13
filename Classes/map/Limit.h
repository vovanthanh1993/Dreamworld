#pragma once
#include "cocos2d.h"
#include "main/Common.h"
using namespace common;
using namespace cocos2d;
class Limit
{
private:
	Sprite* sprite;
	b2Body* body;
public:
	Limit(b2World* world, Scene* scene, TMXTiledMap* map);
	Sprite* getSprite();
	b2Body* getBody();
};

