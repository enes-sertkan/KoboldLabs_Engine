#pragma once

#include "Action.h"
#include "Scene.hpp"
#include "aScoreManager.h"


class aHammerAction : public Action
{
public:
    float effectRange = 7.5f;
    bool isOn = true;
    aScoreManager* scoreManager;

    void Update() override {
        if (!isOn) return;

        for (Object* target : object->scene->sceneObjects) {
            if (target->name == "Barrel" || target->name == "Flame_Enemy" || target->name == "BlueBarrel") {
                if (glm::distance(object->mesh->positionXYZ, target->mesh->positionXYZ) <= effectRange) {
                    target->Destroy();
                    if (scoreManager)
                        scoreManager->DrawScore(target->mesh->positionXYZ);

                }
            }
        }
    }
};