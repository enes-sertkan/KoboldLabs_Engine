#pragma once

#include "Action.h";
#include <glm/vec3.hpp> 
#include "sObject.h"
#include "Scene.hpp"
#include <cmath>

class aTextureWiggler : public Action
{
public:
    float duration = 4; // Total time for one oscillation cycle
    bool affectedTextures[4] = { 0, 1, 0, 0 };
    float maxBlend = 5.0f;
    float minBlend = 0.0f;
    float elapsedTime = 0.0f;

    void Update() override
    {
        // Ensure `duratiom` is not zero to avoid division by zero
        if (duration <= 0)
            return;

        // Loop through the affected textures
        for (int i = 0; i < 4; i++)
        {
            if (affectedTextures[i])
            {
                // Access the blend ratio and change it
                float& blendRatio = object->mesh->blendRatio[i];
                float deltaBlend = (maxBlend - minBlend) / duration * object->scene->deltaTime;

                // Update the blend ratio, reversing direction if it hits min or max
                static bool increasing = true; // Track direction of change (static to persist between calls)

                if (increasing)
                {
                    blendRatio += deltaBlend;
                    if (blendRatio >= maxBlend)
                    {
                        blendRatio = maxBlend;
                        increasing = false;
                    }
                }
                else
                {
                    blendRatio -= deltaBlend;
                    if (blendRatio <= minBlend)
                    {
                        blendRatio = minBlend;
                        increasing = true;
                    }
                }
            }
        }
    }
};