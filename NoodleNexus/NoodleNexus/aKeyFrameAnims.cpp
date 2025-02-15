#include "aKeyFrameAnims.hpp"

void aKeyframeAnimation::update(float deltaTime, float timeScale) {
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
            else {
                return;
            }
        }
        return;
    }

    t = std::max(0.0f, std::min(1.0f, t));
    t = keyframes[currentKeyframeIndex].easingFunction(t);

    currentPosition = lerp(keyframes[currentKeyframeIndex].position, keyframes[currentKeyframeIndex + 1].position, t);
    currentRotation = slerp(keyframes[currentKeyframeIndex].rotation, keyframes[currentKeyframeIndex + 1].rotation, t);
}


