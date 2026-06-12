#include "WaveDecoder.hpp"
#include "Entities/Enemies/EnemySpawner.hpp"
#include "cJSON.h"
#include <filesystem>
#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>

static Formation ParseFormation(cJSON* json) {
    Formation f;
    if (!cJSON_IsArray(json)) return f;

    cJSON* item;
    cJSON_ArrayForEach(item, json) {
        cJSON* type = cJSON_GetObjectItem(item, "type");
        cJSON* xOffset = cJSON_GetObjectItem(item, "xOffset");
        cJSON* delay = cJSON_GetObjectItem(item, "delay");
        cJSON* healthMult = cJSON_GetObjectItem(item, "healthMult");
        cJSON* speedMult = cJSON_GetObjectItem(item, "speedMult");
        
        cJSON* attrs = cJSON_GetObjectItem(item, "attrs");


        if (!cJSON_IsNumber(type) || !cJSON_IsNumber(xOffset) || !cJSON_IsNumber(delay))
            continue;

        FormationEnemy fe;
        fe.type = (EnemyType)type->valueint;
        fe.xOffset = (float)xOffset->valuedouble;
        fe.delay = (float)delay->valuedouble;
        fe.healthMult = healthMult && cJSON_IsNumber(healthMult) ? (float)healthMult->valuedouble : 1.f;
        fe.speedMult = speedMult  && cJSON_IsNumber(speedMult)  ? (float)speedMult->valuedouble  : 1.f;
        
        if (cJSON_IsObject(attrs)) {
            cJSON* attr;
            cJSON_ArrayForEach(attr, attrs) {
                if (cJSON_IsNumber(attr))
                fe.attrs[attr->string] = (float)attr->valuedouble;
            }   
        }

        f.enemies.push_back(fe);
    }
    return f;
}

static WaveEntry ParseEntry(cJSON* json) {
    WaveEntry e;

    cJSON* kind = cJSON_GetObjectItem(json, "kind");
    cJSON* formation = cJSON_GetObjectItem(json, "formation");
    cJSON* weight = cJSON_GetObjectItem(json, "weight");
    cJSON* delayAfter = cJSON_GetObjectItem(json, "delayAfter");

    if (cJSON_IsString(kind) && std::string(kind->valuestring) == "pooled")
        e.type = WaveEntryType::Pooled;
    else
        e.type = WaveEntryType::Forced;

    e.formation  = ParseFormation(formation);
    e.weight     = weight     && cJSON_IsNumber(weight)     ? weight->valueint              : 1;
    e.delayAfter = delayAfter && cJSON_IsNumber(delayAfter) ? (float)delayAfter->valuedouble : 0.f;

    return e;
}

std::vector<WaveDef> DecodeWavesJSON(const std::string& path) {
    std::vector<WaveDef> result;

    auto size = std::filesystem::file_size(path);
    std::string data(size, '\0');
    std::ifstream stream(path);
    if (!stream) return {};
    stream.read(&data[0], size);

    cJSON* root = cJSON_Parse(data.c_str());
    if (!root) return {};

    cJSON* waves = cJSON_GetObjectItem(root, "waves");
    cJSON* wave;
    cJSON_ArrayForEach(wave, waves) {
        WaveDef wDef;
        cJSON* entries = cJSON_GetObjectItem(wave, "entries");
        if (!cJSON_IsArray(entries)) continue;

        cJSON* entry;
        cJSON_ArrayForEach(entry, entries) {
            wDef.entries.push_back(ParseEntry(entry));
        }
        result.push_back(std::move(wDef));
    }

    cJSON_Delete(root);
    return result;
}

namespace WaveCache {
    static std::unordered_map<std::string, std::vector<WaveDef>> cache;

    const std::vector<WaveDef>& GetWaves(const std::string& path) {
        auto it = cache.find(path);
        if (it != cache.end()) return it->second;
        auto [itNew, inserted] = cache.emplace(path, DecodeWavesJSON(path));
        return itNew->second;
    }
}