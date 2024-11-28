#pragma once

#include "Action.h"
#include "Scene.hpp"

class aHammerAction : public Action
{
public:
    float effectRange = 10.0f;
    bool isOn = true;

    void Update() override {
        if (!isOn) return;

        for (Object* target : object->scene->sceneObjects) {
            if (target->name == "Barrel" || target->name == "Flame_Enemy" || target->name == "BlueBarrel") {
                if (glm::distance(object->mesh->positionXYZ, target->mesh->positionXYZ) <= effectRange) {
                    target->Destroy();
                }
            }
        }
    }
};