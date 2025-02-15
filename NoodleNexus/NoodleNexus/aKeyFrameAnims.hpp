#ifndef AKEYFRAMEANIMATION_HPP
#define AKEYFRAMEANIMATION_HPP

#include <GLFW/glfw3.h>
#include <vector>
#include <functional>
#include <cmath>
#include <algorithm>
#include <iostream>
#include <vector>

// Define M_PI if it is not already defined
#ifndef M_PI
#define M_PI 3.14159265358979323846
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

// Keyframe structure
struct Keyframe {
    Vector2 position;
    float rotation;
    float time;
    std::function<float(float)> easingFunction;

    Keyframe(Vector2 pos, float rot, float t, std::function<float(float)> easing = linear)
        : position(pos), rotation(rot), time(t), easingFunction(easing) {}
};

// KeyframeAnimation class
class aKeyframeAnimation {
public:
    aKeyframeAnimation(const std::vector<Keyframe>& keyframes, bool loop = false)
        : keyframes(keyframes), loop(loop), currentTime(0.0f), currentKeyframeIndex(0) {}

    void update(float deltaTime, float timeScale);
    Vector2 getPosition() const { return currentPosition; }
    float getRotation() const { return currentRotation; }

private:
    std::vector<Keyframe> keyframes;
    size_t currentKeyframeIndex;
    float currentTime;
    Vector2 currentPosition;
    float currentRotation;
    bool loop;
};

#endif // AKEYFRAMEANIMATION_HPP