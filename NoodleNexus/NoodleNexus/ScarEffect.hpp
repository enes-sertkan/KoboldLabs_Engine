#pragma once

#include "Action.h"

class ScarEffect : public Action {
public:
    void Start() override {
        object->mesh->objectColourRGBA = glm::vec4(0.2f, 0.2f, 0.2f, 1.0f); // Dark grey color
        object->mesh->uniformScale = 0.1f;  // Smaller scale for scar
    }

};

