#include "aKeyFrameAnimator.h"
#include "Scene.hpp"

void aKeyframeAnimator::Update() {
    if (keyframes.empty()) return;

    // Get deltaTime from the scene or another source
    float deltaTime = 0.016f; // Example: 60 FPS

    // Apply time scaling
    float timeScale = 1.0f; // Use appropriate timeScale
    deltaTime *= timeScale;

    // handle loop
     //If at the end of the list, 

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
    //Updating time
    currentTime += deltaTime;

    //To take duretion we just substract one timepoint from another
    float frameDuration = keyframes[currentKeyframeIndex + 1].time - keyframes[currentKeyframeIndex].time;
   
    //And if duration is 0, we do nothing
    if (frameDuration <= 0) return;

    //Now we are calculating current time between 2 points
    float t = (currentTime - keyframes[currentKeyframeIndex].time) / frameDuration;
 
    //If at the ed of animation or even further
    if (t >= 1.0f) {
        //Set at the proper end.
        t = 1.0f;


        //Pick Next Key Frame
        currentKeyframeIndex++;

        //Take pos from next frame
        currentPosition = keyframes[currentKeyframeIndex].position;
        currentRotation = keyframes[currentKeyframeIndex].rotation;


       

            //If keyframe out of bounds, do the loop or stop
            if (currentKeyframeIndex >= keyframes.size() - 1)
            {
                if (loop)
                {
                    currentKeyframeIndex = 0;
                    currentTime = 0.0f;
                }
                else
                {
                    return;
                }
            }

        return;
    }


    //Make sure t is between 0 and 1 
   // t = std::clamp(t, 0.0f, 1.0f);
    //Apply function
    t = keyframes[currentKeyframeIndex].easingFunction(t);

    // Interpolate position and rotation
    currentPosition = glm::mix(keyframes[currentKeyframeIndex].position, keyframes[currentKeyframeIndex + 1].position, t);
    currentRotation = glm::mix(keyframes[currentKeyframeIndex].rotation, keyframes[currentKeyframeIndex + 1].rotation, t);

    object->mesh->positionXYZ = currentPosition;
    object->mesh->rotationEulerXYZ = currentRotation;

}   