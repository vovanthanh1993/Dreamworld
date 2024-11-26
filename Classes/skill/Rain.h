#pragma once
#include "cocos2d.h"
#include "main/Common.h"
#include "base/BaseNode.h"
#include "main/Constants.h"

using namespace constants;
using namespace common;
using namespace cocos2d;
class Rain :public BaseNode
{
public:
	bool init();
	Rain(b2World* world, Scene* scene, Vec2 position, unordered_map<b2Body*, Sprite*>* bodyToSpriteMap);
};

