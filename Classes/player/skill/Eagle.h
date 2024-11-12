#pragma once
#include "cocos2d.h"
#include "Common.h"
#include "base/BaseItem.h"
using namespace common;
using namespace cocos2d;
class Eagle :public BaseItem
{

public:
	void init(b2World* world, Scene* scene, Vec2 position,
		unordered_map<b2Body*, Sprite*>* _bodyToSpriteMap);
};

