#pragma once
#include "cocos2d.h"
#include "main/Common.h"
#include "main/Constants.h"
#include "enemy/Soul.h"

using namespace constants;
using namespace common;
using namespace cocos2d;
class SoulPool
{
private:
	std::vector<Soul*> pool;
public:
	SoulPool(b2World* world, Scene* scene, unordered_map<b2Body*, Sprite*>* bodyToSpriteMap, int poolSize);
	~SoulPool();
	Soul* getFromPool();
	const std::vector<Soul*>& getPool() const;        // Getter
	void setPool(const std::vector<Soul*>& newPool);  // Setter
};

