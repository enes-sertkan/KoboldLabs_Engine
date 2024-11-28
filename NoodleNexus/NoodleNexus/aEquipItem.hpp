#pragma once

#include "Action.h"
#include "Scene.hpp"

class aEquipItem : public Action
{
public:
    std::string equipTarget;

    void Update() override
    {
        for (Object* obj : object->scene->sceneObjects)
        {
            if (obj->name == "mario1")
            {
                if (object->scene->IsNearby(equipTarget, object->mesh->positionXYZ))
                {
                    std::cout << "Mario nearby hammer. Equipping hammer..." << std::endl;

                    // Hide hammer initially.
                    object->mesh->bIsVisible = false;

                    // Attach hammer to Mario.
                    object->mesh->positionXYZ = obj->mesh->positionXYZ + glm::vec3(0.0f, 2.0f, 0.5f);

                    // Add hammer to Mario's held items.
                    obj->heldItems.push_back(object);

                    // Make hammer visible again and confirm position.
                    object->mesh->bIsVisible = true;

                    std::cout << "Hammer equipped. Visibility: " << object->mesh->bIsVisible << std::endl;
                    std::cout << "New Hammer Position Y: " << object->mesh->positionXYZ.y << std::endl;
                    std::cout << "New Hammer Position Z: " << object->mesh->positionXYZ.z << std::endl;
                }
            }
        }
    }

};
