#pragma once
#include "sObject.h"
#include "Action.h"
#include "LabAttackFactory.h"

class aEnemySpawner : public Action {
public:
    enum class WaveState {
        SPAWNING,
        WAITING,
        COOLDOWN
    };

    struct EnemyWaveConfig {
        float baseAmount = 0.0f;
        float currentAmount = 0.0f;
        float amountIncrease = 0.0f;
        int toSpawnThisWave = 0;
        bool enabled = true;
    };

private:
    WaveState currentState = WaveState::COOLDOWN;
    float waveCooldown = 5.0f;
    float stateTimer = 0.0f;
    int currentWave = 0;

    EnemyWaveConfig type1Config;
    EnemyWaveConfig type2Config;

    int activeEnemies = 0;


public:
    LabAttackFactory* factory = nullptr;
    // Configuration interface
    void ConfigureType1(float start, float increase) {
        type1Config.baseAmount = start;
        type1Config.currentAmount = start;
        type1Config.amountIncrease = increase;
    }

    void ConfigureType2(float start, float increase) {
        type2Config.baseAmount = start;
        type2Config.currentAmount = start;
        type2Config.amountIncrease = increase;
    }

    virtual void Start() override {
        ConfigureType1(0, 0.125);
        ConfigureType2(1, 0.25);


        type1Config.currentAmount = type1Config.baseAmount;
        type2Config.currentAmount = type2Config.baseAmount;
        StartNextWave();
    }

    virtual void Update() override {
        if (!factory) return;
        float delta = glm::min(object->scene->deltaTime, 0.1f);
        activeEnemies = factory->m_creepPool.size();
        switch (currentState) {
        case WaveState::SPAWNING:
            UpdateSpawning(delta);
            break;
        case WaveState::WAITING:
            UpdateWaiting();
            break;
        case WaveState::COOLDOWN:
            UpdateCooldown(delta);
            break;
        }
    }

private:
    void StartNextWave() {
        currentWave++;

        // Update wave configs
        type1Config.currentAmount += type1Config.amountIncrease;
        type2Config.currentAmount += type2Config.amountIncrease;

        type1Config.toSpawnThisWave = static_cast<int>(type1Config.currentAmount);
        type2Config.toSpawnThisWave = static_cast<int>(type2Config.currentAmount);

        currentState = WaveState::SPAWNING;
        stateTimer = 0.0f;
    }

    void UpdateSpawning(float delta) {
        stateTimer += delta;

        // Spawn one enemy per second
        if (stateTimer >= 1.0f) {
            stateTimer = 0.0f;

            if (SpawnNextEnemy()) {
                if (type1Config.toSpawnThisWave <= 0 &&
                    type2Config.toSpawnThisWave <= 0) {
                    currentState = WaveState::WAITING;
                }
            }
        }
    }

    bool SpawnNextEnemy() {
        if (type1Config.toSpawnThisWave > 0) {
            SpawnEnemy(true);
            type1Config.toSpawnThisWave--;
            return true;
        }

        if (type2Config.toSpawnThisWave > 0) {
            SpawnEnemy(false);
            type2Config.toSpawnThisWave--;
            return true;
        }

        return false;
    }

    void SpawnEnemy(bool isType1) {
        Object* enemy = isType1
            ? factory->SpawnBrutShooterEnemy(object->mesh->positionXYZ)
            : factory->SpawnBrut(object->mesh->positionXYZ);

        if (enemy) {
         
        }
    }

    void UpdateWaiting() {
        if (activeEnemies <= 0) {
            currentState = WaveState::COOLDOWN;
            stateTimer = waveCooldown;
        }
    }

    void UpdateCooldown(float delta) {
        stateTimer -= delta;
        if (stateTimer <= 0.0f) {
            StartNextWave();
        }
    }
};