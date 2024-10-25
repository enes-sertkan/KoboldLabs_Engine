#include "SceneEditor.h"

#include "GLCommon.h"
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <iostream>

#include "sharedThings.h"   // The camera, etc.
#include "sMesh.h"

#include "cLightManager.h"

void SceneEditor::HandleInputAsync(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
    {
        PickNextObject();

    }


}

void SaveScene(std::string name)
{

}