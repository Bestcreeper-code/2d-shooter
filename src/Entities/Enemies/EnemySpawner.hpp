#pragma once
#include "Object/Object.hpp"
#include "raylib.h"
#include <string>
#include <unordered_map>
#include <vector>

enum class EnemyType {
    EN_Basic,
    EN_Strafing
};


struct FormationEnemy {
    EnemyType type;
    float xOffset;
    float delay;
    float healthMult = 1.0f;
    float speedMult = 1.0f;
    std::unordered_map<std::string, float> attrs;
};



struct Formation {
    std::vector<FormationEnemy> enemies;
};



enum class WaveEntryType { Forced, Pooled };

struct WaveEntry {
    WaveEntryType type;
    Formation     formation;
    int           weight     = 1;
    float         delayAfter = 0.f;
};

struct WaveDef {
    std::vector<WaveEntry> entries;
};

struct PendingSpawn { FormationEnemy e;
    float timeLeft;
    float areaLeft;
    float areaWidth;
};


class WaveSpawner : public Actor {
    std::vector<WaveDef> waves;
    int   waveIndex  = -1;
    int   liveCount  = 0;

    
    std::vector<WaveEntry*> sequence;
    int   seqIndex   = 0;

    
    std::vector<PendingSpawn> pending;
    bool formationActive = false;
    float entryDelay     = 0.f;

    float spawnY, areaLeft, areaRight;

    void BuildSequence(const WaveDef& wave);
    void StartFormation(const WaveEntry& entry);
    void SpawnEnemy(const FormationEnemy& e, float anchorX);

public:
    WaveSpawner(float areaLeft, float areaRight, float spawnY);

    void AddWave(WaveDef wave);
    void LoadWave(int index);
    void StartWaves();
    void NextWave();

    bool AllWavesDone() { return waveIndex >= (int)waves.size(); }
    
    bool WaveCleared() {
        return !formationActive 
            && seqIndex >= (int)sequence.size() && liveCount == 0; 
    }

    void OnEnemyDied()  { if (liveCount > 0) liveCount--; }
    void Update(float dt) override;

    void SpawnEnemy(const FormationEnemy& fe, float areaLeft, float areaWidth);
};