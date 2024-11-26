#pragma once
#include "cocos2d.h"
#include "main/Common.h"
#include "base/BaseNode.h"
#include "main/Constants.h"

using namespace constants;
using namespace common;
using namespace cocos2d;
class Arrow:public BaseNode
{
public:
	Arrow(b2World* world, Scene* scene, Vec2 position, unordered_map<b2Body*, Sprite*>* bodyToSpriteMap);
	bool init();
};

