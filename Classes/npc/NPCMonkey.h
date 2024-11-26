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
	bool init();
	NPCMonkey(b2World* world, Scene* scene, Vec2 position, unordered_map<b2Body*, Sprite*>* bodyToSpriteMap);
	void walk();
};
#endif // __NPCMonkey_H__
