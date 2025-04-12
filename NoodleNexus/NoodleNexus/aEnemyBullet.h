#pragma once
#include "sObject.h"
#include "Action.h"
#include "LabAttackFactory.h"
#include "aBullet.h"
#include "aPlayerCore.h"

class aEnemyBullet : public aBullet {
private:
    float lifetime = 0.0f;
    const float maxLifetime = 3.0f; // Self-destruct after 3 seconds

public:

    virtual void Start() {
        lifetime = 0.0f; // Reset timer on spawn
    }
    virtual void Update() override {
        // 1. Handle enemy collisions
        if (!object) return;
        // 1. Handle enemy collisions
        aBullet::Update();
        if (!object) return;
        if (!factory) return;

        aPlayerCore* playerCore = factory->playerCore;

        float distance = glm::distance(playerCore->object->mesh->positionXYZ + playerCore->colliderCenter, object->mesh->positionXYZ);

        if (distance < playerCore->colliderRadius) {

            playerCore->Damage(damage) ;

            DestroyBullet();// Destroy bullet on hit
            return; // Exit early since bullet is destroyed
        }
    }
};