#pragma once
#include "cocos2d.h"
#include "main/Common.h"
#include "main/Constants.h"
#include "WukongFly.h"

using namespace constants;
using namespace common;
using namespace cocos2d;
class WukongFlyPool
{
private:
	std::vector<WukongFly*> pool;
public:
	WukongFlyPool(b2World* world, Scene* scene, unordered_map<b2Body*, Sprite*>* bodyToSpriteMap, int poolSize);
	~WukongFlyPool();
	WukongFly* getFromPool();
	const std::vector<WukongFly*>& getPool() const;        // Getter
	void setPool(const std::vector<WukongFly*>& newPool);  // Setter
};

