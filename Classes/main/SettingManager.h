#ifndef SETTING_MANAGER_H
#define SETTING_MANAGER_H

class SettingManager {
private:
    // Singleton instance
    static SettingManager* instance;

    // Private constructor để ngăn việc tạo trực tiếp instance
    SettingManager();
    ~SettingManager();

    // Các thuộc tính
    float volume;
    float gameplayVol;
    int sub;
    int vsyn;
    int bgMusicId;

public:
    // Static method để lấy instance duy nhất
    static SettingManager* getInstance();

    // Phương thức để giải phóng instance
    static void destroyInstance();

    // Getter và setter
    float getVolume() const;
    void setVolume(float vol);

    float getGameplayVol() const;
    void setGameplayVol(float vol);

    float getBgMusicId() const;
    void setBgMusicId(int id);

    void setSub(int value);
    int getSub() const;

    void setVsyn(int value);
    int getVsyn() const;

    // Phương thức để lưu và tải cài đặt
    void saveSettingData();
    void loadSettingData();
};

#endif // SETTING_MANAGER_H
