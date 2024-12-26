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
	int manaBonus = 0;
	int healthBonus = 0;
	int slashDamageBonus = 0;
	int stickDamageBonus = 0;
	int eagleDamageBonus = 0;
	string name;
	string spritePath;
	int id;
	string effect = " hello";
	void saveToFile(ofstream& out);
	void getEffectString();
};

