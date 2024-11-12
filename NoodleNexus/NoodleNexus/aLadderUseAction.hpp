#pragma once
#include "Action.h";
#include "sObject.h"
#include <glm/vec3.hpp> 
#include "cBasicFlyCamera/cBasicFlyCamera.h"
#include "Scene.hpp"
#include <glm/glm.hpp>



class LadderUseAction : public Action
{
public:
    
    bool isClimbing = false;

    void Start()
    {
    }

    void Update() override
    {
        // Laser firing with right mouse click

        if (glfwGetKey(object->scene->window, GLFW_KEY_W) == GLFW_PRESS)
        {

        }

    }
 
};
