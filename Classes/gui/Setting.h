#pragma once
#include "cocos2d.h"
#include "main/Common.h"
#include <iostream>
#include <thread>
#include <chrono>
#include "skill/Slash.h"
#include <npc/NPC1.h>

using namespace common;
using namespace cocos2d;
using namespace std;
class Setting
{
private:
	float volume;
    float gameplayVol;
    int bgMusicId;
    int sub;
    int vsyn;
public:
	
    Setting(); // Constructor
    void saveSettingData(); // Lưu dữ liệu âm lượng
    void loadSettingData(); // Tải dữ liệu âm lượng

    // Getter cho volume
    float getVolume() const;

    // Setter cho volume
    void setVolume(float vol);

    
    float getGameplayVol() const;
    void setGameplayVol(float vol);

    // Getter cho volume
    float getBgMusicId() const;

    // Setter cho volume
    void setBgMusicId(int vol);

    // Setters and Getters for sub and vsyn
    void setSub(int value);
    int getSub() const;

    void setVsyn(int value);
    int getVsyn() const;
};

