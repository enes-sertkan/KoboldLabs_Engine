#pragma once
#include "sObject.h"
#include "Action.h"
#include "LabAttackFactory.h"
#include "aBullet.h"

class aEnemyBullet : public aBullet {
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
       

    }
};