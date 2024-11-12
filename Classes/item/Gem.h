#pragma once
#include "cocos2d.h"
#include "Common.h"
using namespace common;
using namespace cocos2d;
class Gem
{
private:
	Sprite* sprite;
	b2Body* body;
public:
	void init(b2World* world, Scene* scene, Vec2 position, unordered_map<b2Body*, Sprite*>* _bodyToSpriteMap, int num);
	Sprite* getSprite();
	b2Body* getBody();
};

