#pragma once
#include "sObject.h"
#include <glm/glm.hpp>
#include "Scene.hpp"
#include <GLFW/glfw3.h>

class Object;

class aWavesEffect : public Action
{
private:
    float waveSpawnInterval = 0.5f; // Time (seconds) between waves when moving
    float timeSinceLastWave = 0.0f; // Timer for wave spawning
    glm::vec3 lastPlayerPos = glm::vec3(0.0f); // To track player's previous position
    float movementThreshold = 0.01f; // Minimum distance to consider that the player is moving

public:
    glm::vec2 offset = glm::vec2(0);
    Object* player = nullptr;

    int colliderID = -1;

    virtual void Start() override
    {
        if (player)
        {
            lastPlayerPos = player->mesh->positionXYZ;
            colliderID = object->mesh->CreateCollider(player->mesh->positionXYZ, 5.f, 10.f);
        }
        timeSinceLastWave = waveSpawnInterval;
       

    }

    virtual void Update() override
    {
        if (!player) return;
        glm::vec3 currentPos = player->mesh->positionXYZ;


        glm::vec2 planeScale = glm::vec2(object->mesh->uniformScale * 1.91f, object->mesh->uniformScale * 1.9f);
        glm::vec2 uvPos = CalculateUV(currentPos, object->mesh->positionXYZ,
            glm::vec3(0, object->mesh->rotationEulerXYZ.y, 0),
            planeScale, offset);
        object->mesh->UpdateColliderPosition(colliderID, player->mesh->positionXYZ);



       
        float movedDist = glm::distance(glm::vec2(currentPos.x, currentPos.z), glm::vec2(lastPlayerPos.x, lastPlayerPos.z));
        bool isMoving = movedDist > movementThreshold;

        if (isMoving)
        {
            if (timeSinceLastWave <= 0.0f)
            {
                TrySpawnWave(currentPos);
                timeSinceLastWave = 0.001f; // Small positive value to indicate wave was spawned
            }
            else
            {
                timeSinceLastWave += object->scene->deltaTime;
                if (timeSinceLastWave >= waveSpawnInterval)
                {
                    TrySpawnWave(currentPos);
                    timeSinceLastWave = 0.0f; // Reset timer after spawning
                }
            }
        }
        else
        {
            timeSinceLastWave = 0.0f;
        }

        lastPlayerPos = currentPos;
    }

    void TrySpawnWave(const glm::vec3& worldPos)
    {
        glm::vec2 planeScale = glm::vec2(object->mesh->uniformScale * 2.f, object->mesh->uniformScale * 2.f);
        glm::vec2 uvPos = CalculateUV(worldPos, object->mesh->positionXYZ,
            glm::vec3(0, object->mesh->rotationEulerXYZ.y, 0),
            planeScale, offset);

        // Only spawn wave if UV coordinates are within valid range [0,1]
        if (uvPos.x >= 0.0f && uvPos.x <= 1.0f && uvPos.y >= 0.0f && uvPos.y <= 1.0f)
        {
            std::cout << "Wave spawned at UV: " << uvPos.x << " " << uvPos.y << std::endl;
            object->mesh->spawnWave(uvPos);
        }
    }

    glm::vec2 CalculateUV(glm::vec3 worldPos, glm::vec3 planePos, glm::vec3 planeRotation, glm::vec2 planeScale, glm::vec2 pivotOffset)
    {
        glm::vec3 localPos = worldPos - planePos;

        glm::mat4 rotationMatrix = glm::mat4(1.0f);
        rotationMatrix = glm::rotate(rotationMatrix, glm::radians(planeRotation.x), glm::vec3(1, 0, 0));
        rotationMatrix = glm::rotate(rotationMatrix, glm::radians(planeRotation.y), glm::vec3(0, 1, 0));
        rotationMatrix = glm::rotate(rotationMatrix, glm::radians(planeRotation.z), glm::vec3(0, 0, 1));

        glm::vec3 localRotatedPos = glm::vec3(glm::inverse(rotationMatrix) * glm::vec4(localPos, 1.0f));

        localRotatedPos.x -= pivotOffset.x * planeScale.x;
        localRotatedPos.z -= pivotOffset.y * planeScale.y;

        glm::vec2 uv;
        uv.x = (localRotatedPos.x / planeScale.x) + 0.5f;
        uv.y = -(localRotatedPos.z / planeScale.y) + 0.5f;

        return uv;
    }
};
