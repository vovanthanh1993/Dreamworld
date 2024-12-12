#pragma once
#include "cocos2d.h"
#include "main/Common.h"
#include <iostream>
#include <thread>
#include <chrono>
#include "player/skill/Slash.h"
#include "player/skill/SlashPool.h"
#include "player/skill/EaglePool.h"
#include "player/skill/StickPool.h"
#include "base/BaseNode.h"
#include "main/Constants.h"
#include "Inventory/charm.h"

using namespace constants;
using namespace common;
using namespace cocos2d;
using namespace std;
class PlayerFly : public BaseNode
{
private:
	
	
public:
	void walk();
	void die();
	void idle();
	bool isAlive;
	void update(float dt);
};

