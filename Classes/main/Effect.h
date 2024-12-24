#pragma once
#ifndef __Effect_H__
#define __Effect_H__

#include "cocos2d.h"
#include "main/Common.h"
#include "player/Player.h"
#include "skill/SlashEnemy.h"
#include "item/Gem.h"
#include "audio/include/AudioEngine.h"
#include "main/Constants.h"

using namespace constants;
using namespace common;
using namespace cocos2d;
namespace common {
	class Effect
	{
	public:
		static void destroyStick(b2World* world, Scene* scene, Vec2 position);
		static void destroyArrow(b2World* world, Scene* scene, Vec2 position);
		static void destroyFire(b2World* world, Scene* scene, Vec2 position);
		
		static void destroyRain(b2World* world, Scene* scene, Vec2 position);
		static void smoke(b2World* world, Scene* scene, Vec2 position);
		static void destroyFireRain(b2World* world, Scene* scene, Vec2 position);
		
		static void healing(b2World* world, Scene* scene, Vec2 position);
	};
}

#endif // __Effect_H__