#pragma once
#ifndef COMMONS_H
#define COMMONS_H
#include <string>
#include "cocos2d.h"
#include "box2d/Box2D.h"
#include <unordered_map>
#include "main/Constants.h"

using namespace constants;
using namespace cocos2d;
using namespace std;
namespace common {
	class Common
	{
	public:
		static Animation* createAnimation(string prefixName, int pFramesOrder, float delay);
		static float scaleSizeY();
		static float scaleSizeX();
		static float scaleSizeXY();
		static Vec2 scalePos(Vec2 pos);
		static void updatePosition(b2World* world, unordered_map<b2Body*, Sprite*>* _bodyToSpriteMap);
		static void destroyObject(b2World* world, b2Body* body, unordered_map<b2Body*, Sprite*>& _bodyToSpriteMap);
				
		static int randomNum(int from, int to);
		static void togglePause(bool& isPaused, Scene* scene);
		static void zoomAction(MenuItemImage* item);
		static int playBackgroundMusic(float volume, string path);
		static void changeSpriteColor(Sprite* sprite, bool& isHit);
		static void showBossText(Scene* scene, string text);
		static void saveRound(int round);
		static int loadRound();
		static void zoomAction(MenuItemLabel* item);
		static void checkAndRemoveSprite(cocos2d::Sprite* sprite, cocos2d::Camera* camera);
		static b2Body* createBoundary(b2World* world, bool isFirst);
		static void zoomAction(Sprite* item);
		static Vec2 getCenter();
		static bool isCollision(b2Body* body, int compareTag);
		static void spawnGem(b2World* world, Scene* scene, Vec2 position, unordered_map<b2Body*, Sprite*>* bodyToSpriteMap, int num);

		template <typename T>
		static void scaleAll(T* node, float ratio) {
			// Kiểm tra xem đối tượng có kế thừa từ Node không (an toàn hơn khi sử dụng dynamic_cast)
			if (auto cocosNode = dynamic_cast<cocos2d::Node*>(node)) {
				auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
				auto contentSize = cocosNode->getContentSize();

				// Nếu đối tượng là Menu, cần tính toán kích thước dựa trên các MenuItem
				if (auto menu = dynamic_cast<cocos2d::Menu*>(node)) {
					// Lấy kích thước của Menu dựa trên các item bên trong nó
					for (auto& item : menu->getChildren()) {
						auto itemSize = item->getContentSize();
						contentSize.width = std::max(contentSize.width, itemSize.width);
						contentSize.height = std::max(contentSize.height, itemSize.height);
					}
				}

				// Tính toán scale như bình thường
				float scaleX = visibleSize.width / contentSize.width;
				float scaleY = visibleSize.height / contentSize.height;

				float scale = std::max(scaleX, scaleY);
				cocosNode->setScale(scale * ratio);
			}
		}
	};
}
#endif // COMMONS_H


