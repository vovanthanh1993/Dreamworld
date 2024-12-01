#pragma once
#include "cocos2d.h"
#include "main/Common.h"
#include "main/Constants.h"
#include "main/Constants.h"
#include "base/BaseNode.h"

using namespace constants;
using namespace common;
using namespace cocos2d;
class StoneBall: public BaseNode
{
private:
	float duration = 5.0f; // Thời gian tồn tại tính bằng giây
	std::chrono::steady_clock::time_point startTime;
	bool isActive = true;
	float scale = 0.4;
public:
	StoneBall(b2World* world, Scene* scene, Vec2 position, unordered_map<b2Body*, Sprite*>* bodyToSpriteMap);
	bool init();
	void update(float dt);
	void destroy();
};

