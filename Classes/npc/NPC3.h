#pragma once
#ifndef __NPC3_H__
#define __NPC3_H__

#include "cocos2d.h"
#include "main/Common.h"
#include "skill/SlashEnemy.h"
#include "item/Gem.h"
#include "main/Constants.h"
#include "main/Effect.h"
#include "base/BaseNode.h"

using namespace constants;
using namespace common;
using namespace cocos2d;
class NPC3:public BaseNode
{
public:
	void idle();
	NPC3(b2World* world, Scene* scene, unordered_map<b2Body*, Sprite*>* bodyToSpriteMap);
	bool init(Vec2 position);
};
#endif // __NPC3_H__
