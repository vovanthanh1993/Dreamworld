#pragma once
#include "cocos2d.h"
#include "main/Common.h"
#include "player/Player.h"
#include "skill/SlashEnemy.h"
#include "skill/Fire.h"
#include "skill/Rain.h"
#include "item/Gem.h"
#include "main/Constants.h"

using namespace constants;
using namespace common;
using namespace cocos2d;
class BossMap2:public BaseCharacter
{
private:
	float attackCooldown =1.5f;  // Thời gian chờ giữa các đợt tấn công
	float timeSinceLastAttack = 0.0f;  // Thời gian đã trôi qua kể từ lần tấn công cuối cùng
	bool canAttack = false;  // Cờ để xác định liệu kẻ thù có thể tấn công không
	int health = 20;
	int count = 1;
	int maxHealth = 20;
	cocos2d::Sprite* healthBar; // Sprite cho thanh máu
	cocos2d::Sprite* healthBarBg; // Sprite cho nền thanh máu
	bool isHit = false;
public:
	BossMap2(b2World* world, Scene* scene, Vec2 position, unordered_map<b2Body*, Sprite*>* _bodyToSpriteMap);
	void init();
	void walk();
	void idle();
	void hurt();
	void die();
	void updateAttack(Player* player, float dt, TMXTiledMap* map);
	void setHealth(int h);
	int getHealth();
	bool isALive = true;
	void skill2();
	void updateHealth(int damage);
	void moveBodyToPoint(TMXTiledMap* map);
	void moveBodyToInit(TMXTiledMap* map);
	bool isInPoint = false;
	int countPhase2 = 1;
	void throwSkull();
	void updateHealthBar(float health);
	void createHealthBar();
	void updateHealthBarPosition();
};

