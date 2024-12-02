#pragma once
#include "cocos2d.h"
#include "main/Common.h"
#include "base/BaseNode.h"
#include "main/Constants.h"

using namespace constants;
using namespace common;
using namespace cocos2d;
class Stick :public BaseNode
{
public:
	bool init(Vec2 position);
	Stick(b2World* world, Scene* scene, unordered_map<b2Body*, Sprite*>* bodyToSpriteMap);
};

