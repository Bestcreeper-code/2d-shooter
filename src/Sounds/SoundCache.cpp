#include "Textures/TextureCache.hpp"
#include "raylib.h"
#include <string>
#include <unordered_map>

namespace SoundCache{

std::unordered_map<std::string, Sound> soundCache;

const Sound& GetSound(const std::string& path) {
    auto it = soundCache.find(path);
    if (it != soundCache.end()) {
        return it->second;
    }
    
    Sound snd = LoadSound(path.c_str());
    auto [inserted, ok] = soundCache.emplace(path, snd);
    return inserted->second;
}











}