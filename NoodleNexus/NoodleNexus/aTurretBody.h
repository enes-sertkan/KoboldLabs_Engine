#pragma once
#include "aTurretPart.h"
class aTurretBody : public aTurretPart
{
public:
    float rotationSpeed = 1.0f;
    glm::vec3 targetDirection = glm::vec3(0, 0, 1);

    // Override Clone() for derived class
    aTurretBody* Clone() const override
    {
        aTurretBody* clone = new aTurretBody(*this);
        clone->object = nullptr;
        return clone;
    }

    // Copy constructor
    aTurretBody(const aTurretBody& other)
        : aTurretPart(other), // Call base class copy constructor
        rotationSpeed(other.rotationSpeed),
        targetDirection(other.targetDirection)
    {
        // Copy any additional members here
    }

    aTurretBody()
    { }
    // Rest of your implementation...
};