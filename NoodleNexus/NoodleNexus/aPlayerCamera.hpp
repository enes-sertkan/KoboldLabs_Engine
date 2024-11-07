#pragma once
#include "Action.h"
#include "sObject.h" 
#include "Transform.h" 
#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>
#include "cBasicFlyCamera/cBasicFlyCamera.h"

class aPlayerCamera : public Action
{
private:
    glm::vec3 offset; 
    cBasicFlyCamera* fCamera;
    Transform* cameraTransform;

public:
    aPlayerCamera(cBasicFlyCamera* camera, glm::vec3 cameraOffset)
        : fCamera(camera), offset(cameraOffset)
    {
    }

    void SetOrientationTowardsObject(Object* object);

    void Update() override
    {

        if (object->scene->isFlyCamera) return;


        // Check if the player object and camera are valid
        if (object && object->mesh && fCamera)
        {
            // Update the camera's position based on the player's position + the offset
            glm::vec3 newCameraPosition = object->mesh->positionXYZ + offset;
            fCamera->setEyeLocation(newCameraPosition);

            // Calculate the direction vector from the camera to the player
            glm::vec3 direction = object->mesh->positionXYZ - newCameraPosition;

            //// If the direction vector is valid, update the camera's yaw and pitch
            //if (glm::length(direction) > 0.0f)
            //{
            //    direction = glm::normalize(direction);

            //    float yaw = glm::atan(direction.x, direction.z); 
            //    float pitch = glm::asin(direction.y); 

            //    fCamera->rotateLeftRight_Yaw_NoScaling(yaw);
            //    fCamera->pitchUpDown(pitch);
            //}
        }
    }
};

//void aPlayerCamera::SetOrientationTowardsObject(Object* object) {
//    glm::vec3 direction = glm::normalize(object->startTranform->position - cameraTransform->position);
//
//    float yaw = glm::atan(direction.x, direction.z);  // Yaw: rotation around Y
//    float pitch = glm::asin(direction.y);             // Pitch: rotation around X
//
//    glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), yaw, glm::vec3(0.0f, 1.0f, 0.0f));
//    rotationMatrix = glm::rotate(rotationMatrix, pitch, glm::vec3(1.0f, 0.0f, 0.0f));
//
//    glm::vec3 eulerRotation = glm::vec3(pitch, yaw, 0.0f); // Roll is 0 here
//    cameraTransform->rotation = eulerRotation;  // Assign as Euler angles
//
//}
