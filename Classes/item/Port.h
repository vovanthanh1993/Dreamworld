#pragma once
#include "cocos2d.h"
#include "main/Common.h"
#include "main/Constants.h"

using namespace constants;
using namespace common;
using namespace cocos2d;
class Port
{
private:
	Sprite* sprite;
	b2Body* body;
public:
	Port(b2World* world, Scene* scene, unordered_map<b2Body*, Sprite*>* _bodyToSpriteMap, TMXTiledMap* map);
	Sprite* getSprite();
	b2Body* getBody();
};

