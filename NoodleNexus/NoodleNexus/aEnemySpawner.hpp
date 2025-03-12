#pragma once
#include "Action.h"
#include "sObject.h"
#include <glm/vec3.hpp> 
#include "cBasicFlyCamera/cBasicFlyCamera.h"
#include <glm/glm.hpp>

class EnemySpawner : public Action {
private:
    float spawnInterval = 5.0f; // Spawn enemies every 5 seconds
    float spawnTimer = 0.0f;
    int waveSize = 6; // Number of enemies per wave
    glm::vec3 spawnAreaMin = glm::vec3(-10.0f, 0.0f, -10.0f); // Min bounds of spawn area
    glm::vec3 spawnAreaMax = glm::vec3(10.0f, 0.0f, 10.0f); // Max bounds of spawn area

public:
    void Update() override {
        spawnTimer -= object->scene->deltaTime;

        if (spawnTimer <= 0) {
            SpawnWave();
            spawnTimer = spawnInterval; // Reset the timer
        }
    }

    void SpawnWave() {
        for (int i = 0; i < waveSize; i++) {
            SpawnEnemy();
        }
    }

    void SpawnEnemy() {
        // Randomize spawn position within the spawn area
        glm::vec3 spawnPosition = glm::vec3(
            spawnAreaMin.x + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (spawnAreaMax.x - spawnAreaMin.x))),
            spawnAreaMin.y,
            spawnAreaMin.z + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (spawnAreaMax.z - spawnAreaMin.z)))
        );

        // Create a new enemy object
        Object* enemy = object->scene->GenerateMeshObjectsFromObject("assets/models/EnemyModel.ply", spawnPosition, 1.0f, glm::vec3(0.0f, 0.0f, 0.0f), false, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), true, object->scene->sceneObjects);

        // Attach enemy AI to the enemy object
        aEnemyAI* enemyAI = new aEnemyAI();
        object->scene->AddActionToObj(enemyAI, enemy);
    }
};