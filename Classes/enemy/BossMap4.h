#pragma once
#include "cocos2d.h"
#include "main/Common.h"
#include "main/Constants.h"
#include "base/BaseEnemy.h"
#include "main/Effect.h"
#include "enemy/Wukong.h"
#include "enemy/WukongFly.h"
#include "item/Port.h"
#include "base/BasePool.h"

using namespace constants;
using namespace common;
using namespace cocos2d;
class BossMap4:public BaseEnemy
{
public:
	BossMap4(b2World* world, Scene* scene, unordered_map<b2Body*, Sprite*>* bodyToSpriteMap);
	bool init(Vec2 position);
	void getDamage(int damage);
	void hurt();
	bool isHit = false;
	void die();
	BasePool<Wukong>* wukongPool = new BasePool<Wukong>();
	BasePool<WukongFly>* wukongFlyPool = new BasePool<WukongFly>();
	TMXTiledMap* map;
	void spawnWukong();
	void spawnWukongFly();
};

