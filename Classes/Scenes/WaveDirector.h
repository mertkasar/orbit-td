#ifndef WAVE_DIRECTOR_H
#define WAVE_DIRECTOR_H

#include <Globals.h>

#include <vector>

class GameScene;

class WaveDirector {
private:
    GameScene *mGameScene;

    std::vector<std::vector<CreepTypes>> mWaves;

    unsigned int mCurrentWave;

    bool mCleared;

public:
    void init(GameScene *pGameScene);

    void update(float pDelta);

    bool spawnNextWave();

    bool isCleared() const {
        return mCleared;
    }

    unsigned int getCurrentWave() const {
        return mCurrentWave;
    }

    unsigned int getTotalWaves() const {
        return mWaves.size();
    }
};

#endif //WAVE_DIRECTOR_H
