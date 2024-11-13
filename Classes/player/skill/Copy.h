#pragma once
#include "cocos2d.h"
#include "main/Common.h"
using namespace common;
using namespace cocos2d;
class Copy
{
private:
	Sprite* sprite;
	b2Body* body;
public:
	void init(b2World* world, Scene* scene, Vec2 position,
		unordered_map<b2Body*, Sprite*>* _bodyToSpriteMap);
	Sprite* getSprite();
	b2Body* getBody();
};

