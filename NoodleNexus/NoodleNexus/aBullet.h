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
       
        // 2. Lifetime tracking
        lifetime += object->scene->deltaTime;
        if (lifetime >= maxLifetime) {
            object->Destroy(); // Self-destruct after timeout
        }
    }
};