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

          
        }
    }
};

