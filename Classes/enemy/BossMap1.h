#pragma once
#include "cocos2d.h"
#include "main/Common.h"
#include "player/Player.h"
#include "skill/SlashEnemy.h"
#include "skill/Fire.h"
#include "skill/FireRain.h"
#include "item/Gem.h"
#include "main/Constants.h"

using namespace constants;
using namespace common;
using namespace cocos2d;
class BossMap1:public BaseCharacter
{
private:
	float attackCooldown =1.5f;  // Thời gian chờ giữa các đợt tấn công
	float timeSinceLastAttack = 0.0f;  // Thời gian đã trôi qua kể từ lần tấn công cuối cùng
	bool canAttack = false;  // Cờ để xác định liệu kẻ thù có thể tấn công không
	int health = 20;
	int count = 1;
	float maxHealth =20;
	cocos2d::Sprite* healthBar; // Sprite cho thanh máu
	cocos2d::Sprite* healthBarBg; // Sprite cho nền thanh máu
	bool isHit = false;
public:
	void init();
	void walk();
	void idle();
	void die();
	void hit();
	void updateAttack(Player* player, float dt);
	void setHealth(int h);
	int getHealth();
	bool isALive = true;
	void phase2();
	void updateHealth(int damage);
	void updateHealthBar(float health);
	void createHealthBar();
	void updateHealthBarPosition();
	void hurt();
	BossMap1(b2World* world, Scene* scene, Vec2 position, unordered_map<b2Body*, Sprite*>* _bodyToSpriteMap);
};

