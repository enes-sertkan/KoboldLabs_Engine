#pragma once

#include "Action.h"
#include <glm/glm.hpp>

class aScaleOverTime : public Action {
public:
    aScaleOverTime(Object* obj, float targetScale, float duration)
        : object(obj), initialScale(obj->mesh->uniformScale), targetScale(targetScale), duration(duration), animationTime(0.0f) {}

    void Update();

private:
    Object* object;
    float initialScale; // Change from glm::vec3 to float
    float targetScale;  // Change from glm::vec3 to float
    float duration;
    float animationTime;
};
