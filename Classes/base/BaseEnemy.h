#pragma once
#include "cocos2d.h"
#include <iostream>
#include <fstream>
#include "main/Common.h"
#include <thread>
#include <chrono>
#include "base/BaseNode.h"
#include "player/Player.h"

using namespace common;
using namespace cocos2d;
using namespace std;
class BaseEnemy : public BaseNode
{
public:
	float attackCooldown;  // Thời gian chờ giữa các đợt tấn công
	float timeSinceLastAttack;  // Thời gian đã trôi qua kể từ lần tấn công cuối cùng
	bool canAttack = false;  // Cờ để xác định liệu kẻ thù có thể tấn công không
	bool isAlive = false;
	float attackRange;
	int direction;
	int health;
	int speed;
	BaseEnemy(b2World* world, Scene* scene, unordered_map<b2Body*, Sprite*>* bodyToSpriteMap);
	Player* player;
};

