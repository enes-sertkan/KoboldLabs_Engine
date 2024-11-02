#include "SceneEditor.h"

#include "GLCommon.h"
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <iostream>
#include <fstream>
#include <vector>

#include "sharedThings.h"   // The camera, etc.
#include "sMesh.h"

#include "cLightManager.h"
#include "KLFileManager.hpp";

// Possible transformation parameters
enum TransformMode
{
    Position,
    Rotation,
    ScaleX
};

enum LightModes
{
    LightPosition,
    Diffuse,
    Specular,
    Atten,
    Param1,
    Param2
};

// Current mode initialized to Position
TransformMode currentTransform = Position;
LightModes currentLightMode = LightPosition;



bool isControlD(GLFWwindow* window)
{
    if ((glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) ||
        (glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS))
    {
        return true;
    }
    return false;
}

bool isShftDown(GLFWwindow* window)
{
    if ((glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) ||
        (glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS))
    {
        return true;
    }
    return false;
}
bool isAltD(GLFWwindow* window)
{
    if ((glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS) ||
        (glfwGetKey(window, GLFW_KEY_RIGHT_ALT) == GLFW_PRESS))
    {
        return true;
    }
    return false;
}

void SceneEditor::HandleInputAsync(GLFWwindow* window)
{

    float speed = 1;


    //is shift, triple the speed, if not make it 0.5
    if (isShftDown(window))
    {

        speed = 10;
        if (isControlD(window))
        {
            speed = 100;

        }

    }
    if (isShftDown(window))
    {
        if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
        {
            scene->flyCamera = true;
        }

        if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
        {
            scene->flyCamera = false;
        }
    }


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

    if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
    {
        currentFrameTime = glfwGetTime();
        double deltaTime = currentFrameTime - lastFrameTime;
        lastFrameTime = currentFrameTime;

        if (deltaTime > 0.36f)
        {
            scene->SetCameraToNextPoint();
        }
     
    }
    if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
    {
        currentFrameTime = glfwGetTime();
        double deltaTime = currentFrameTime - lastFrameTime;
        lastFrameTime = currentFrameTime;

        if (deltaTime > 0.36f)
        {
            scene->NextCameraPoint();
        }

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
                selectedObject->startTranform->position.x -= speed;
            if (isControlD(window))
            {
                if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
                    selectedObject->startTranform->position.y += speed;
                if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
                    selectedObject->startTranform->position.y -= speed;
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
        
        if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
        {
            if (selectedObject) // Check if an object is selected
            {
                scene->RemoveObject(selectedObject);
               // selectedObject = nullptr; // Clear the pointer after deletion
                std::cout << "Selected object deleted." << std::endl;
                PickNextLight();
               
            }
            else
            {
                std::cout << "No object selected for deletion." << std::endl; // Prevent access violation
            }
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

            if (deltaTime > 0.36f)
            {
                PickNextLight();
            }
        }

  

        // Toggle light editing mode with keys 1-7
        if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) currentLightMode = LightPosition;
        if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) currentLightMode = Diffuse;
        if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) currentLightMode = Specular;
        if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS) currentLightMode = Atten;
        if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS) currentLightMode = Param1;
        if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS) currentLightMode = Param2;

        // Based on the currentLightMode, adjust the corresponding property
        switch (currentLightMode)
        {
        case Position:
            if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
                selectedLight->position.z += 0.5f;
            if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
                selectedLight->position.z -= 0.5f;
            if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
                selectedLight->position.x += 0.5f;
            if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
                selectedLight->position.x -= 0.5f;

            // Y-axis controls with Control key
            if (isControlD(window))
            {
                if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
                    selectedLight->position.y += 0.5f;
                if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
                    selectedLight->position.y -= 0.5f;
            }
            break;

        case Diffuse:
            // Increase or decrease diffuse color values
            if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
                selectedLight->diffuse.y += 1.0f; // Adjusting the Y channel
            if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
                selectedLight->diffuse.y -= 1.0f;
            if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
                selectedLight->diffuse.x += 1.0f; // Adjusting the X channel
            if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
                selectedLight->diffuse.x -= 1.0f;

            if (isControlD(window))
            {
                if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
                    selectedLight->diffuse.z += 1.0f; // Adjusting the Z channel
                if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
                    selectedLight->diffuse.z -= 1.0f;
            }
            break;

        case Specular:
            // Adjust specular color and power
            if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
                selectedLight->specular.w += 1.0f; // Power
            if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
                selectedLight->specular.w -= 1.0f;

            if (isControlD(window))
            {
                if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
                    selectedLight->specular.x -= 1.0f; // Adjusting the X channel
                if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
                    selectedLight->specular.x += 1.0f;
            }
            break;

        case Atten:
            // Adjust attenuation
            if (isAltD(window)) {
                if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
                    selectedLight->atten.y *= 0.99f; // Linear
                if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
                    selectedLight->atten.y *= 1.01f;

                if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
                    selectedLight->atten.z *= 0.99f; // Quadratic
                if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
                    selectedLight->atten.z *= 1.01f;
            }
            break;

        case Param1:
            // Adjust param1 values
            if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
                selectedLight->param1.y += 0.1f; // Inner angle
            if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
                selectedLight->param1.y -= 0.1f;

            if (isControlD(window))
            {
                if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
                    selectedLight->param1.z -= 0.1f; // Outer angle
                if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
                    selectedLight->param1.z += 0.1f;
            }
            break;

        case Param2:
            // Toggle on/off for the light
            if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
                selectedLight->param2.x = 1.0f; // Turn on
            if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
                selectedLight->param2.x = 0.0f; // Turn off
            break;
        }
    }
    if (isControlD)
    {
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            SaveScene("SaveScene.txt");
    }


}

void SceneEditor::SaveScene(std::string name)
{
    fileManger->WriteSceneFile(scene, name);
        
}

