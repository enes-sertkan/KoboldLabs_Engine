#pragma once

#include "aProjectileMovement.hpp"
#include "Action.h";
#include "sObject.h"
#include <glm/vec3.hpp> 
#include "cBasicFlyCamera/cBasicFlyCamera.h"
#include "Scene.hpp"
#include <glm/glm.hpp>


class aLinearProjectileMovement : public aProjectileMovement
{
public:
 



    void Update() override
    {
        
        ApplyAcceleration();

        ApplySpeed();


    }

protected:
    virtual void OnImpact()
    {
        // Trigger explosion or other impact effects
    }
};
