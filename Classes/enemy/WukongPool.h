#pragma once
#include "cocos2d.h"
#include "main/Common.h"
#include "main/Constants.h"
#include "Wukong.h"

using namespace constants;
using namespace common;
using namespace cocos2d;
class WukongPool
{
private:
	std::vector<Wukong*> pool;
public:
	WukongPool(b2World* world, Scene* scene, unordered_map<b2Body*, Sprite*>* bodyToSpriteMap, int poolSize);
	~WukongPool();
	Wukong* getFromPool();
	const std::vector<Wukong*>& getPool() const;        // Getter
	void setPool(const std::vector<Wukong*>& newPool);  // Setter
};

