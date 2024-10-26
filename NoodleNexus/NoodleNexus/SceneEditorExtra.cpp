#include "SceneEditor.h"

#include "GLCommon.h"
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <iostream>

#include "sharedThings.h"   // The camera, etc.
#include "sMesh.h"

#include "cLightManager.h"

// Possible transformation parameters
enum TransformMode
{
    Position,
    Rotation,
    ScaleX
};

// Current mode initialized to Position
TransformMode currentTransform = Position;

bool isControlD(GLFWwindow* window)
{
    if ((glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) ||
        (glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS))
    {
        return true;
    }
    return false;
}

void SceneEditor::HandleInputAsync(GLFWwindow* window)
{

    float speed = 0.5;

    //is shift, triple the speed, if not make it 0.5



    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
    {
        ChangeMode("Objects");
        std::cout << "Object Mode." << std::endl;
    }

    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
    {
        ChangeMode("Lights");
        std::cout << "Light Mode." << std::endl;
    }

    if (editMode == "Objects")
    {
        if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
        {
            currentFrameTime = glfwGetTime();
            double deltaTime = currentFrameTime - lastFrameTime;
            lastFrameTime = currentFrameTime;

            // std::cout<< deltaTime<<std::endl;
            if (deltaTime > 0.36f)
            {
                PickNextObject();
            }

        }
        // Toggle transformation mode based on key press
        if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
        {
            currentTransform = Position;
            std::cout << "Position mode selected." << std::endl;
        }
        else if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
        {
            currentTransform = Rotation;
            std::cout << "Rotation mode selected." << std::endl;
        }
        else if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
        {
            currentTransform = ScaleX;
            std::cout << "Scale mode selected." << std::endl;
        }

        // Check which mode is active and apply arrow key transformations
        if (currentTransform == Position)
        {
            // Arrow key controls for position
            if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
                selectedObject->startTranform->position.z += speed;
            if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
                selectedObject->startTranform->position.z -= speed;
            if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
                selectedObject->startTranform->position.x += speed;
            if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
                selectedObject->startTranform->position.x -= 0.5f;
            if (isControlD(window))
            {
                if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
                    selectedObject->startTranform->position.y += 0.5f;
                if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
                    selectedObject->startTranform->position.y -= 0.5f;
            }
        }
        else if (currentTransform == Rotation)
        {
            // Arrow key controls for rotation
            if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
                selectedObject->startTranform->rotation.x += 1.0f;
            if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
                selectedObject->startTranform->rotation.x -= 1.0f;
            if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
                selectedObject->startTranform->rotation.z += 1.0f;
            if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
                selectedObject->startTranform->rotation.z -= 1.0f;
            if (isControlD(window))
            {
                if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
                    selectedObject->startTranform->rotation.y += 1.0f;
                if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
                    selectedObject->startTranform->rotation.y -= 1.0f;
            }
        }
        else if (currentTransform == ScaleX)
        {
            // Arrow key controls for scale (only on the x-axis)
            if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
                selectedObject->startTranform->scale.x += 0.01f;
            if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
                selectedObject->startTranform->scale.x -= 0.01f;
        }

        // Update mesh properties
        selectedObject->mesh->positionXYZ = selectedObject->startTranform->position;
        selectedObject->mesh->rotationEulerXYZ = selectedObject->startTranform->rotation;
        selectedObject->mesh->uniformScale = selectedObject->startTranform->scale.x;
    }


    if (editMode == "Lights")
    {

        if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
        {
            currentFrameTime = glfwGetTime();
            double deltaTime = currentFrameTime - lastFrameTime;
            lastFrameTime = currentFrameTime;

            // std::cout<< deltaTime<<std::endl;
            if (deltaTime > 0.36f)
            {
                PickNextLight();
            }

        }

        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
            selectedLight->position.z += 0.5f;
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
            selectedLight->position.z -= 0.5f;
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
            selectedLight->position.x += 0.5f;
        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
            selectedLight->position.x -= 0.5f;
        if (isControlD(window))
        {
            if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
                selectedLight->position.y += 0.5f;
            if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
                selectedLight->position.y -= 0.5f;
        }



    }

}

void SaveScene(std::string name)
{
    // Implementation for saving the scene (if needed)
}
