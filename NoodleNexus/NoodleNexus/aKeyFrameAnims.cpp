#include "aKeyFrameAnims.hpp"
#include "Scene.hpp"

void aKeyframeAnimation::Update() {
    if (keyframes.empty() || !isPlaying || !object) return;

    // Get deltaTime from the scene or another source
    float deltaTime = 0.016f; // Example: 60 FPS
    deltaTime *= timeScale;

    // Handle loop
    if (reversePlay) {
        currentTime -= deltaTime;
        if (currentTime <= 0.0f) {
            if (loop) {
                currentKeyframeIndex = keyframes.size() - 1;
                currentTime = keyframes.back().time;
            }
            else {
                currentTime = 0.0f;
                currentPosition = keyframes.front().position;
                currentRotation = keyframes.front().rotation;
                currentScale.x = keyframes.front().scale.x;

                return;
            }
        }
    }
    else {
        currentTime += deltaTime;
        if (currentTime >= keyframes.back().time) {
            if (loop) {
                currentKeyframeIndex = 0;
                currentTime = 0.0f;
            }
            else {
                currentTime = keyframes.back().time;
                currentPosition = keyframes.back().position;
                currentRotation = keyframes.back().rotation;
                currentScale.x = keyframes.back().scale.x;
                return;
            }
        }
    }

    // Binary search or optimized search for the correct keyframe
    if (reversePlay) {
        while (currentKeyframeIndex > 0 && keyframes[currentKeyframeIndex].time > currentTime) {
            currentKeyframeIndex--;
        }
    }
    else {
        while (currentKeyframeIndex < keyframes.size() - 1 && keyframes[currentKeyframeIndex + 1].time <= currentTime) {
            currentKeyframeIndex++;
        }
    }

    float t = (currentTime - keyframes[currentKeyframeIndex].time) / (keyframes[currentKeyframeIndex + 1].time - keyframes[currentKeyframeIndex].time);
 //   t = std::clamp(t, 0.0f, 1.0f);
    t = keyframes[currentKeyframeIndex].easingFunction(t);

    // Interpolate position and rotation
    currentPosition = glm::mix(keyframes[currentKeyframeIndex].position, keyframes[currentKeyframeIndex + 1].position, t);
    currentRotation = glm::mix(keyframes[currentKeyframeIndex].rotation, keyframes[currentKeyframeIndex + 1].rotation, t);
    currentScale = glm::mix(keyframes[currentKeyframeIndex].scale, keyframes[currentKeyframeIndex + 1].scale, t);

    // Update object's position and rotation if there's a significant change
    if (currentPosition != object->mesh->positionXYZ || currentRotation != object->mesh->rotationEulerXYZ || currentScale.x != object->mesh->uniformScale) {
        object->mesh->positionXYZ = currentPosition;
        object->mesh->rotationEulerXYZ = currentRotation;

        std::cout << "Setting scale to " << currentScale.x << std::endl;
        object->mesh->uniformScale = currentScale.x;
    }
}


void aKeyframeAnimation::NextSequence() {
    if (currentKeyframeIndex < keyframes.size() - 1) {
        currentKeyframeIndex++;
        currentTime = keyframes[currentKeyframeIndex].time;
        std::cout << "Next Sequence: Keyframe " << currentKeyframeIndex << std::endl;
    }
    else {
        std::cout << "Already at the last keyframe!" << std::endl;
    }
}

void aKeyframeAnimation::PreviousSequence() {
    if (currentKeyframeIndex > 0) {
        currentKeyframeIndex--;
        currentTime = keyframes[currentKeyframeIndex].time;
        std::cout << "Previous Sequence: Keyframe " << currentKeyframeIndex << std::endl;
    }
    else {
        std::cout << "Already at the first keyframe!" << std::endl;
    }
}

void aKeyframeAnimation::SetSpeed(float speed) {
    timeScale = speed;
    std::cout << "Speed set to: " << timeScale << "x" << std::endl;
}

void aKeyframeAnimation::TogglePause() {
    isPlaying = !isPlaying;
    std::cout << (isPlaying ? "Animation Resumed" : "Animation Paused") << std::endl;
}

void aKeyframeAnimation::ReversePlay() {
    reversePlay = !reversePlay;
    std::cout << (reversePlay ? "Reverse play started" : "Reverse play stopped") << std::endl;
}
