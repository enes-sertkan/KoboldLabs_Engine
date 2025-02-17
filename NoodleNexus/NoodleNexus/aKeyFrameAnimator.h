#ifndef AKEYFRAMEANIMATION_HPP
#define AKEYFRAMEANIMATION_HPP

#include <vector>
#include <functional>
#include <cmath>
#include <algorithm>
#include <iostream>
#include "Action.h"
#include <glm/glm.hpp> 
#include <glm/gtc/matrix_transform.hpp>

// Define M_PI if it is not already defined
#ifndef M_PI
#define M_PI 3.14
#endif

// Vector2 structure for 2D positions
struct Vector2 {
    float x, y;

    Vector2(float x = 0.0f, float y = 0.0f) : x(x), y(y) {}
};

// Linear Interpolation (LERP) function
inline Vector2 lerp(const Vector2& start, const Vector2& end, float t) {
    return Vector2((1 - t) * start.x + t * end.x, (1 - t) * start.y + t * end.y);
}

// Spherical Linear Interpolation (SLERP) function for Rotation
inline float slerp(float start, float end, float t) {
    float delta = fmod(end - start + 360, 360.0f);
    if (delta > 180.0f) delta -= 360.0f;
    return start + delta * t;
}

// Easing functions
inline float linear(float t) { return t; }
inline float easeInSine(float t) { return 1 - cos(t * M_PI / 2); }
inline float easeOutSine(float t) { return sin(t * M_PI / 2); }
inline float easeInOutSine(float t) { return -(cos(M_PI * t) - 1) / 2; }

// Convenience wrapper functions
inline float EaseIn(float t) { return easeInSine(t); }
inline float EaseOut(float t) { return easeOutSine(t); }
inline float EaseInOut(float t) { return easeInOutSine(t); }

struct Keyframe {
    glm::vec3 position; // Use glm::vec3 for position
    glm::vec3 rotation; // Use glm::vec3 for rotation (Euler angles)
    glm::vec3 scale; // Use glm::vec3 for rotation (Euler angles)
    float time;
    std::function<float(float)> easingFunction;

    Keyframe(glm::vec3 pos, glm::vec3 rot, glm::vec3 sc, float t, std::function<float(float)> easing = linear)
   {
    
        position = pos;
        rotation = rot;
        scale = sc;
        time = t;
        easingFunction = easing;
    }
};

class aKeyframeAnimator : public Action {
public:
    aKeyframeAnimator() : currentTime(0.0f), currentKeyframeIndex(0), loop(false) {}

    void AddKeyframe(float time, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, std::function<float(float)> easing = linear) {
        keyframes.emplace_back(position, rotation, scale, time, easing);
    }

    void SetEasingFunctionForKeyframe(size_t index, std::function<float(float)> easingFunction) {
        if (index < keyframes.size()) {
            keyframes[index].easingFunction = easingFunction;
        }
    }

    void Update() override; // Override the Update method from Action
    glm::vec3 getPosition() const { return currentPosition; }
    glm::vec3 getRotation() const { return currentRotation; }

    std::vector<Keyframe> keyframes;
    size_t currentKeyframeIndex;
    float currentTime;
    glm::vec3 currentPosition;
    glm::vec3 currentRotation;
    bool loop;
};
#endif // AKEYFRAMEANIMATION_HPP