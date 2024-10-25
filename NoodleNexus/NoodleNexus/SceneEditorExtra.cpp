#include "SceneEditor.h"

#include "GLCommon.h"
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <iostream>

#include "sharedThings.h"   // The camera, etc.
#include "sMesh.h"

#include "cLightManager.h"


std::string parameter = "Position";

void SceneEditor::HandleInputAsync(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
    {
        PickNextObject();

    }

    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
    {
        if (parameter == "Scale") selectedObject->startTranform->scale++;
     
        selectedObject->startTranform->position.x++;
        selectedObject->mesh->positionXYZ.x = selectedObject->startTranform->position.x;

    }

    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    {

        parameter = "Scale";

    }

}

void SaveScene(std::string name)
{

}