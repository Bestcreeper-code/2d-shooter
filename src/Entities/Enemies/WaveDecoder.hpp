#pragma once

#include "Entities/Enemies/EnemySpawner.hpp"
#include <string>
#include <vector>



std::vector<WaveDef> DecodeWavesJSON(const std::string& path);

namespace WaveCache {

    const std::vector<WaveDef>& GetWaves(const std::string& path);
}


