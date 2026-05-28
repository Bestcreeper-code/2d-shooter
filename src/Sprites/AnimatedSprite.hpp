#pragma once
#include "raylib.h"
#include <vector>

class AnimatedSprite {
public:
    float   frameDuration = 0.1f;
    float   scale         = 1.0f;
    Color   tint          = WHITE;
    Vector2 origin        = {};
    bool    flipH         = false;
    bool    looping       = true;

    void AddFrame(Texture2D tex) {
        m_frames.push_back(tex);
    }

    Texture2D GetFrame(int index) const {
        if (index < 0 || index >= (int)m_frames.size()) return {};
        return m_frames[index];
    }

    void Update(float dt) {
        if (m_frames.empty()) return;

        m_timer += dt;
        if (m_timer >= frameDuration) {
            m_timer -= frameDuration;
            if (m_currentFrame < (int)m_frames.size() - 1) {
                ++m_currentFrame;
            } else if (looping) {
                m_currentFrame = 0;
            }
        }
    }

    void Draw(Vector2 position, float rotation = 0.0f) const {
        if (m_frames.empty()) return;

        const Texture2D& tex = m_frames[m_currentFrame];
        Rectangle src = { 0, 0, flipH ? -(float)tex.width : (float)tex.width, (float)tex.height };
        Rectangle dst = { position.x, position.y, tex.width * scale, tex.height * scale };

        DrawTexturePro(tex, src, dst, origin, rotation, tint);
    }

    bool IsFinished() const {
        return !looping && m_currentFrame == (int)m_frames.size() - 1;
    }

    void Reset() {
        m_currentFrame = 0;
        m_timer        = 0.0f;
    }

    void Unload() {
        for (auto& t : m_frames) UnloadTexture(t);
        m_frames.clear();
    }

private:
    std::vector<Texture2D> m_frames;
    int   m_currentFrame = 0;
    float m_timer        = 0.0f;
};