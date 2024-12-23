#include "MusicManager.h"
#include "audio/include/AudioEngine.h"

using namespace cocos2d;

MusicManager* MusicManager::instance = nullptr;

// Constructor
MusicManager::MusicManager() : currentMusicId(-1) {}

// Singleton instance
MusicManager* MusicManager::getInstance() {
    if (!instance) {
        instance = new MusicManager();
    }
    return instance;
}

// Phát nhạc nền
void MusicManager::playBackgroundMusic(const std::string& filePath, bool loop) {
    if (currentMusicId != -1) {
        AudioEngine::stop(currentMusicId);
    }
    currentMusicId = AudioEngine::play2d(filePath, loop);
}

// Dừng nhạc nền
void MusicManager::stopBackgroundMusic() {
    if (currentMusicId != -1) {
        AudioEngine::stop(currentMusicId);
        currentMusicId = -1;
    }
}

// Thiết lập âm lượng cho nhạc nền
void MusicManager::setVolume(float volume) {
    if (currentMusicId != -1) {
        AudioEngine::setVolume(currentMusicId, volume);
    }
}

// Phát effect âm thanh
int MusicManager::playEffect(const std::string& filePath, bool loop) {
    int soundId = AudioEngine::play2d(filePath, loop);
    soundEffectIds[filePath] = soundId;
    return soundId;
}

// Dừng một effect
void MusicManager::stopEffect(int soundId) {
    AudioEngine::stop(soundId);
}

// Dừng tất cả các effect
void MusicManager::stopAllEffects() {
    for (const auto& effect : soundEffectIds) {
        AudioEngine::stop(effect.second);
    }
    soundEffectIds.clear();
}

// Thiết lập âm lượng cho một effect
void MusicManager::setEffectVolume(int soundId, float volume) {
    AudioEngine::setVolume(soundId, volume);
}

// Giải phóng tài nguyên
void MusicManager::release() {
    AudioEngine::end();
    if (instance) {
        delete instance;
        instance = nullptr;
    }
}


// Sound
void MusicManager::soundTing() {
    Setting settingInit;
    settingInit.loadSettingData();
    AudioEngine::setVolume(AudioEngine::play2d("sound/ting.mp3"), settingInit.getGameplayVol());

}

void MusicManager::enemyDie() {
    Setting settingInit;
    settingInit.loadSettingData();
    AudioEngine::setVolume(AudioEngine::play2d("Enemy/warrior/die.mp3"), settingInit.getGameplayVol());
}

void MusicManager::enemyHurt() {
    Setting settingInit;
    settingInit.loadSettingData();
    AudioEngine::setVolume(AudioEngine::play2d("sound/enemyhurt.mp3"), settingInit.getGameplayVol());
}

void MusicManager::playerJump() {
    Setting settingInit;
    settingInit.loadSettingData();
    AudioEngine::setVolume(AudioEngine::play2d("player/sound/jump.mp3"), settingInit.getGameplayVol());
}

void MusicManager::playerSlash() {
    Setting settingInit;
    settingInit.loadSettingData();
    AudioEngine::setVolume(AudioEngine::play2d("player/sound/slash.mp3"), settingInit.getGameplayVol());
}

void MusicManager::chest() {
    Setting settingInit;
    settingInit.loadSettingData();
    AudioEngine::setVolume(AudioEngine::play2d("sound/chest.mp3"), settingInit.getGameplayVol());

}

void MusicManager::soundHealth() {
    Setting settingInit;
    settingInit.loadSettingData();
    AudioEngine::setVolume(AudioEngine::play2d("sound/health.mp3"), 0.5 * settingInit.getGameplayVol());
}
void MusicManager::soundError() {
    Setting settingInit;
    settingInit.loadSettingData();
    AudioEngine::setVolume(AudioEngine::play2d("sound/error.mp3"), 0.5 * settingInit.getGameplayVol());
}

void MusicManager::soundLaughter() {
    Setting settingInit;
    settingInit.loadSettingData();
    AudioEngine::setVolume(AudioEngine::play2d("Enemy/Bossmap1/sound/laughter.mp3"), 0.08 * settingInit.getGameplayVol());
}
void MusicManager::soundCreepyLaughter() {
    Setting settingInit;
    settingInit.loadSettingData();
    AudioEngine::setVolume(AudioEngine::play2d("Enemy/Bossmap2/sound/creepy-laughter.mp3"), 2 * settingInit.getGameplayVol());
}

void MusicManager::soundBoss1Hurt() {
    Setting settingInit;
    settingInit.loadSettingData();
    AudioEngine::setVolume(AudioEngine::play2d("Enemy/Bossmap1/sound/boss1hurt.mp3"), 0.5 * settingInit.getGameplayVol());
}

void MusicManager::soundBoss3Hurt() {
    Setting settingInit;
    settingInit.loadSettingData();
    AudioEngine::setVolume(AudioEngine::play2d("Enemy/Bossmap3/sound/hurt.mp3"), 0.5 * settingInit.getGameplayVol());
}

void MusicManager::soundMagicFire() {
    Setting settingInit;
    settingInit.loadSettingData();
    AudioEngine::setVolume(AudioEngine::play2d("Enemy/Bossmap1/sound/magic-fire.mp3"), 0.5 * settingInit.getGameplayVol());
}

void MusicManager::soundDarkMagic() {
    Setting settingInit;
    settingInit.loadSettingData();
    AudioEngine::setVolume(AudioEngine::play2d("Enemy/Bossmap2/sound/dark-magic.mp3"), 1 * settingInit.getGameplayVol());
}

void MusicManager::playerHurt() {
    Setting settingInit;
    settingInit.loadSettingData();
    AudioEngine::setVolume(AudioEngine::play2d("player/sound/hurt.mp3"), 0.1 * settingInit.getGameplayVol());
}
