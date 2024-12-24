#include "SettingManager.h"
#include <iostream>
#include <fstream>

// Singleton instance
SettingManager* SettingManager::instance = nullptr;

// Private constructor khởi tạo âm lượng mặc định
SettingManager::SettingManager() : volume(50), gameplayVol(50), sub(1), vsyn(1), bgMusicId(0) {}

// Destructor
SettingManager::~SettingManager() {}

// Static method để truy cập instance duy nhất
SettingManager* SettingManager::getInstance() {
    if (instance == nullptr) {
        instance = new SettingManager();
    }
    return instance;
}

// Phương thức để xóa instance
void SettingManager::destroyInstance() {
    if (instance) {
        delete instance;
        instance = nullptr;
    }
}

// Phương thức để lưu dữ liệu âm lượng vào file
void SettingManager::saveSettingData() {
    std::ofstream outFile("SettingManager.txt");
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
void SettingManager::loadSettingData() {
    std::ifstream inFile("SettingManager.txt");
    if (inFile.is_open()) {
        if (!(inFile >> volume)) volume = 50; // Fallback to default
        if (!(inFile >> gameplayVol)) gameplayVol = 50; // Fallback to default
        if (!(inFile >> sub)) sub = 1; // Fallback to default
        if (!(inFile >> vsyn)) vsyn = 1; // Fallback to default
        inFile.close();
    }
    else {
        std::cerr << "Settings file not found. Using default values." << std::endl;
    }
}

// Getter cho volume
float SettingManager::getVolume() const {
    return volume;
}

// Setter cho volume
void SettingManager::setVolume(float vol) {
    volume = vol ;
}

// Getter cho bg
float SettingManager::getBgMusicId() const {
    return bgMusicId;
}

// Setter cho bg
void SettingManager::setBgMusicId(int id) {
    bgMusicId = id;
}

// Implementations for sub and vsyn
void SettingManager::setSub(int value) {
    sub = value;
}

int SettingManager::getSub() const {
    return sub;
}

void SettingManager::setVsyn(int value) {
    vsyn = value;
}

int SettingManager::getVsyn() const {
    return vsyn;
}

float SettingManager::getGameplayVol() const {
    return gameplayVol;
}
void SettingManager::setGameplayVol(float vol) {
    gameplayVol = vol;
}