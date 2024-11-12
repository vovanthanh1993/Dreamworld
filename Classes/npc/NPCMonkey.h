#pragma once
#include "cocos2d.h"
#include "Common.h"

using namespace common;
using namespace cocos2d;
class NPCMonkey
{
private:
	Sprite* sprite;
	SpriteBatchNode* spriteNode;
	b2Body* body;
public:
	NPCMonkey(b2World* world, Scene* scene, Vec2 position, unordered_map<b2Body*, Sprite*>* _bodyToSpriteMap);
	void walk();
};

