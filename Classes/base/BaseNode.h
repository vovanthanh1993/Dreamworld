#pragma once
#include "cocos2d.h"
#include <iostream>
#include <fstream>
#include "main/Common.h"
#include <thread>
#include <chrono>

using namespace common;
using namespace cocos2d;
using namespace std;
class BaseNode : public Node
{
protected:
	Sprite* sprite;
	SpriteBatchNode* spriteNode;
	b2Body* body;
	Scene* scene;
	b2World* world;
	Vec2 position;
	unordered_map<b2Body*, Sprite*>* bodyToSpriteMap;
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	bool isActive = true;

public:
	Sprite* getSprite();
	b2Body* getBody();
	SpriteBatchNode* getSpriteNode();
	BaseNode(b2World* world, Scene* scene, Vec2 position, unordered_map<b2Body*, Sprite*>* bodyToSpriteMap);
	BaseNode(b2World* world, Scene* scene, Vec2 position);
	void destroyNode();
};

