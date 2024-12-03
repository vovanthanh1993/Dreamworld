#pragma once
#include "cocos2d.h"
#include "main/Common.h"
#include "main/Constants.h"
#include "item/Chest.h"
#include "item/Heart.h"
#include "item/BackStick.h"
#include "item/Bridge.h"
#include "item/BrokenBridge.h"
#include "item/Stone.h"

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
	void spawnBridge(int type);
	void spawnBrokenBridge();
	void spawnStone();
	void spawnWallAndLimit();
	void spawnAll();
	void spawnEndGate();
	void spawnBase();
};

