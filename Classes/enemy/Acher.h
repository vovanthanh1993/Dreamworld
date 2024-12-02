#pragma once
#include "cocos2d.h"
#include "main/Common.h"
#include "player/Player.h"
#include "skill/Arrow.h"
#include "skill/ArrowPool.h"
#include "item/Gem.h"
#include "main/Constants.h"
#include "main/Effect.h"
#include "base/BaseNode.h"

using namespace constants;
using namespace common;
using namespace cocos2d;
class Acher: public BaseNode
{
private:
	float attackCooldown = 2.5f;  // Thời gian chờ giữa các đợt tấn công
	float timeSinceLastAttack = 0.0f;  // Thời gian đã trôi qua kể từ lần tấn công cuối cùng
	bool canAttack = false;  // Cờ để xác định liệu kẻ thù có thể tấn công không
	bool isAlive = true;
	float attackRange = 15;
	ArrowPool* arrowPool;
	int direction = -1;
public:
	Acher(b2World* world, Scene* scene, Vec2 position, unordered_map<b2Body*, Sprite*>* bodyToSpriteMap);
	bool init();
	void walk();
	void idle();
	void hit();
	void update(float dt);
	void die();
	Player* player;
};

