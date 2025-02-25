#pragma once
#include "Action.h"
#include <vector>
#include <string>
#include <cmath>  // For sin(), abs()
#include <cstdlib> // For rand()

class aCameraSwitch : public Action {
public:
    aCameraSwitch(Object* obj, std::vector<std::string> cameraTextures, float switchInterval)
        : cameraTextures(cameraTextures), switchInterval(switchInterval), timeSinceLastSwitch(0.0f), currentTextureIndex(0) {
        this->object = obj;
        flickerOffset = 0.0f;
        staticIntensity = 0.0f;
    }

    virtual ~aCameraSwitch() {}

    void Start() override {
        // Initialize first texture
        if (!cameraTextures.empty()) {
            currentTextureIndex = 0;
        }
    }

    void Update() override {
        float deltaTime = 0.016f; // Assuming ~60 FPS (should be passed in)

        // Update timer
        timeSinceLastSwitch += deltaTime;

        // Switch textures every few seconds
        if (timeSinceLastSwitch >= switchInterval && !cameraTextures.empty()) {
            timeSinceLastSwitch = 0.0f;
            currentTextureIndex = (currentTextureIndex + 1) % cameraTextures.size();
        }

        // Apply flicker & static effects
        ApplyScreenEffects();
    }

    std::string GetCurrentTexture() const {
        if (cameraTextures.empty()) return "";
        return cameraTextures[currentTextureIndex];
    }

    float GetFlickerOffset() const {
        return flickerOffset;
    }

    float GetStaticIntensity() const {
        return staticIntensity;
    }

private:
    std::vector<std::string> cameraTextures; // List of camera view textures
    float switchInterval;  // Time in seconds before switching views
    float timeSinceLastSwitch;
    int currentTextureIndex;

    // Flicker & static effect parameters
    float flickerOffset;
    float staticIntensity;

    void ApplyScreenEffects() {
        // Random flicker effect (moves UV slightly)
        flickerOffset = (sin(timeSinceLastSwitch * 2.0f) * 0.3f) + 0.7f;

        // Random static intensity
        staticIntensity = abs(sin(timeSinceLastSwitch * 1.5f)) * 0.5f;
    }
};
