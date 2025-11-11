#ifndef GAMELEVELDATA_H
#define GAMELEVELDATA_H
#include <QObject>

#include <QSettings>
#include <QString>
#include <QVariant>

// 关卡数据结构体（方便传递）
struct LevelData {
    int levelId;         // 关卡编号
    bool isUnlocked;     // 是否解锁
    int highScore;       // 最高分
    bool isCleared;      // 是否通关
    double clearTime;    // 通关时间（秒）

    // 默认构造（初始化默认值）
    LevelData() : levelId(0), isUnlocked(false), highScore(0), isCleared(false), clearTime(0.0) {}
};

class GameLevelData {
public:
    // 单例模式（全局唯一实例，避免重复打开文件）
    static GameLevelData* instance();

    // 写入单个关卡数据
    void writeLevelData(const LevelData& data);

    // 读取单个关卡数据（不存在则返回默认值）
    LevelData readLevelData(int levelId);

    // 解锁指定关卡
    void unlockLevel(int levelId);

    // 更新关卡最高分（仅当新分数更高时更新）
    void updateHighScore(int levelId, int newScore);

    // 标记关卡为通关，并记录通关时间
    void markLevelCleared(int levelId, double clearTime);

private:
    // 私有构造（单例模式）
    GameLevelData();

    // 生成 INI 文件中对应的分组名（如 "Level1"）
    QString getLevelGroupName(int levelId) const;

    QSettings m_settings; // QSettings 实例（自动管理文件）

};


#endif // GAMELEVELDATA_H
