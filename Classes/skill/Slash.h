#pragma once
#include "cocos2d.h"
#include "main/Common.h"
#include "main/Constants.h"

using namespace constants;
using namespace common;
using namespace cocos2d;
class Slash
{
private:
	Sprite* sprite;
	b2Body* body;
	float duration = 0.02; // Thời gian tồn tại tính bằng giây
	std::chrono::steady_clock::time_point startTime;
	bool visible = true;
public:
	void init(b2World* world, Scene* scene, Vec2 position);
	Sprite* getSprite();
	b2Body* getBody();
	void update(float deltaTime, Scene* scene);
	bool IsVisible();
};

