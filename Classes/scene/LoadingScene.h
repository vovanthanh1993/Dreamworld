#ifndef LOADINGSCENE_H
#define LOADINGSCENE_H

#include "cocos2d.h"
#include "main/Common.h"

class LoadingScene : public cocos2d::Scene
{
public:
    static LoadingScene* create();

    virtual bool init();
	vector<string> loadingtext = {
		"Tips: Buying and selling at NPCs, upgrading your character will help you fight more easily...",
		"Tips: Equip charms to change the character's stats...",
		"Tips: Jumping continuously when near a high wall will help you overcome it..."
	};

private:
    void loadResourcesAndSwitchScene();
};

#endif // LOADINGSCENE_H