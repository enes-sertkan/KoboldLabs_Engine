#pragma once
#include "Action.h"
#include "Scene.hpp"

class aUseItem : public Action {
public:
    float effectRange = 5.0f;
    int damage = 10;

    void Update() override {
        for (Object* target : object->scene->sceneObjects) {
            if (target->name == "Barrel" || target->name == "Flame_Enemy") {
                if (glm::distance(object->mesh->positionXYZ, target->mesh->positionXYZ) <= effectRange) {
                    target->TakeDamage(damage);
                }
            }
        }
    }
};

