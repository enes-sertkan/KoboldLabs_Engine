#pragma once

#include "Action.h"
#include "Scene.hpp"
#include <GLFW/glfw3.h>

class aToggleHammerVisibility : public Action
{
public:
    Object* hammerObject = nullptr;
    bool isHammerVisible = true;
    GLFWwindow* window;

    aToggleHammerVisibility(GLFWwindow* glfwWindow) : window(glfwWindow) {}

    void Update() override
    {

        if (!hammerObject) return;

        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        {

            hammerObject->mesh->bIsVisible = true;
        }
        else
        {
            hammerObject->mesh->bIsVisible = false;

        }
    }
};
