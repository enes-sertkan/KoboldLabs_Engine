#include "aKeyFrameAnims.hpp"
#include <algorithm>
#include "Scene.hpp"

Object* object = new Object();

void aKeyframeAnimation::update(float deltaTime, float timeScale) {
    std::cout << "Updating animation for object: " << object->mesh->uniqueFriendlyName << std::endl;
    if (keyframes.empty()) return;

    deltaTime *= timeScale;

    // Handle loop
    if (currentKeyframeIndex >= keyframes.size() - 1) {
        if (loop) {
            currentKeyframeIndex = 0;
            currentTime = 0.0f;
        }
        else {
            currentPosition = keyframes.back().position;
            currentRotation = keyframes.back().rotation;
            return;
        }
    }

    currentTime += deltaTime;
    float frameDuration = keyframes[currentKeyframeIndex + 1].time - keyframes[currentKeyframeIndex].time;

    if (frameDuration <= 0) return;

    float t = (currentTime - keyframes[currentKeyframeIndex].time) / frameDuration;

    if (t >= 1.0f) {
        t = 1.0f;
        currentPosition = keyframes[currentKeyframeIndex + 1].position;
        currentRotation = keyframes[currentKeyframeIndex + 1].rotation;
        currentKeyframeIndex++;
        if (currentKeyframeIndex >= keyframes.size() - 1) {
            if (loop) {
                currentKeyframeIndex = 0;
                currentTime = 0.0f;
            }
            return;
        }
    }

    t = std::clamp(t, 0.0f, 1.0f);
    t = keyframes[currentKeyframeIndex].easingFunction(t);

    // Interpolate position and rotation
    currentPosition = glm::mix(keyframes[currentKeyframeIndex].position, keyframes[currentKeyframeIndex + 1].position, t);
    currentRotation = glm::mix(keyframes[currentKeyframeIndex].rotation, keyframes[currentKeyframeIndex + 1].rotation, t);

    std::cout << "Current Position: (" << currentPosition.x << ", " << currentPosition.y << ", " << currentPosition.z << ")\n";
    std::cout << "Current Rotation: (" << currentRotation.x << ", " << currentRotation.y << ", " << currentRotation.z << ")\n";
}
