#include "Setting.h"
#include "skill/Stick.h"
#include "scene/GameOver.h"
#include <iostream>
#include <fstream>
#include <npc/ShopLayer.h>

// Constructor khởi tạo âm lượng mặc định
Setting::Setting() : volume(50), gameplayVol(50),sub(1), vsyn(1) {}

// Phương thức để lưu dữ liệu âm lượng vào file
void Setting::saveSettingData() {
    std::ofstream outFile("setting.txt");
    if (outFile.is_open()) {

        outFile << volume << "\n";
        outFile << gameplayVol << "\n";
        outFile << sub << "\n";
        outFile << vsyn << "\n";
        outFile.close();
        std::cout << "Data saved successfully." << std::endl;
    }
    else {
        std::cerr << "Unable to open file for writing." << std::endl;
    }
}

// Phương thức để tải dữ liệu âm lượng từ file
void Setting::loadSettingData() {
    std::ifstream inFile("setting.txt");
    if (inFile.is_open()) {
        inFile >> volume;
        inFile >> gameplayVol;
        inFile >> sub;
        inFile >> vsyn;
        inFile.close();
    }
}

// Getter cho volume
float Setting::getVolume() const {
    return volume;
}

// Setter cho volume
void Setting::setVolume(float vol) {
    volume = vol ;
}

// Getter cho bg
float Setting::getBgMusicId() const {
    return bgMusicId;
}

// Setter cho bg
void Setting::setBgMusicId(int id) {
    bgMusicId = id;
}

// Implementations for sub and vsyn
void Setting::setSub(int value) {
    sub = value;
}

int Setting::getSub() const {
    return sub;
}

void Setting::setVsyn(int value) {
    vsyn = value;
}

int Setting::getVsyn() const {
    return vsyn;
}

float Setting::getGameplayVol() const {
    return gameplayVol;
}
void Setting::setGameplayVol(float vol) {
    gameplayVol = vol;
}