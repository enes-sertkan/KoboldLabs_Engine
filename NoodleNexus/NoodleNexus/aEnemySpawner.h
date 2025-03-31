#pragma once
#include "sObject.h"
#include "Action.h"
#include "LabAttackFactory.h"

class aEnemySpawner : public Action {
private:
    float spawnInterval = 1.0f; // Seconds between spawns
    float timeSinceLastSpawn = 0.f;
    int maxEnemies = 10;        // Maximum concurrent enemies
    int currentEnemies = 0;

public:
    LabAttackFactory* factory = nullptr;

    virtual void Start() {
        timeSinceLastSpawn = spawnInterval; // Spawn first enemy immediately
    }

    virtual void Update() {
        if (!factory) return;
        float delta = object->scene->deltaTime;
            if (delta > 0.5)
                delta = 0.f;

        timeSinceLastSpawn += delta;
     
        if (timeSinceLastSpawn >= spawnInterval && factory->m_creepPool.size() < maxEnemies) {
            SpawnEnemy();
            timeSinceLastSpawn = 0.0f;
        }
    }

    void SpawnEnemy() {
        Object* enemy = factory->SpawnBrut(object->mesh->positionXYZ);
        if (enemy) {
            currentEnemies++;

            // Optional: Set up enemy death callback
        /*    enemy->OnDestroy = [this]() {
                currentEnemies--;
                };*/
        }
    }

    // Configuration methods
    void SetSpawnInterval(float interval) { spawnInterval = interval; }
    void SetMaxEnemies(int max) { maxEnemies = max; }
};