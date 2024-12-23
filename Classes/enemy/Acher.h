#pragma once
#include "cocos2d.h"
#include "main/Common.h"
#include "player/Player.h"
#include "skill/Arrow.h"
#include "item/Gem.h"
#include "main/Constants.h"
#include "main/Effect.h"
#include "base/BaseEnemy.h"

using namespace constants;
using namespace common;
using namespace cocos2d;
class Acher: public BaseEnemy
{
private:
	BasePool<Arrow>* arrowPool = new BasePool<Arrow>();
public:
	Acher(b2World* world, Scene* scene, unordered_map<b2Body*, Sprite*>* bodyToSpriteMap);
	bool init(Vec2 position);
	void walk();
	void idle();
	void hit();
	void update(float dt);
	void die();
	void followPlayer();
	void getDamage(int damage);
	void hurt();
};

