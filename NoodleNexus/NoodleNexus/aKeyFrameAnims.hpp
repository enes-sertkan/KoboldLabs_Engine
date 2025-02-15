#ifndef AKEYFRAMEANIMS_HPP
#define AKEYFRAMEANIMS_HPP

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <vector>
#include <functional>
#include <iostream>

// Easing types
enum EasingType {
    NONE,
    EASE_IN,
    EASE_OUT,
    EASE_IN_OUT
};

// KeyFrame struct
struct KeyFrame {
    float time;
    glm::vec3 position;
    glm::vec3 scale;
    glm::quat rotation;

    KeyFrame(float t, glm::vec3 pos, glm::vec3 scl, glm::quat rot)
        : time(t), position(pos), scale(scl), rotation(rot) {}
};

// KeyFrameEvent struct
struct KeyFrameEvent {
    float time;
    std::function<void()> callback;
    bool triggered = false;

    KeyFrameEvent(float t, std::function<void()> cb)
        : time(t), callback(cb) {}
};

// Animation class
class Animation {
public:
    std::vector<KeyFrame> keyFrames;
    std::vector<KeyFrameEvent> keyFrameEvents;
    float duration;
    float currentTime = 0.0f;
    float speed = 1.0f;
    bool isPlaying = true;
    bool isReversed = false;
    EasingType easingType = NONE;

    Animation(float dur) : duration(dur) {}

    void update(float deltaTime);
    void addKeyFrame(const KeyFrame& frame);
    void addEvent(const KeyFrameEvent& event);
    void applyEasing(float& t);
    void togglePlay();
    void setSpeed(float newSpeed);
    void reverse();

private:
    void checkEvents();
};

#endif // KEYFRAMEANIMS_HPP