#pragma once
#include "cocos2d.h"
#include "Common.h"
#include <iostream>
#include <thread>
#include <chrono>

using namespace common;
using namespace cocos2d;
using namespace std;
class BaseItem
{
protected:
	Sprite* sprite;
	SpriteBatchNode* spriteNode;
	b2Body* body;
	Scene* scene;
	b2World* world;
	unordered_map<b2Body*, Sprite*>* _bodyToSpriteMap;
	TMXTiledMap* map;
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
public:
	Sprite* getSprite();
	b2Body* getBody();
	SpriteBatchNode* getSpriteNode();
	BaseItem(b2World* world, Scene* scene, unordered_map<b2Body*, Sprite*>* _bodyToSpriteMap, TMXTiledMap* map);
	BaseItem();
	BaseItem(b2World* world, Scene* scene, unordered_map<b2Body*, Sprite*>* _bodyToSpriteMap);
};

