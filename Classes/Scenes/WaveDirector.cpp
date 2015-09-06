#include <Scenes/WaveDirector.h>

#include <Scenes/GameScene.h>

void WaveDirector::init(GameScene *pGameScene) {
    mGameScene = pGameScene;

    mCleared = false;

    mWaves.clear();

    for (int i = 0; i < 5; i++)
        mWaves.push_back(std::vector<CreepTypes>(5, RAPTOR));

    mCurrentWave = 0;
}

void WaveDirector::update(float pDelta) {
    if (mGameScene->getCreepCount() <= 0) {
        if (!spawnNextWave())
            mCleared = true;
    }
}

bool WaveDirector::spawnNextWave() {
    if (mCurrentWave < mWaves.size()) {
        auto wave = mWaves.at(mCurrentWave);

        for (int i = 0; i < wave.size(); i++) {
            mGameScene->spawnEnemy(wave.at(i), i);
        }

        mCurrentWave++;

        return true;
    }

    return false;
}
