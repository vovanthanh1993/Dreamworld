#pragma once
#include "cocos2d.h"
#include "main/Common.h"
#include "player/Player.h"
#include "skill/SlashEnemy.h"
#include "item/Gem.h"
using namespace common;
using namespace cocos2d;
class Warrior:public BaseCharacter
{
private:
	float attackCooldown =2.0f;  // Thời gian chờ giữa các đợt tấn công
	float timeSinceLastAttack = 0.0f;  // Thời gian đã trôi qua kể từ lần tấn công cuối cùng
	bool canAttack = false;  // Cờ để xác định liệu kẻ thù có thể tấn công không
public:
	Warrior(b2World* world, Scene* scene, Vec2 position, unordered_map<b2Body*, Sprite*>* _bodyToSpriteMap);
	void init();
	void walk();
	void idle();
	void jump();
	SlashEnemy* hit();
	void updateAttack(vector<SlashEnemy*>& slashEnemyVector, Player* player, float dt);
	void die();
	bool isLive = true;
};

