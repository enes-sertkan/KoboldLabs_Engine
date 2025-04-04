#pragma once

#include "LabAttackFactory.h"
#include <glm/glm.hpp>
#include "cTurret.h"
#include "sObject.h"

class aTurretPart : public Action
{
public:
    LabAttackFactory* factory = nullptr;
    Turret* turret = nullptr;


    // Override Clone() for derived class
    Action* Clone() const override
    {
        aTurretPart* clone = new aTurretPart(*this);
        clone->object = nullptr;
        clone->factory = factory;
        return clone;
    }

    // Copy constructor
    aTurretPart(const aTurretPart& other)
        : Action(other) // Call base class copy constructor
    {
        // Copy any additional members here
    }

    aTurretPart()
    {
    }
    // Rest of your implementation...
};