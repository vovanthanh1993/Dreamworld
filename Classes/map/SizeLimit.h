#pragma once
#include "cocos2d.h"
#include "Common.h"
using namespace common;
using namespace cocos2d;
class SizeLimit
{
private:
	Sprite* sprite;
	b2Body* body;
public:
	SizeLimit(b2World* world, Scene* scene, unordered_map<b2Body*, Sprite*>* _bodyToSpriteMap, TMXTiledMap* map, string layerName);
	Sprite* getSprite();
	b2Body* getBody();
};

