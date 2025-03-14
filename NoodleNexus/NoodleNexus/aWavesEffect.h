#pragma once
#include "sObject.h"
#include <glm/glm.hpp>
#include "Scene.hpp"

class Object;


class aWavesEffect: public Action
{
private:

public:

    Object* player = nullptr;
	
	virtual void Start()
	{
	}
	virtual void Update()
	{
   
        if (glfwGetKey(object->scene->window, GLFW_KEY_T) == GLFW_PRESS)
        {
            glm::vec2 planeScale = glm::vec2(0.75f * object->mesh->uniformScale, 1.f * object->mesh->uniformScale);
            glm::vec2 uvPos = CalculateUV(player->mesh->positionXYZ, object->mesh->positionXYZ, object->mesh->rotationEulerXYZ, planeScale);
            object->mesh->spawnWave(glm::vec2(static_cast<float>(rand()) / static_cast<float>(RAND_MAX), static_cast<float>(rand()) / static_cast<float>(RAND_MAX)));


        }
        else
        {

        }
	}

    glm::vec2 CalculateUV(glm::vec3 worldPos, glm::vec3 planePos, glm::vec3 planeRotation, glm::vec2 planeScale) {
        // Step 1: Convert world position to local space (relative to plane position)
        glm::vec3 localPos = worldPos - planePos;

        // Step 2: Create a rotation matrix from the plane's rotation
        glm::mat4 rotationMatrix = glm::mat4(1.0f);
        rotationMatrix = glm::rotate(rotationMatrix, glm::radians(planeRotation.x), glm::vec3(1, 0, 0)); // Pitch
        rotationMatrix = glm::rotate(rotationMatrix, glm::radians(planeRotation.y), glm::vec3(0, 1, 0)); // Yaw
        rotationMatrix = glm::rotate(rotationMatrix, glm::radians(planeRotation.z), glm::vec3(0, 0, 1)); // Roll

        // Step 3: Rotate the local position into the plane's space
        glm::vec3 localRotatedPos = glm::vec3(glm::inverse(rotationMatrix) * glm::vec4(localPos, 1.0f));

        // Step 4: Convert to UV coordinates (scaling from -planeScale/2 to +planeScale/2 → 0 to 1)
        glm::vec2 uv;
        uv.x = (localRotatedPos.x / planeScale.x) + 0.5f;
        uv.y = (localRotatedPos.z / planeScale.y) + 0.5f; // Using Z because it's an XZ plane

        return uv;
    }

};


