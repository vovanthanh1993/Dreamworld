#pragma once
#include "cocos2d.h"
#include "main/Common.h"
#include "main/Constants.h"
#include "base/BaseNode.h"

using namespace constants;
using namespace common;
using namespace cocos2d;
class Port: public BaseNode
{
public:
	Port(b2World* world, Scene* scene, unordered_map<b2Body*, Sprite*>* _bodyToSpriteMap);
	bool init();
	TMXTiledMap* map;
};

