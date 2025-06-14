// for non keyboard callback handling

#include "GLCommon.h"
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <iostream>

#include "sharedThings.h"   // The camera, etc.
#include "sMesh.h"

#include "cLightManager.h"
#include "Scene.hpp"

//extern cLightManager* g_pLightManager;

#include "imgui/imgui.h"          // Main MGUI header
#include "imgui/imconfig.h"          // Main MGUI header
#include "imgui/imgui_impl_glfw.h" // GLFW integration (if required)
#include "imgui/imgui_impl_opengl3.h" // OpenGL 3+ integration



struct sMouseState
{
    glm::ivec2 currentPositionXY = glm::ivec2(0, 0);
    glm::ivec2 lastPositionXY = glm::ivec2(0, 0);
    bool bIsLeftMouseButtonDown = false;
};

sMouseState g_MouseState;

bool isShiftDown(GLFWwindow* window)
{
    if ((glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) || 
        (glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS) )
    {
        return true;
    }
    return false;
}

bool isControlDown(GLFWwindow* window)
{
    if ((glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) ||
        (glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS))
    {
        return true;
    }
    return false;
}

bool isAltDown(GLFWwindow* window)
{
    if ((glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS) ||
        (glfwGetKey(window, GLFW_KEY_RIGHT_ALT) == GLFW_PRESS))
    {
        return true;
    }
    return false;
}

bool areAllModifiersUp(GLFWwindow* window)
{
    if ( isShiftDown(window) ) 
    {
        return false;
    }

    if ( isControlDown(window) ) 
    {
        return false;
    }

    if ( isAltDown(window) ) 
    {
        return false;
    }

    // Nothing is down
    return true;
}

void handleMouseAsync(GLFWwindow* window)
{

//    if (glfwRawMouseMotionSupported())
//        glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);


    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);

//    std::cout << "Async mouse location (x,y): " << xpos << ", " << ypos << std::endl;

    return;
}



void handleKeyboardAsync(GLFWwindow* window ,Object* screen_quad, Scene* scene)
{
    const float CAMERA_MOVE_SPEED = 1.f;
    const float CAMERA_TURN_SPEED = 0.6f;


    if (glfwGetKey(window, GLFW_KEY_F1) == GLFW_PRESS)
    {
        scene->isFlyCamera = false;
    }

    if (glfwGetKey(window, GLFW_KEY_F2) == GLFW_PRESS)
    {
        scene->isFlyCamera = true;
    }
    if (screen_quad!=nullptr)
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
    {

        if (screen_quad->mesh->zoomPower >= 1.f)
            screen_quad->mesh->zoomPower = 1.f;
        else
            screen_quad->mesh->zoomPower += scene->deltaTime;

    }
    else
    {
        if (screen_quad->mesh->zoomPower <= 0.f)
            screen_quad->mesh->zoomPower = 0.f;
        else
            screen_quad->mesh->zoomPower -= scene->deltaTime*2;
    }



    if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)
    {
        scene->fCamera->nightMode = true;
    }
    else
    {
        scene->fCamera->nightMode = false;
    }

    if ( isShiftDown(window) )
    {
        // Keys move the target
    }




    if (isControlDown(window) ) 
    {

     




        const float LIGHT_MOVE_SPEED = 0.02f;

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        {
         scene->lightManager->theLights[g_selectedLightIndex].position.z += LIGHT_MOVE_SPEED;
        }

        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        {
            scene->lightManager->theLights[g_selectedLightIndex].position.z -= LIGHT_MOVE_SPEED;
        }

        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        {
            scene->lightManager->theLights[g_selectedLightIndex].position.x -= LIGHT_MOVE_SPEED;
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        {
            scene->lightManager->theLights[g_selectedLightIndex].position.x += LIGHT_MOVE_SPEED;
        }
        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        {
            scene->lightManager->theLights[g_selectedLightIndex].position.y += LIGHT_MOVE_SPEED;
        }
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        {
            scene->lightManager->theLights[g_selectedLightIndex].position.y -= LIGHT_MOVE_SPEED;
        }
        //// Linear atten 1 & 2
        //if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
        //{
        //    // Increase linear (making it darker)
        //    scene->lightManager->theLights[g_selectedLightIndex].atten.y *= 0.99f;     // Down 1%
        //}
        //if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
        //{
        //    // Increase linear (making it darker)
        //    scene->lightManager->theLights[g_selectedLightIndex].atten.y *= 1.01f;     // Up 1%
        //}
        //// Quadratic atten 3 & 4
        //if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
        //{
        //    // Quadratic linear (making it darker)
        //    // This is WAY more sensitive than the linear
        //    //  ...so you might want to change it by 0.1% (0.999)
        //    scene->lightManager->theLights[g_selectedLightIndex].atten.z *= 0.99f;    // Down 1%
        //}
        //if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
        //{
        //    // Quadratic linear (making it darker)
        //    scene->lightManager->theLights[g_selectedLightIndex].atten.z *= 1.01f;     // Up 1%
        //}

        //// Angles of spot light
        //if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
        //{
        //    scene->lightManager->theLights[g_selectedLightIndex].param1.y -= 0.1f;     
        //}       
        //if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
        //{
        //    scene->lightManager->theLights[g_selectedLightIndex].param1.y += 0.1f;     
        //}              

        //if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
        //{
        //    scene->lightManager->theLights[g_selectedLightIndex].param1.z -= 0.1f;     
        //}       
        //if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
        //{
        //    scene->lightManager->theLights[g_selectedLightIndex].param1.z += 0.1f;     
        //}          
        //// HACK:Exit early
        //return;
    }

    if (areAllModifiersUp(window) && scene->isFlyCamera)
    {

        // Move the camera 
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        {
            // Go forward
//            ::g_cameraEye.z -= CAMERA_MOVE_SPEED;
            ::g_pFlyCamera->moveForward( CAMERA_MOVE_SPEED*4 );
        }

        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        {
            // Go back
//            ::g_cameraEye.z += CAMERA_MOVE_SPEED;
            ::g_pFlyCamera->moveForward( -CAMERA_MOVE_SPEED * 4);
        }

        if ( glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS )
        {
            // Go left
//           ::g_cameraEye.x -= CAMERA_MOVE_SPEED;
            ::g_pFlyCamera->moveLeftRight( CAMERA_MOVE_SPEED * 4);
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        {
            // Go right
//            ::g_cameraEye.x += CAMERA_MOVE_SPEED;
            ::g_pFlyCamera->moveLeftRight( -CAMERA_MOVE_SPEED * 4);
        }



        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        {
            // Go down
//            ::g_cameraEye.y -= CAMERA_MOVE_SPEED;
            ::g_pFlyCamera->moveUpDown( -CAMERA_MOVE_SPEED/5 );
        }
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        {
            // Go up
//            ::g_cameraEye.y += CAMERA_MOVE_SPEED;
            ::g_pFlyCamera->moveUpDown( CAMERA_MOVE_SPEED/5 );
        }

        // Use the arrow keys to turn and pitch the camera

    }//if (areAllModifiersUp(window)

    return;
}




bool IsMouseOverImGui()
{
    ImGuiIO& io = ImGui::GetIO();
    return io.WantCaptureMouse; // True when mouse is over any ImGui window
}
// *********************************************************
// Mouse callbacks:

// Set with glfwSetCursorPosCallback(window, cursor_position_callback);
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (IsMouseOverImGui()) return;
//    std::cout << "mouse x,y: " << xpos << ", " << ypos << std::endl;
    
    if (::g_pFlyCamera->scene->isFlyCamera)
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
    else
    {  // Hide and capture the cursor
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        // Optional: Raw mouse motion for smoother input (if supported)
        if (glfwRawMouseMotionSupported()) {
            glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
        }
 
    }

    g_MouseState.currentPositionXY.x = (int)xpos;
    g_MouseState.currentPositionXY.y = (int)-ypos;

    if ( g_MouseState.bIsLeftMouseButtonDown || !::g_pFlyCamera->scene->isFlyCamera)
    {
        // If the Left mouse key is down, 
        //  figure out the differene between the current mouse postion and the last one

        glm::ivec2 deltaMousePosition = g_MouseState.lastPositionXY - g_MouseState.currentPositionXY;

        // Use those values to "turn" the camera
    
        // Move left or right? 
        if ( deltaMousePosition.x != 0 )
        {
            ::g_pFlyCamera->rotateLeftRight_Yaw( (float)deltaMousePosition.x * 1.f);
        }
        // Mouse move Up or down? 
        if ( deltaMousePosition.y != 0 )
        {
            ::g_pFlyCamera->pitchUpDown((float)deltaMousePosition.y*1.f);
        }
    }//if ( g_MouseState.bIsLeftMouseButtonDown )


    // Update the last one
    g_MouseState.lastPositionXY = g_MouseState.currentPositionXY;

    //return;
}

// Set with glfwSetCursorEnterCallback(window, cursor_enter_callback);
void cursor_enter_callback(GLFWwindow* window, int entered)
{
    //if ( entered )
    //{
    //    std::cout << "Mouse is over the window" << std::endl;
    //}
    //else
    //{
    //    std::cout << "Mouse left the window" << std::endl;
    //}
    return;
}


// Set with glfwSetMouseButtonCallback(window, mouse_button_callback);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT)
    {
        switch (action)
        {
        case GLFW_PRESS:
//            std::cout << "GLFW_PRESS" << std::endl;
            g_MouseState.bIsLeftMouseButtonDown = true;
            break;
        case GLFW_RELEASE:
//            std::cout << "GLFW_RELEASE" << std::endl;
            g_MouseState.bIsLeftMouseButtonDown = false;
            break;
        default:
            std::cout << "Mouse button state unknown" << std::endl;
            break;
        }
    }

    return;
}

// Set with glfwSetScrollCallback(window, scroll_callback);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    // For most mice, the value is the DIFFERENCE between the last time this was called!
    // (i.e. it's not an absolute value)
    // It also goes up and down along the Y axis and is an integer
//    std::cout << "xoffset, yoffset: " << xoffset << ", " << yoffset << std::endl;

    const float MOUSE_WHEEL_RATIO = 0.1f;

    ::g_pFlyCamera->adjustMovementSpeed( float(yoffset) * MOUSE_WHEEL_RATIO );

    return;
}



