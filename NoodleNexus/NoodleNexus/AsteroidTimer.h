#include "Action.h"
#include "Scene.hpp"  // Assuming Scene has the RemoveObject method

class AsteroidTimer : public Action {
public:
    AsteroidTimer(Scene* scene) : scene(scene), timeElapsed(0.0) {}

    void Update() override {
        timeElapsed += deltaTime;  // Update the timer

        if (timeElapsed >= 8.0f) {  // 8-second threshold
            std::cout << "Asteroid lifetime exceeded 8 seconds. Removing asteroid." << std::endl;
            scene->RemoveObject(object);  // Remove asteroid from the scene
        }
    }

private:
    Scene* scene;      // Reference to the scene to access RemoveObject
    float timeElapsed; // Tracks the time since spawning
};
