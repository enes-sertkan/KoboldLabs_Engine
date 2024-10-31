#pragma once
#include "sObject.h"
#include <string>
#include <iostream>

class Action
{
private:

public:
    double deltaTime;
    Object* object = nullptr;

    virtual void Start() {}

    // Update method with deltaTime parameter
    virtual void Update()
    {
      

        printf("WOW, you are UPDATING\n");

    }
};

class MoveForward : public Action
{
    void Update() override
    {
        // Use deltaTime to control movement speed
        object->mesh->positionXYZ.x += 1.0f; // Adjust this speed as needed
    }
};


class ExplosionLogic : public Action {
public:
    void Update() override {
        if (!object) {
            std::cerr << "Error: object is nullptr!" << std::endl; // Error handling
            return; // Prevent dereferencing a null pointer
        }

        // Check if the explosion has reached its maximum size
        if (object->mesh->uniformScale >= 1000.0f) {
            // Set the asteroid's visibility to zero (invisible)
            object->mesh->bIsVisible = false; // Make object invisible
            return;
        }

        // Increment the scale during the explosion
        object->mesh->uniformScale += 500.0f * deltaTime; // Adjust explosion speed as needed

        // Optionally, gradually reduce opacity for a fade-out effect (uncomment if desired)
        //object->mesh->objectColourRGBA.a = glm::max(0.0f, object->mesh->objectColourRGBA.a - 0.01f * deltaTime);
    }
};


//class AsteroidTimer : public Action {
//public:
//    AsteroidTimer(Scene* scene) : scene(scene), timeElapsed(0.0) {}
//
//    void Update() override {
//        timeElapsed += deltaTime;  // Update the timer
//
//        if (timeElapsed >= 8.0f) {  // 8-second threshold
//            std::cout << "Asteroid lifetime exceeded 8 seconds. Removing asteroid." << std::endl;
//            scene->RemoveObject(object);  // Remove asteroid from the scene
//        }
//    }
//
//private:
//    Scene* scene;      // Reference to the scene to access RemoveObject
//    float timeElapsed; // Tracks the time since spawning
//};
