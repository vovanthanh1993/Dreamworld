#pragma once
#ifndef __Miko_H__
#define __Miko_H__

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
class Miko: public BaseNode
{
private:
	void displayMessage(const std::string& message, Scene* scene);
	std::string _characterName; // Tên nhân vật
	cocos2d::Label* _nameLabel; // Label hiển thị tên
public:
	void idle();
	Miko(b2World* world, Scene* scene, unordered_map<b2Body*, Sprite*>* bodyToSpriteMap);
	bool init(Vec2 position);
	void startConversation(Scene* scene);
};

#endif // __Miko_H__