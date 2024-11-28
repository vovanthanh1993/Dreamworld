#pragma once
#include "cocos2d.h"
#include "main/Common.h"
#include "base/BaseNode.h"
#include "main/Constants.h"
#include "player/Player.h"

using namespace constants;
using namespace common;
using namespace cocos2d;
class Skull :public BaseNode
{
public:
	bool init();
	Skull(b2World* world, Scene* scene, Vec2 position, unordered_map<b2Body*, Sprite*>* bodyToSpriteMap);
	void followPlayer();
	Player* player;
	float scale = 0.6;
};

