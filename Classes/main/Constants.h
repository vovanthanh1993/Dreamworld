#pragma once
#include <string>
#include "cocos2d.h"
#include "box2d/Box2D.h"
#include <unordered_map>
using namespace cocos2d;
using namespace std;
namespace constants {
	class Constants
	{
	public:
		static const int MAX_SIZE_MAP = 14376;
		static const int PIXELS_PER_METER = 32;
		static const int TITLE_SIZE = 24;
		static inline const double PLAYER_SCALE = 0.6;
		static inline const double WARRIOR_SCALE = 0.2;
		static inline const double STICK_SCALE = 0.3;
		static inline const double ACHER_SCALE = 0.15;
		static inline const double ARROW_SCALE = 0.2;
		static inline const double BRIDGE_SCALE = 0.9;
		static inline const double BOX_SCALE = 0.2;
		static inline const double BOSSMAP1_SCALE = 1;
		static inline const double BOSSMAP2_SCALE = 1;
		static inline const double FIRE_SCALE = 1.5;
		static inline const double GEM_SCALE = 0.07;
		static inline const double WRAITH_SCALE = 1;
		static inline const double PORT_SCALE = 0.8;
		static inline const double NPC_SCALE = 0.3;
		static inline const double SKULL_SCALE = 0.6;
		static inline const double BONE_SCALE = 0.4;
		static inline const double HEALTH_BAR_SCALE = 0.4;
		static inline const double NPC3_SCALE = 0.5;
		static inline const double FIRE_RAIN_SCALE = 0.8;
		static inline const double HAFL_SCALE = 0.5;

		static const uint16 CATEGORY_PLAYER = 0x0001; // 0001 trong nhị phân
		static const uint16 CATEGORY_STICK = 0x0002;
		static const uint16 CATEGORY_ENEMY = 0x0004;
		static const uint16 CATEGORY_LIMIT = 0x0008;
		static const uint16 CATEGORY_WALL = 0x0010;
		static const uint16_t CATEGORY_SLASH_ENEMY = 0x0020;
		static const uint16_t CATEGORY_ITEM = 0x0040;
		static const uint16_t CATEGORY_SLASH = 0x0080;
		static const uint16_t CATEGORY_CHEST = 0x0100;
		static const uint16_t CATEGORY_ARROW = 0x0200;
		static const uint16_t CATEGORY_STONE = 0x0400;
		static const uint16_t CATEGORY_GEM = 0x0800;
		static const uint16_t CATEGORY_NPC = 0x1000;
		static const uint16_t CATEGORY_BLOCK = 0x2000;
		static const uint16_t CATEGORY_CLOUD = 0x4000;
		static const uint16_t CATEGORY_LIMIT_MAP = 0x8000;

		static const int TAG_PLAYER = 1;
		static const int TAG_STICK = 2;
		static const int TAG_ACHER = 3;
		static const int TAG_LIMIT = 4;
		static const int TAG_WALL = 5;
		static const int TAG_SLASH = 6;
		static const int TAG_SLASH_ENEMY = 7;
		static const int TAG_CHEST = 8;
		static const int TAG_HEART = 9;
		static const int TAG_ARROW = 10;
		static const int TAG_BRIDGE = 11;
		static const int TAG_BRIDGE_BREAK = 12;
		static const int TAG_BOX = 13;
		static const int TAG_BOSSMAP1 = 14;
		static const int TAG_FIRE = 15;
		static const int TAG_GEM = 16;
		static const int TAG_ENDGATE = 17;
		static const int TAG_RAIN = 18;
		static const int TAG_NPC = 19;
		static const int TAG_BOSSMAP2 = 20;
		static const int TAG_BONE_RAIN = 22;
		static const int TAG_COPY = 23;
		static const int TAG_EAGLE = 24;
		static const int TAG_STICK_ITEM = 25;
		static const int TAG_FIRE_RAIN = 26;
		static const int TAG_WAR = 27;
		static const int TAG_ELEMENT = 28;
		static const int TAG_HED = 29;
		static const int TAG_GOLEM = 30;
		static const int TAG_BAT = 31;
		static const int TAG_BOSSMAP3 = 32;
		static const int TAG_STONE_BALL = 33;
		static const int TAG_EQUIPMENT = 34;
		static const int TAG_WUKONG = 35;
		static const int TAG_WUKONG_FLY = 36;
		static const int TAG_BOSSMAP4 = 37;
		

		static const int MAX_HEALTH = 15;
		static const int MAX_MANA = 15;
		static const int GRAVITY = -250;
		static const int SPEED_ENEMY = -6;
		static const int SPEED_PLAYER = 15;
		static const int SPEED_BRIDGE = 10;
		static const int SPEED_BRIDGE_BREAK = 10;
		static const int SPEED_BOSS2 = -15;
		static const int SPEED_BOSS1 = -10;

		static const int JUMP_PLAYER = 60;
		static const int ATTACK_RANGE_WAR = 3;
		static const int ATTACK_RANGE_BOSS_MAP1 = 50;
		static const int ATTACK_RANGE_ACHER = 15;
		static const int ATTACK_RANGE_WRAITH = 25;
		static const int TALK_RANGE = 3;
		static const int ATTACK_RANGE_BOSS_MAP2 = 50;
		static const int COLLECT_RANGE = 2;

		static const int DAMAGE_ARROW = 3;
		static const int DAMAGE_BOOM = 3;
		static const int DAMAGE_SLASH_ENEMY = 2;
		static const int DAMAGE_STONE_BALL = 2;
	};
}


