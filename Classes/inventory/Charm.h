#pragma once
#include "cocos2d.h"
#include "main/Common.h"
#include "main/Constants.h"
#include "base/BaseNode.h"

using namespace constants;
using namespace common;
using namespace cocos2d;
class Charm:public BaseNode
{
public:
	Charm(b2World* world, Scene* scene, unordered_map<b2Body*, Sprite*>* bodyToSpriteMap);
	bool init(Vec2 position);
	float manaBonus = 0;
	float healthBonus = 0;
	float dameBonus = 0;
	string name;
	string spritePath;
	int id;

};

