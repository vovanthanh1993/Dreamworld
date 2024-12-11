#pragma once
#ifndef __NPCMonkey_H__
#define __NPCMonkey_H__

#include "cocos2d.h"
#include "main/Common.h"
#include "main/Constants.h"
#include "base/BaseNode.h"

using namespace constants;
using namespace common;
using namespace cocos2d;
class NPCMonkey: public BaseNode
{
public:
	bool init(Vec2 position);
	NPCMonkey(b2World* world, Scene* scene, unordered_map<b2Body*, Sprite*>* bodyToSpriteMap);
	void walk();
	int direction = -1;
	int speed = 6;
};
#endif // __NPCMonkey_H__
