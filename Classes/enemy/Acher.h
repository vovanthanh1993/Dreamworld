#pragma once
#include "cocos2d.h"
#include "main/Common.h"
#include "player/Player.h"
#include "skill/Arrow.h"
#include "item/Gem.h"

using namespace common;
using namespace cocos2d;
class Acher: public BaseCharacter
{
private:
	float attackCooldown = 2.5f;  // Thời gian chờ giữa các đợt tấn công
	float timeSinceLastAttack = 0.0f;  // Thời gian đã trôi qua kể từ lần tấn công cuối cùng
	bool canAttack = false;  // Cờ để xác định liệu kẻ thù có thể tấn công không
public:
	Acher(b2World* world, Scene* scene, Vec2 position, unordered_map<b2Body*, Sprite*>* _bodyToSpriteMap);
	void init();
	void walk();
	void idle();
	Arrow* hit();
	void updateAttack(Player* player, float dt);
	void die();
};

