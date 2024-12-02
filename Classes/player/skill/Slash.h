#pragma once
#include "cocos2d.h"
#include "main/Common.h"
#include "main/Constants.h"
#include "base/BaseNode.h"

using namespace constants;
using namespace common;
using namespace cocos2d;
class Slash:public BaseNode
{
private:
	float duration = 0.01; // Thời gian tồn tại tính bằng giây
	std::chrono::steady_clock::time_point startTime;
public:
	Slash(b2World* world, Scene* scene);
	bool init(Vec2 position);
	void update(float dt);
};

