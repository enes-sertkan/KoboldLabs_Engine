#pragma once
#include "sObject.h"
#include "Action.h"
#include "LabAttackFactory.h"

class aBullet : public Action {
private:
    float lifetime = 0.0f;
    const float maxLifetime = 3.0f; // Self-destruct after 3 seconds

public:
    LabAttackFactory* factory = nullptr;

    virtual void Start() {
        lifetime = 0.0f; // Reset timer on spawn
    }

    virtual void Update() {
        // 1. Handle enemy collisions
        std::vector<Object*>::iterator it = factory->m_creepPool.begin();
        while (it != factory->m_creepPool.end()) {
            Object* enemy = *it;
            float distance = glm::distance(enemy->mesh->positionXYZ, object->mesh->positionXYZ);

            if (distance < 2.0f) {
                it = factory->m_creepPool.erase(it);
                enemy->Destroy();
                object->Destroy(); // Destroy bullet on hit
                return; // Exit early since bullet is destroyed
            }
            else {
                ++it;
            }
        }

        // 2. Lifetime tracking
        lifetime += object->scene->deltaTime;
        if (lifetime >= maxLifetime) {
            object->Destroy(); // Self-destruct after timeout
        }
    }
};