#pragma once
#include "cocos2d.h"
#include "main/Common.h"
using namespace common;
using namespace cocos2d;
class EndGate
{
private:
	Sprite* sprite;
	b2Body* body;
public:
	EndGate(b2World* world, Scene* scene, unordered_map<b2Body*, Sprite*>* _bodyToSpriteMap, TMXTiledMap* map);
	Sprite* getSprite();
	b2Body* getBody();
};

