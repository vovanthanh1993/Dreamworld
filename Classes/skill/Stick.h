#pragma once
#include "cocos2d.h"
#include "main/Common.h"
#include "base/BaseItem.h"
using namespace common;
using namespace cocos2d;
class Stick :public BaseItem
{
public:
	void init(b2World* world, Scene* scene, Vec2 position, unordered_map<b2Body*, Sprite*>* _bodyToSpriteMap);
	Stick();
};

