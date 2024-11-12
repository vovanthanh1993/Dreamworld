#pragma once
#include "cocos2d.h"
#include "Common.h"
#include "base/BaseItem.h"
using namespace common;
using namespace cocos2d;
class Chest : public BaseItem
{
public:
	Chest(b2World* world, Scene* scene, unordered_map<b2Body*, Sprite*>* _bodyToSpriteMap, TMXTiledMap* map);
	void init();
};

