// aTurretNeckRotate.hpp
#pragma once
#include "Action.h"
#include <glm/glm.hpp>
#include "Scene.hpp"
#include "aTurretNeck.h"
class aTurretNeckRotate : public aTurretNeck
{
public:
    float rotationSpeed = 45.0f; // Degrees per second

    void Start() override
    {

        turret->constantShooting = true;

    }


    void Update() override 
    {
        turret->constantShooting = true;
        if (!object) return;
        
        // Rotate around Y axis
        float deltaRotation = rotationSpeed * object->scene->deltaTime;
        object->mesh->rotationEulerXYZ.y += deltaRotation;

 
    }

    aTurretNeck* Clone() const override
    {
        aTurretNeckRotate* clone = new aTurretNeckRotate(*this);
        clone->object = nullptr;
        return clone;
    }
};