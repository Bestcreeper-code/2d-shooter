#include "EnemySpawner.hpp"
#include "Entities/Enemies/Strafer/Strafer.hpp"
#include "Entities/Enemies/TestEnemy/TestEnemy.hpp"
#include "ObjectMgr/ObjectMgr.hpp"
#include "helpers.hpp"
#include <cstdio>
#include <cstdlib>
#include <algorithm>

WaveSpawner::WaveSpawner(float areaLeft, float areaRight, float spawnY)
    : spawnY(spawnY), areaLeft(areaLeft), areaRight(areaRight) {
    if (areaRight <= areaLeft)
        errorf("invalid spawn area: areaLeft=%.1f areaRight=%.1f", areaLeft, areaRight);
}

void WaveSpawner::AddWave(WaveDef wave) {
    if (wave.entries.empty())
        errorf("wave %zu has no entries", waves.size());
    waves.push_back(std::move(wave));
}

void WaveSpawner::BuildSequence(const WaveDef& wave) {
    sequence.clear();

    std::vector<WaveEntry*> pool;
    int totalWeight = 0;
    for (auto& e : const_cast<WaveDef&>(wave).entries) {
        if (e.type == WaveEntryType::Pooled) {
            if (e.weight <= 0)
                errorf("wave %d: pooled entry has weight %d, will be ignored", waveIndex, e.weight);
            else {
                pool.push_back(&e);
                totalWeight += e.weight;
            }
        }
    }

    auto samplePool = [&]() -> WaveEntry* {
        if (pool.empty()) return nullptr;
        int roll = rand() % totalWeight, acc = 0;
        for (auto* e : pool) {
            acc += e->weight;
            if (roll < acc) return e;
        }
        return pool.back();
    };

    int forcedCount = 0;
    for (auto& e : const_cast<WaveDef&>(wave).entries) {
        if (e.type != WaveEntryType::Forced) continue;
        if (e.formation.enemies.empty())
            errorf("wave %d: forced entry %d has empty formation, skipping", waveIndex, forcedCount);
        else {
            sequence.push_back(const_cast<WaveEntry*>(&e));
            if (auto* pooled = samplePool())
                sequence.push_back(pooled);
        }
        forcedCount++;
    }

    if (sequence.empty())
        errorf("wave %d: sequence is empty after build (no forced entries?)", waveIndex);
    else
        printf("wave %d: built sequence of %zu entries (%d forced, %zu pooled slots)\n",
               waveIndex, sequence.size(), forcedCount, sequence.size() - forcedCount);
}

void WaveSpawner::LoadWave(int index) {
    if (index < 0 || index >= (int)waves.size()) {
        errorf("LoadWave(%d) out of range (have %zu waves)", index, waves.size());
        waveIndex = index;
        return;
    }
    waveIndex = index;
    seqIndex = 0;
    liveCount = 0;
    pending.clear();
    formationActive = false;
    entryDelay = 0.f;
    printf("loading wave %d\n", waveIndex);
    BuildSequence(waves[waveIndex]);
}

void WaveSpawner::StartWaves() {
    if (waves.empty()) { errorf("StartWaves called with no waves loaded"); return; }
    LoadWave(0);
}
void WaveSpawner::NextWave() { 
    LoadWave(waveIndex + 1);
}

void WaveSpawner::StartFormation(const WaveEntry& entry) {
    if (entry.formation.enemies.empty()) {
        errorf("wave %d: StartFormation called on empty formation", waveIndex);
        return;
    }
    formationActive = true;
    float areaWidth = areaRight - areaLeft;
    for (auto& fe : entry.formation.enemies)
        pending.push_back({ fe, fe.delay, areaLeft, areaWidth });
}

void WaveSpawner::Update(float dt) {
    if (waveIndex < 0 || AllWavesDone()) return;

    for (auto& ps : pending) {
        ps.timeLeft -= dt;
        if (ps.timeLeft <= 0.f)
            SpawnEnemy(ps.e, ps.areaLeft, ps.areaWidth);
    }
    pending.erase(
        std::remove_if(pending.begin(), pending.end(),
            [](const PendingSpawn& ps){ return ps.timeLeft <= 0.f; }),
        pending.end()
    );

    if (!pending.empty()) return;

    if (entryDelay > 0.f) {
        entryDelay -= dt;
        return;
    }

    formationActive = false;

    if (seqIndex < (int)sequence.size()) {
        WaveEntry* entry = sequence[seqIndex++];
        entryDelay = entry->delayAfter;
        StartFormation(*entry);
    }
}

void WaveSpawner::SpawnEnemy(const FormationEnemy& fe, float areaLeft, float areaWidth) {
    Vector2 pos = { areaLeft + fe.xOffset * areaWidth, spawnY };
    switch (fe.type) {
        case EnemyType::EN_Basic: {
            auto* enemy = new TestEnemy(pos);
            enemy->max_health *= fe.healthMult;
            enemy->health = enemy->max_health;
            enemy->speed *= fe.speedMult;
            RegisterActor(enemy);
            liveCount++;
            break;
        }
        case EnemyType::EN_Strafing: {
            auto* enemy = new StraferEnemy(pos,100);
            enemy->max_health *= fe.healthMult;
            enemy->health = enemy->max_health;
            enemy->speed *= fe.speedMult;
            RegisterActor(enemy);
            enemy->strafe_y = strmap_get_or_default(fe.attrs, "target_y", 100.f);
            liveCount++;
            break;
        }
        default:
            errorf("unknown EnemyType %d", (int)fe.type);
            break;
    }
}