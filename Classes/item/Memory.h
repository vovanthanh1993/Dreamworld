#pragma once
#include "cocos2d.h"
#include "main/Common.h"
#include "main/Constants.h"
#include "base/BaseNode.h"
#include "player/Player.h"

using namespace constants;
using namespace common;
using namespace cocos2d;
class Memory :public BaseNode
{
public:
	Memory(b2World* world, Scene* scene, unordered_map<b2Body*, Sprite*>* bodyToSpriteMap);
	bool init(Vec2 position, int index);
	Player* player;
	void update(float dt);
};

