#pragma once
#include "cocos2d.h"
#include "main/Common.h"
#include "skill/SlashEnemy.h"
#include "item/Gem.h"
#include "main/Constants.h"

using namespace constants;
using namespace common;
using namespace cocos2d;
class NPC3
{
private:
	Sprite* sprite;
	SpriteBatchNode* spriteNode;
	b2Body* body;
	void displayMessage(const std::string& message, Scene* scene);
	std::string _characterName; // Tên nhân vật
	cocos2d::Label* _nameLabel; // Label hiển thị tên
public:
	
	void idle();
	Sprite* getSprite();
	b2Body* getBody();
	SpriteBatchNode* getSpriteNode();
	NPC3(b2World* world, Scene* scene, Vec2 position, unordered_map<b2Body*, Sprite*>* _bodyToSpriteMap);
	void startConversation(Scene* scene);
};

