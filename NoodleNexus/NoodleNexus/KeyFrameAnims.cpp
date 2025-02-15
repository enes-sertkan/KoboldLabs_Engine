#include "aKeyFrameAnims.hpp"

// Update the animation
void Animation::update(float deltaTime) {
    if (!isPlaying) return;

    // Update current time
    if (isReversed) {
        currentTime -= deltaTime * speed;
        if (currentTime < 0) currentTime = duration;
    }
    else {
        currentTime += deltaTime * speed;
        if (currentTime > duration) currentTime = 0;
    }

    // Find previous and next keyframes
    KeyFrame prevFrame = keyFrames[0];
    KeyFrame nextFrame = keyFrames[0];
    for (size_t i = 1; i < keyFrames.size(); ++i) {
        if (keyFrames[i].time > currentTime) {
            nextFrame = keyFrames[i];
            prevFrame = keyFrames[i - 1];
            break;
        }
    }

    // Calculate interpolation factor (t)
    float t = (currentTime - prevFrame.time) / (nextFrame.time - prevFrame.time);
    applyEasing(t);

    // Interpolate position, scale, and rotation
    glm::vec3 position = glm::mix(prevFrame.position, nextFrame.position, t);
    glm::vec3 scale = glm::mix(prevFrame.scale, nextFrame.scale, t);
    glm::quat rotation = glm::slerp(prevFrame.rotation, nextFrame.rotation, t);

    // Apply transformations to your object here
    // (You can pass these values to your rendering system)

    // Check for keyframe events
    checkEvents();
}

// Add a keyframe to the animation
void Animation::addKeyFrame(const KeyFrame& frame) {
    keyFrames.push_back(frame);
}

// Add a keyframe event
void Animation::addEvent(const KeyFrameEvent& event) {
    keyFrameEvents.push_back(event);
}

// Apply easing to the interpolation factor (t)
void Animation::applyEasing(float& t) {
    switch (easingType) {
    case EASE_IN: t = t * t; break;
    case EASE_OUT: t = t * (2 - t); break;
    case EASE_IN_OUT: t = t * t * (3 - 2 * t); break;
    default: break;
    }
}

// Toggle play/pause
void Animation::togglePlay() {
    isPlaying = !isPlaying;
}

// Set animation speed
void Animation::setSpeed(float newSpeed) {
    speed = newSpeed;
}

// Reverse the animation
void Animation::reverse() {
    isReversed = !isReversed;
}

// Check for keyframe events
void Animation::checkEvents() {
    for (KeyFrameEvent& event : keyFrameEvents) {
        if (currentTime >= event.time && !event.triggered) {
            event.callback();
            event.triggered = true;
        }
    }
}