#pragma once
#include "cocos2d.h"
#include "main/Common.h"
#include "main/Constants.h"

using namespace constants;
using namespace common;
using namespace cocos2d;
class MapItem
{
private:
	Scene* scene;
	b2World* world;
	unordered_map<b2Body*, Sprite*>* bodyToSpriteMap;
	TMXTiledMap* map;
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
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

