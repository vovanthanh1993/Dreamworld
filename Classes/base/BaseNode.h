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
	bool isActive = false;
	float scale = 1;
public:
	Sprite* getSprite();
	b2Body* getBody();
	SpriteBatchNode* getSpriteNode();
	BaseNode(b2World* world, Scene* scene, Vec2 position, unordered_map<b2Body*, Sprite*>* bodyToSpriteMap);
	BaseNode(b2World* world, Scene* scene, Vec2 position);
	BaseNode(b2World* world, Scene* scene);
	BaseNode(b2World* world, Scene* scene, unordered_map<b2Body*, Sprite*>* bodyToSpriteMap);
	void destroyNode();

	// Phương thức Get cho isActive
	bool getIsActive();

	// Phương thức Set cho isActive
	void setIsActive(bool active);

	// Phương thức Get cho scale
	float getScale();

	// Phương thức Set cho scale
	void setScale(float newScale);

	void reset();
};

