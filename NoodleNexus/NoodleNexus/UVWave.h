#pragma once
#include <glm/vec2.hpp>

struct UVWave {
    glm::vec2 uv = glm::vec2(0.5f);       // The UV coordinate for the wave
    bool active=false;        // Whether the wave is active
    float time;         // The current time (or phase) of the wave
};