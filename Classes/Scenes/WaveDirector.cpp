#include <Scenes/WaveDirector.h>

#include <Scenes/GameScene.h>

void WaveDirector::init(GameScene *pGameScene) {
    mGameScene = pGameScene;

    mCleared = false;

    mWaves.clear();
    mWaves.push_back(std::vector<CreepTypes>{SPEEDY, RAPTOR, PULSAR, PANZER});
    mWaves.push_back(std::vector<CreepTypes>{RAPTOR});
    mWaves.push_back(std::vector<CreepTypes>{RAPTOR, RAPTOR, RAPTOR});
    mWaves.push_back(std::vector<CreepTypes>{SPEEDY, SPEEDY, RAPTOR, RAPTOR, RAPTOR});
    mWaves.push_back(std::vector<CreepTypes>{RAPTOR, RAPTOR, PULSAR, PULSAR});
    mWaves.push_back(std::vector<CreepTypes>{RAPTOR, RAPTOR, RAPTOR, SPEEDY, SPEEDY, PULSAR, PANZER});
    mWaves.push_back(std::vector<CreepTypes>(7, SPEEDY));
    mWaves.push_back(std::vector<CreepTypes>{PULSAR, PULSAR, PULSAR, PULSAR, PULSAR, PULSAR, PANZER});
    mWaves.push_back(std::vector<CreepTypes>{10, PULSAR});
    mWaves.push_back(std::vector<CreepTypes>{SPEEDY, SPEEDY, SPEEDY, SPEEDY, RAPTOR, RAPTOR, RAPTOR, RAPTOR, PULSAR,
                                             PULSAR, PANZER, PANZER});
    mWaves.push_back(std::vector<CreepTypes>{15, PANZER});

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
