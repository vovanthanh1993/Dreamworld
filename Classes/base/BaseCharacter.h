#pragma once
#include "cocos2d.h"
#include "main/Common.h"
#include <iostream>
#include <thread>
#include <chrono>

using namespace common;
using namespace cocos2d;
using namespace std;
class BaseCharacter
{
protected:
	Sprite* sprite;
	SpriteBatchNode* spriteNode;
	b2Body* body;
	Scene* scene;
	b2World* world;
	unordered_map<b2Body*, Sprite*>* _bodyToSpriteMap;
	Vec2 position;
public:

	Sprite* getSprite();
	b2Body* getBody();
	SpriteBatchNode* getSpriteNode();
	BaseCharacter(b2World* world, Scene* scene, Vec2 position, unordered_map<b2Body*, Sprite*>* _bodyToSpriteMap);
};

