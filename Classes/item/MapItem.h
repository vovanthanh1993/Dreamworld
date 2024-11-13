#pragma once
#include "cocos2d.h"
#include "main/Common.h"
#include "base/BaseItem.h"
using namespace common;
using namespace cocos2d;
class MapItem : public BaseItem
{
public:
	MapItem(b2World* world, Scene* scene, unordered_map<b2Body*, Sprite*>* _bodyToSpriteMap, TMXTiledMap* map);
	void spawnBackStick();
	void spawnChest();
	void spawnHeart();
	void spawnBridge();
	void spawnBridgeBreak();
	void spawnBox();
	void spawnWallAndLimit();
	void spawnAll();
	void spawnEndGate();
};

