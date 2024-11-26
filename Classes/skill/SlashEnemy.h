#pragma once
#include "cocos2d.h"
#include "main/Common.h"
#include "main/Constants.h"
#include "main/Constants.h"
#include "base/BaseNode.h"

using namespace constants;
using namespace common;
using namespace cocos2d;
class SlashEnemy: public BaseNode
{
private:
	float duration = 0.01; // Thời gian tồn tại tính bằng giây
	std::chrono::steady_clock::time_point startTime;
	bool visible = true;
public:
	SlashEnemy(b2World* world, Scene* scene, Vec2 position);
	bool init();
	void update(float dt);
	bool IsVisible();
};

