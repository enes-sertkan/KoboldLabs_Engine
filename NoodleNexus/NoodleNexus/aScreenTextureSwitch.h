#pragma once
#include "Action.h"
#include <vector>
#include <string>
#include "sObject.h"
class Object;

class ScreenTextureSwitch : public Action {
public:
    std::vector<std::string> cameraTextures;


     void AddTexture(std::string texture)
    {
        cameraTextures.push_back(texture);

    }


    virtual  void Start() {
       
        // Initialize first texture
        if (!cameraTextures.empty()) {
            currentTextureIndex = 0;
            object->mesh->textures[0] = cameraTextures[currentTextureIndex];
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
            object->mesh->textures[0] = cameraTextures[currentTextureIndex];
            
        }


    }

    std::string GetCurrentTexture() const {
        if (cameraTextures.empty()) return "";
        return cameraTextures[currentTextureIndex];
    }



private:
  // List of camera view textures
    float switchInterval=3.5;  // Time in seconds before switching views
    float timeSinceLastSwitch=0;
    int currentTextureIndex=0;


\
};