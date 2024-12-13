#pragma once
#include "cocos2d.h"
#include "main/Common.h"
#include "main/Constants.h"
#include "BackStick.h"

using namespace constants;
using namespace common;
using namespace cocos2d;
class BackStickPool
{
private:
	std::vector<BackStick*> pool;
public:
	BackStickPool(b2World* world, Scene* scene, unordered_map<b2Body*, Sprite*>* bodyToSpriteMap, int poolSize);
	~BackStickPool();
	BackStick* getFromPool();
	const std::vector<BackStick*>& getPool() const;        // Getter
	void setPool(const std::vector<BackStick*>& newPool);  // Setter
};

