#pragma once

#include "Action.h"
#include "Scene.hpp"

class aMarioDeath : public Action
{
public:

    std::string modelName;
    bool isOn = false;
    Object* objectToFollow = new Object();
    glm::vec3 offset;


    void Update() override
    {

 if( object->scene->IsNearby("Barrel",object->mesh->positionXYZ) || object->scene->IsNearby("BlueBarrel", object->mesh->positionXYZ) || object->scene->IsNearby("FlameEnemy", object->mesh->positionXYZ))
 {
     object->Destroy();
 }
    }
};
