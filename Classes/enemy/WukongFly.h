#ifndef __WukongFly_H__
#define __WukongFly_H__

#include "cocos2d.h"
#include "main/Common.h"
#include "player/Player.h"
#include "skill/SlashEnemy.h"
#include "item/Gem.h"
#include "main/Constants.h"
#include "main/Effect.h"
#include "main/MusicManager.h"
#include "base/BaseEnemy.h"

using namespace constants;
using namespace common;
using namespace cocos2d;
class WukongFly :public BaseEnemy
{
protected:
public:
	WukongFly(b2World* world, Scene* scene, unordered_map<b2Body*, Sprite*>* bodyToSpriteMap);
	bool init(Vec2 position);
	void idle();
	void die();
	void update(float dt);
	void followPlayer();
	void getDamage(int damage);
};

#endif // __WukongFly_H__