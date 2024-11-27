#pragma once

#include "Action.h"
#include "Scene.hpp"

class aEquipItem : public Action
{
public:
    std::string equipTarget;

    void Update() override
    {
        // Iterate over all objects in the scene to find the item and Mario
        for (Object* obj : object->scene->sceneObjects)
        {
            if (obj->name == "mario1") // If this is the player (Mario)
            {
                if (object->scene->IsNearby(equipTarget, object->mesh->positionXYZ)) // If Mario is nearby the item
                {
                    object->mesh->bIsVisible = false;  // Hide the item

                    // Remove the item from the scene
                    //object->scene->RemoveObject(object);

                    // Attach the item to Mario
                    object->mesh->positionXYZ = obj->mesh->positionXYZ + glm::vec3(0.0f, 2.0f, 0.5f); // Adjust position to appear in hand
                    obj->heldItems.push_back(object);  // Add to Mario's inventory

                    // You can optionally set the item as a child or make it visible after the attachment
                    object->mesh->bIsVisible = true;  
                }
            }
        }
    }
};
