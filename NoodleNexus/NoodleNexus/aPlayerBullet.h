#pragma once
#include "sObject.h"
#include "Action.h"
#include "LabAttackFactory.h"
#include "aBullet.h"

class aPlayerBullet : public aBullet {
private:
    float lifetime = 0.0f;
    const float maxLifetime = 3.0f; // Self-destruct after 3 seconds

public:
    LabAttackFactory* factory = nullptr;

    virtual void Start() {
        lifetime = 0.0f; // Reset timer on spawn
    }

    virtual void Update() override {
        // 1. Handle enemy collisions
        aBullet::Update();
        std::vector<BruteEnemy*>::iterator it = factory->m_creepPool.begin();
        while (it != factory->m_creepPool.end()) {
            BruteEnemy* enemy = *it;
            float distance = glm::distance(enemy->object->mesh->positionXYZ, object->mesh->positionXYZ);

            if (distance < 2.0f) {

                enemy->Damage(2);
                object->Destroy(); // Destroy bullet on hit
                return; // Exit early since bullet is destroyed
            }
            else {
                ++it;
            }
        }


    }
};