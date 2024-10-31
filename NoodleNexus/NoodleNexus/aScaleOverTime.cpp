#include "aScaleOverTime.hpp"

void aScaleOverTime::Update() {
    animationTime += deltaTime;

    if (animationTime < duration) {
        object->mesh->uniformScale = glm::mix(initialScale, targetScale, animationTime / duration);
    }
    else {
        object->isDestroyed = true;  // Mark for deletion after animation
    }
}