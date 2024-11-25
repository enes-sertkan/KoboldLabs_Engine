//#define GLAD_GL_IMPLEMENTATION
//#include <glad/glad.h>
//
//#define GLFW_INCLUDE_NONE
//#include <GLFW/glfw3.h>

#define GLM_ENABLE_EXPERIMENTAL 
#include "Action.h"
#include "aMoveInDirection.h"
#include "GLCommon.h"

//#include "linmath.h"
#include <glm/glm.hpp>
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> 
// glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr
//#include <glm/gtc/magnitude.hpp> // Magnitude calculations
#include <glm/gtx/norm.hpp>  // Normalize, length

#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>

#include <iostream>     // "input output" stream
#include <fstream>      // "file" stream
#include <sstream>      // "string" stream ("string builder" in Java c#, etc.)
#include <string>
#include <vector>

//void ReadPlyModelFromFile(std::string plyFileName);
#include "PlyFileLoaders.h"
#include "Basic_Shader_Manager/cShaderManager.h"
#include "sMesh.h"
#include "cVAOManager/cVAOManager.h"
#include "sharedThings.h"       // Fly camera
#include "cPhysics.h"
#include "cLightManager.h"
#include <windows.h>    // Includes ALL of windows... MessageBox
#include "cLightHelper/cLightHelper.h"
#include "KLFileManager.hpp"
#include "PhysicsManager.h"
#include "SceneEditor.h"
#include "Scene.hpp"
#include "cBasicFlyCamera/cBasicFlyCamera.h"
#include "sObject.h"
#include "aPlayerCamera.hpp"  // Include the header for aPlayerCamera class
#include "aPlayerMovement.h"
#include "MazeGenerator.hpp"
#include "aRotateAction.hpp"

#include "aRayCastPhysics.h"
#include "aDrawAim.hpp"
#include "aPlayerItemsController.h"
#include "aRayCastPhysics2D.hpp"
#include "aPlayerMovement2D.hpp"
#include "BarrelFactory.h"
#include "aModelsFramesAnimator.hpp"
#include "aClimb.hpp"


std::vector<sMesh*> g_vecMeshesToDraw;

cPhysics* g_pPhysicEngine = NULL;
// This loads the 3D models for drawing, etc.
cVAOManager* g_pMeshManager = NULL;


void DrawMesh(sMesh* pCurMesh, GLuint program);



static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

bool isControlDown(GLFWwindow* window);


static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);

    const float CAMERA_MOVE_SPEED = 0.1f;

    if (mods == GLFW_MOD_SHIFT)
    {
        if (key == GLFW_KEY_F9 && action == GLFW_PRESS)
        {
            // Save state to file
//            MyAmazingStateThing->saveToFile("MySaveFile.sexy");
        }
        if (key == GLFW_KEY_F10 && action == GLFW_PRESS)
        {
            // Save state to file
            // https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-messagebox
//            MessageBox(NULL, L"Hello!", L"The title", MB_OK);
            if (MessageBox(NULL, L"Kill all humans?", L"Bender asks", MB_YESNO) == IDYES)
            {
                std::cout << "You are heartless" << std::endl;
            }
            else
            {
                std::cout << "Humans still live..." << std::endl;
            }
        }
    }//if (mods == GLFW_MOD_SHIFT)

 //   if (mods == GLFW_KEY_LEFT_CONTROL)
    if (isControlDown(window))
    {
        if (key == GLFW_KEY_5 && action == GLFW_PRESS)
        {
            // check if you are out of bounds
            if (::g_selectedLightIndex > 0)
            {

                ::g_selectedLightIndex--;
            }

        }
        if (key == GLFW_KEY_6 && action == GLFW_PRESS)
        {
            ::g_selectedLightIndex++;
            if (::g_selectedLightIndex >= 10)
            {
                ::g_selectedLightIndex = 9;
            }
        }

        if (key == GLFW_KEY_9 && action == GLFW_PRESS)
        {
            ::g_bShowDebugSpheres = true;
        }
        if (key == GLFW_KEY_0 && action == GLFW_PRESS)
        {
            ::g_bShowDebugSpheres = false;
        }
    }

    return;
}

void ConsoleStuff(void);

// https://stackoverflow.com/questions/5289613/generate-random-float-between-two-floats
float getRandomFloat(float a, float b) {
    float random = ((float)rand()) / (float)RAND_MAX;
    float diff = b - a;
    float r = random * diff;
    return a + r;
}


// Returns NULL if NOT found
sMesh* pFindMeshByFriendlyName(std::string theNameToFind)
{
    for (unsigned int index = 0; index != ::g_vecMeshesToDraw.size(); index++)
    {
        if (::g_vecMeshesToDraw[index]->uniqueFriendlyName == theNameToFind)
        {
            return ::g_vecMeshesToDraw[index];
        }
    }
    // Didn't find it
    return NULL;
}




GLuint PrepareOpenGL(GLFWwindow* const& window)
{



    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    // Callback for keyboard, but for "typing"
    // Like it captures the press and release and repeat
    glfwSetKeyCallback(window, key_callback);

    // 
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetWindowFocusCallback(window, cursor_enter_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetScrollCallback(window, scroll_callback);



    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    glfwSwapInterval(1);

    // NOTE: OpenGL error checks have been omitted for brevity


    cShaderManager* pShaderManager = new cShaderManager();

    cShaderManager::cShader vertexShader;
    vertexShader.fileName = "assets/shaders/vertex01.glsl";

    cShaderManager::cShader fragmentShader;
    fragmentShader.fileName = "assets/shaders/fragment01.glsl";

    if (!pShaderManager->createProgramFromFile("shader01",
        vertexShader, fragmentShader))
    {
        std::cout << "Error: " << pShaderManager->getLastError() << std::endl;
    }
    else
    {
        std::cout << "Shader built OK" << std::endl;
    }

    const GLuint program = pShaderManager->getIDFromFriendlyName("shader01");

    glUseProgram(program);

    return program;
}

sModelDrawInfo LoadPlyModel(std::string modelPath, GLuint program)
{
    sModelDrawInfo modelInfo;
    ::g_pMeshManager->LoadModelIntoVAO(modelPath,
        modelInfo, program);
    std::cout << modelInfo.meshPath << "-Loaded" << std::endl << modelInfo.numberOfVertices << " vertices loaded" << std::endl;
    return modelInfo;
}


//TODO: Right now we should not touch physics. But later we will efactor and implement it correctly.
void PreparePhysics()
{
    ::g_pPhysicEngine = new cPhysics();

    // For triangle meshes, let the physics object "know" about the VAO manager
    ::g_pPhysicEngine->setVAOManager(::g_pMeshManager);
}
void PrepareFlyCamera()
{

    ::g_pFlyCamera = new cBasicFlyCamera();
    ::g_pFlyCamera->setEyeLocation(glm::vec3(0.0f, 2.0f, 10.0f));  // Set a more reasonable camera position
    // Rotate the camera 180 degrees
    ::g_pFlyCamera->rotateLeftRight_Yaw_NoScaling(glm::radians(90.0f));  // Adjust yaw
    ::g_pFlyCamera->pitchUpDown(glm::radians(15.0f));
    ::g_pFlyCamera->adjustMovementSpeed(10.0f);
    ::g_pFlyCamera->setEyeTarget(glm::vec3(0.0f, 0.0f, 0.0f));  // Set target to a specific object (like the player)
}



//DONE: Pick Better Name
void SetCameraAndProjectionMatrices(float ratio, GLuint program)
{
    //        glm::mat4 m, p, v, mvp;
    glm::mat4 matProjection = glm::mat4(1.0f);

    matProjection = glm::perspective(0.6f,           // FOV
        ratio,          // Aspect ratio of screen
        0.1f,           // Near plane
        1000000.0f);       // Far plane

    // View or "camera"
    glm::mat4 matView = glm::mat4(1.0f);

    //        glm::vec3 cameraEye = glm::vec3(0.0, 0.0, 4.0f);
    glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 upVector = glm::vec3(0.0f, 1.0f, 0.0f);

    matView = glm::lookAt(::g_pFlyCamera->getEyeLocation(),
        ::g_pFlyCamera->getTargetLocation(),
        upVector);


    const GLint matView_UL = glGetUniformLocation(program, "matView");
    glUniformMatrix4fv(matView_UL, 1, GL_FALSE, (const GLfloat*)&matView);

    const GLint matProjection_UL = glGetUniformLocation(program, "matProjection");
    glUniformMatrix4fv(matProjection_UL, 1, GL_FALSE, (const GLfloat*)&matProjection);

}

void DrawLazer(GLuint program)
{

    // Draw the LASER beam
    cPhysics::sLine LASERbeam;
    glm::vec3 LASERbeam_Offset = glm::vec3(0.0f, -2.0f, 0.0f);

    if (::g_bShowLASERBeam)
    {

        // The fly camera is always "looking at" something 1.0 unit away
        glm::vec3 cameraDirection = ::g_pFlyCamera->getTargetRelativeToCamera();     //0,0.1,0.9


        LASERbeam.startXYZ = ::g_pFlyCamera->getEyeLocation();

        // Move the LASER below the camera
        LASERbeam.startXYZ += LASERbeam_Offset;
        glm::vec3 LASER_ball_location = LASERbeam.startXYZ;

        // Is the LASER less than 500 units long?
        // (is the last LAZER ball we drew beyond 500 units form the camera?)
        while (glm::distance(::g_pFlyCamera->getEyeLocation(), LASER_ball_location) < 150.0f)
        {
            // Move the next ball 0.1 times the normalized camera direction
            LASER_ball_location += (cameraDirection * 0.10f);
            DrawDebugSphere(LASER_ball_location, glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), 0.05f, program);
        }

        // Set the end of the LASER to the last location of the beam
        LASERbeam.endXYZ = LASER_ball_location;

    }//if (::g_bShowLASERBeam)

    // Draw the end of this LASER beam
    DrawDebugSphere(LASERbeam.endXYZ, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), 0.1f, program);

    // Now draw a different coloured ball wherever we get a collision with a triangle
    std::vector<cPhysics::sCollision_RayTriangleInMesh> vec_RayTriangle_Collisions;
    ::g_pPhysicEngine->rayCast(LASERbeam.startXYZ, LASERbeam.endXYZ, vec_RayTriangle_Collisions, false);

    glm::vec4 triColour = glm::vec4(1.0f, 1.0f, 0.0f, 1.0f);
    float triangleSize = 0.25f;

    for (std::vector<cPhysics::sCollision_RayTriangleInMesh>::iterator itTriList = vec_RayTriangle_Collisions.begin();
        itTriList != vec_RayTriangle_Collisions.end(); itTriList++)
    {
        for (std::vector<cPhysics::sTriangle>::iterator itTri = itTriList->vecTriangles.begin();
            itTri != itTriList->vecTriangles.end(); itTri++)
        {

            //                DrawDebugSphere(itTri->intersectionPoint, glm::vec4(1.0f, 1.0f, 0.0f, 1.0f), 0.25f, program);
            DrawDebugSphere(itTri->intersectionPoint, triColour, triangleSize, program);
            triColour.r -= 0.1f;
            triColour.g -= 0.1f;
            triColour.b += 0.2f;
            triangleSize *= 1.25f;


        }//for (std::vector<cPhysics::sTriangle>::iterator itTri = itTriList->vecTriangles

    }//for (std::vector<cPhysics::sCollision_RayTriangleInMesh>::iterator itTriList = vec_RayTriangle_Collisions
}

void SetLight(cLightManager* lightManager, int index,
    const glm::vec4& position,
    const glm::vec4& diffuse,
    const glm::vec3& attenuation,
    const glm::vec4& direction,
    const glm::vec3& param1,
    float param2x)
{
    // Set the properties of the light using lightManager instead of g_pLightManager
    lightManager->theLights[index].position = position;
    lightManager->theLights[index].diffuse = diffuse;
    lightManager->theLights[index].atten.y = attenuation.y;
    lightManager->theLights[index].atten.z = attenuation.z;

    // If it's a spotlight, set the direction and angles
    lightManager->theLights[index].direction = direction;
    lightManager->theLights[index].param1 = glm::vec4(param1, 0.0f);
    lightManager->theLights[index].param2.x = param2x;  // Turn on/off
}




void DrawDebugObjects(cLightHelper TheLightHelper, GLuint program, cLightManager* lightManager)
{


    DrawDebugSphere(lightManager->theLights[::g_selectedLightIndex].position,
        glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), 0.1f, program);

    const float DEBUG_LIGHT_BRIGHTNESS = 0.3f;

    const float ACCURACY = 0.1f;       // How many units distance
    float distance_75_percent =
        TheLightHelper.calcApproxDistFromAtten(0.75f, ACCURACY, FLT_MAX,
            lightManager->theLights[::g_selectedLightIndex].atten.x,   // Const attent
            lightManager->theLights[::g_selectedLightIndex].atten.y,   // Linear attenuation
            lightManager->theLights[::g_selectedLightIndex].atten.z);  // Quadratic attenuation

    DrawDebugSphere(lightManager->theLights[::g_selectedLightIndex].position,
        glm::vec4(DEBUG_LIGHT_BRIGHTNESS, 0.0f, 0.0f, 1.0f),
        distance_75_percent,
        program);


    float distance_50_percent =
        TheLightHelper.calcApproxDistFromAtten(0.5f, ACCURACY, FLT_MAX,
            lightManager->theLights[::g_selectedLightIndex].atten.x,   // Const attent
            lightManager->theLights[::g_selectedLightIndex].atten.y,   // Linear attenuation
            lightManager->theLights[::g_selectedLightIndex].atten.z);  // Quadratic attenuation

    DrawDebugSphere(lightManager->theLights[::g_selectedLightIndex].position,
        glm::vec4(0.0f, DEBUG_LIGHT_BRIGHTNESS, 0.0f, 1.0f),
        distance_50_percent,
        program);

    float distance_25_percent =
        TheLightHelper.calcApproxDistFromAtten(0.25f, ACCURACY, FLT_MAX,
            lightManager->theLights[::g_selectedLightIndex].atten.x,   // Const attent
            lightManager->theLights[::g_selectedLightIndex].atten.y,   // Linear attenuation
            lightManager->theLights[::g_selectedLightIndex].atten.z);  // Quadratic attenuation

    DrawDebugSphere(lightManager->theLights[::g_selectedLightIndex].position,
        glm::vec4(0.0f, 0.0f, DEBUG_LIGHT_BRIGHTNESS, 1.0f),
        distance_25_percent,
        program);

    float distance_05_percent =
        TheLightHelper.calcApproxDistFromAtten(0.05f, ACCURACY, FLT_MAX,
            lightManager->theLights[::g_selectedLightIndex].atten.x,   // Const attent
            lightManager->theLights[::g_selectedLightIndex].atten.y,   // Linear attenuation
            lightManager->theLights[::g_selectedLightIndex].atten.z);  // Quadratic attenuation

    DrawDebugSphere(lightManager->theLights[::g_selectedLightIndex].position,
        glm::vec4(DEBUG_LIGHT_BRIGHTNESS, DEBUG_LIGHT_BRIGHTNESS, 0.0f, 1.0f),
        distance_05_percent,
        program);
}

void UpdateBallShadow()
{
    // HACK: Update "shadow" of ball to be where the ball hits the large block ground
    sMesh* pBallShadow = pFindMeshByFriendlyName("Ball_Shadow");
    sMesh* pBall = pFindMeshByFriendlyName("Ball");
    pBallShadow->positionXYZ.x = pBall->positionXYZ.x;
    pBallShadow->positionXYZ.z = pBall->positionXYZ.z;
    // Don't update the y - keep the shadow near the plane

}

void HandleCollisions()
{

    // Handle any collisions
    if (::g_pPhysicEngine->vec_SphereAABB_Collisions.size() > 0)
    {
        // Yes, there were collisions

        for (unsigned int index = 0; index != ::g_pPhysicEngine->vec_SphereAABB_Collisions.size(); index++)
        {
            cPhysics::sCollision_SphereAABB thisCollisionEvent = ::g_pPhysicEngine->vec_SphereAABB_Collisions[index];

            if (thisCollisionEvent.pTheSphere->pPhysicInfo->velocity.y < 0.0f)
            {
                // Yes, it's heading down
                // So reverse the direction of velocity
                thisCollisionEvent.pTheSphere->pPhysicInfo->velocity.y = fabs(thisCollisionEvent.pTheSphere->pPhysicInfo->velocity.y);
            }

        }//for (unsigned int index

    }//if (::g_pPhysicEngine->vec_SphereAABB_Collisions

}

void UpdateWindowTitle(GLFWwindow* window, cLightManager* lightManager)
{

    //std::cout << "Camera: "
    std::stringstream ssTitle;
    ssTitle << "Camera: "
        << ::g_pFlyCamera->getEyeLocation().x << ", "
        << ::g_pFlyCamera->getEyeLocation().y << ", "
        << ::g_pFlyCamera->getEyeLocation().z
        << "   ";
    ssTitle << "light[" << g_selectedLightIndex << "] "
        << lightManager->theLights[g_selectedLightIndex].position.x << ", "
        << lightManager->theLights[g_selectedLightIndex].position.y << ", "
        << lightManager->theLights[g_selectedLightIndex].position.z
        << "   "
        << "linear: " << lightManager->theLights[0].atten.y
        << "   "
        << "quad: " << lightManager->theLights[0].atten.z;

    //        glfwSetWindowTitle(window, "Hey!");
    glfwSetWindowTitle(window, ssTitle.str().c_str());
}



int main(void)
{
    // Instantiate KLFileManager
    KLFileManager* fileManager = new KLFileManager();

    // Create an sModelDrawInfo object for testing
    sModelDrawInfo modelInfo;


    // question 1 creating models

    modelInfo.modelName = "Cube";
    modelInfo.meshPath = "assets/models/Cube_xyz_n_uv.ply";
    // Call WriteModelFile to save the model info
    fileManager->WriteModelFile(&modelInfo, "Cube.txt", "XYZNUV");

    modelInfo.modelName = "Sphere";
    modelInfo.meshPath = "assets/models/Sphere_radius_xyz_n_uv.ply";
    fileManager->WriteModelFile(&modelInfo, "Sphere.txt", "XYZNUV");

    //Props
    modelInfo.modelName = "base";
    modelInfo.meshPath = "assets/models/DonkeyKong_Level_0_base.ply";
    fileManager->WriteModelFile(&modelInfo, "base.txt", "XYZNUV");

    modelInfo.modelName = "ladder";
    modelInfo.meshPath = "assets/models/DonkeyKong_Level_0_Ladders.ply";
    fileManager->WriteModelFile(&modelInfo, "ladder.txt", "XYZNUV");

    modelInfo.modelName = "platformLong";
    modelInfo.meshPath = "assets/models/DonkeyKong_Level_1_PlatformSectionLong.ply";
    fileManager->WriteModelFile(&modelInfo, "platformLong.txt", "XYZNUV");

    // KONG
    modelInfo.modelName = "kong1";
    modelInfo.meshPath = "assets/models/dk_3d_all_obj/DonkeyKong_Level_0_Kong_0.ply";
    fileManager->WriteModelFile(&modelInfo, "kong1.txt", "XYZNUVRGBA");

    modelInfo.modelName = "kong2";
    modelInfo.meshPath = "assets/models/dk_3d_all_obj/DonkeyKong_Level_0_Kong_1.ply";
    fileManager->WriteModelFile(&modelInfo, "kong2.txt", "XYZNUVRGBA");

    modelInfo.modelName = "kong3";
    modelInfo.meshPath = "assets/models/dk_3d_all_obj/DonkeyKong_Level_0_Kong_2.ply";
    fileManager->WriteModelFile(&modelInfo, "kong3.txt", "XYZNUVRGBA");

    modelInfo.modelName = "kong4";
    modelInfo.meshPath = "assets/models/dk_3d_all_obj/DonkeyKong_Level_0_Kong_3.ply";
    fileManager->WriteModelFile(&modelInfo, "kong4.txt", "XYZNUVRGBA");

    modelInfo.modelName = "kong5";
    modelInfo.meshPath = "assets/models/dk_3d_all_obj/DonkeyKong_Level_0_Kong_4.ply";
    fileManager->WriteModelFile(&modelInfo, "kong5.txt", "XYZNUVRGBA");

    modelInfo.modelName = "kong6";
    modelInfo.meshPath = "assets/models/dk_3d_all_obj/DonkeyKong_Level_0_Kong_5.ply";
    fileManager->WriteModelFile(&modelInfo, "kong6.txt", "XYZNUVRGBA");

    modelInfo.modelName = "kong7";
    modelInfo.meshPath = "assets/models/dk_3d_all_obj/DonkeyKong_Level_0_Kong_6.ply";
    fileManager->WriteModelFile(&modelInfo, "kong7.txt", "XYZNUVRGBA");

    modelInfo.modelName = "kong8";
    modelInfo.meshPath = "assets/models/dk_3d_all_obj/DonkeyKong_Level_0_Kong_7.ply";
    fileManager->WriteModelFile(&modelInfo, "kong8.txt", "XYZNUVRGBA");

    modelInfo.modelName = "kong9";
    modelInfo.meshPath = "assets/models/dk_3d_all_obj/DonkeyKong_Level_0_Kong_8.ply";
    fileManager->WriteModelFile(&modelInfo, "kong9.txt", "XYZNUVRGBA");

    modelInfo.modelName = "kong10";
    modelInfo.meshPath = "assets/models/dk_3d_all_obj/DonkeyKong_Level_0_Kong_9.ply";
    fileManager->WriteModelFile(&modelInfo, "kong10.txt", "XYZNUVRGBA");

    modelInfo.modelName = "kong11";
    modelInfo.meshPath = "assets/models/dk_3d_all_obj/DonkeyKong_Level_0_Kong_10_Barrel.ply";
    fileManager->WriteModelFile(&modelInfo, "kong11.txt", "XYZNUVRGBA");

    modelInfo.modelName = "kong12";
    modelInfo.meshPath = "assets/models/dk_3d_all_obj/DonkeyKong_Level_0_Kong_11_BlueBarrel.ply";
    fileManager->WriteModelFile(&modelInfo, "kong12.txt", "XYZNUVRGBA");

    modelInfo.modelName = "kong13";
    modelInfo.meshPath = "assets/models/dk_3d_all_obj/DonkeyKong_Level_0_Kong_12_Pauline_0.ply";
    fileManager->WriteModelFile(&modelInfo, "kong13.txt", "XYZNUVRGBA");

    modelInfo.modelName = "kong14";
    modelInfo.meshPath = "assets/models/dk_3d_all_obj/DonkeyKong_Level_0_Kong_13_Pauline_1.ply";
    fileManager->WriteModelFile(&modelInfo, "kong14.txt", "XYZNUV");

    // MARIO
    modelInfo.modelName = "mario1";
    modelInfo.meshPath = "assets/models/dk_3d_all_obj/DonkeyKong_Level_0_Mario_0.ply";
    fileManager->WriteModelFile(&modelInfo, "mario1.txt", "XYZNUVRGBA");

    modelInfo.modelName = "mario2";
    modelInfo.meshPath = "assets/models/dk_3d_all_obj/DonkeyKong_Level_0_Mario_1.ply";
    fileManager->WriteModelFile(&modelInfo, "mario2.txt", "XYZNUVRGBA");

    modelInfo.modelName = "mario3";
    modelInfo.meshPath = "assets/models/dk_3d_all_obj/DonkeyKong_Level_0_Mario_2.ply";
    fileManager->WriteModelFile(&modelInfo, "mario3.txt", "XYZNUVRGBA");

    modelInfo.modelName = "mario4";
    modelInfo.meshPath = "assets/models/dk_3d_all_obj/DonkeyKong_Level_0_Mario_3.ply";
    fileManager->WriteModelFile(&modelInfo, "mario4.txt", "XYZNUVRGBA");

    modelInfo.modelName = "mario5";
    modelInfo.meshPath = "assets/models/dk_3d_all_obj/DonkeyKong_Level_0_Mario_4.ply";
    fileManager->WriteModelFile(&modelInfo, "mario5.txt", "XYZNUVRGBA");

    modelInfo.modelName = "mario6";
    modelInfo.meshPath = "assets/models/dk_3d_all_obj/DonkeyKong_Level_0_Mario_5.ply";
    fileManager->WriteModelFile(&modelInfo, "mario6.txt", "XYZNUVRGBA");

    modelInfo.modelName = "mario7";
    modelInfo.meshPath = "assets/models/dk_3d_all_obj/DonkeyKong_Level_0_Mario_6.ply";
    fileManager->WriteModelFile(&modelInfo, "mario7.txt", "XYZNUVRGBA");

    modelInfo.modelName = "mario8";
    modelInfo.meshPath = "assets/models/dk_3d_all_obj/DonkeyKong_Level_0_Mario_7.ply";
    fileManager->WriteModelFile(&modelInfo, "mario8.txt", "XYZNUV");

    modelInfo.modelName = "mario9";
    modelInfo.meshPath = "assets/models/dk_3d_all_obj/DonkeyKong_Level_0_Mario_8.ply";
    fileManager->WriteModelFile(&modelInfo, "mario9.txt", "XYZNUV");

    modelInfo.modelName = "mario10";
    modelInfo.meshPath = "assets/models/dk_3d_all_obj/DonkeyKong_Level_0_Mario_9.ply";
    fileManager->WriteModelFile(&modelInfo, "mario10.txt", "XYZNUV");

    modelInfo.modelName = "mario11";
    modelInfo.meshPath = "assets/models/dk_3d_all_obj/DonkeyKong_Level_0_Mario_10.ply";
    fileManager->WriteModelFile(&modelInfo, "mario11.txt", "XYZNUV");

    modelInfo.modelName = "mario12";
    modelInfo.meshPath = "assets/models/dk_3d_all_obj/DonkeyKong_Level_0_Mario_11.ply";
    fileManager->WriteModelFile(&modelInfo, "mario12.txt", "XYZNUV");

    modelInfo.modelName = "mario13";
    modelInfo.meshPath = "assets/models/dk_3d_all_obj/DonkeyKong_Level_0_Mario_12.ply";
    fileManager->WriteModelFile(&modelInfo, "mario13.txt", "XYZNUV");

    modelInfo.modelName = "mario14";
    modelInfo.meshPath = "assets/models/dk_3d_all_obj/DonkeyKong_Level_0_Mario_13.ply";
    fileManager->WriteModelFile(&modelInfo, "mario14.txt", "XYZNUV");

    modelInfo.modelName = "mario15";
    modelInfo.meshPath = "assets/models/dk_3d_all_obj/DonkeyKong_Level_0_Mario_14.ply";
    fileManager->WriteModelFile(&modelInfo, "mario15.txt", "XYZNUV");

    modelInfo.modelName = "mario16";
    modelInfo.meshPath = "assets/models/dk_3d_all_obj/DonkeyKong_Level_0_Mario_15.ply";
    fileManager->WriteModelFile(&modelInfo, "mario16.txt", "XYZNUV");

    modelInfo.modelName = "mario17";
    modelInfo.meshPath = "assets/models/dk_3d_all_obj/DonkeyKong_Level_0_Mario_16.ply";
    fileManager->WriteModelFile(&modelInfo, "mario17.txt", "XYZNUV");

    modelInfo.modelName = "mario18";
    modelInfo.meshPath = "assets/models/dk_3d_all_obj/DonkeyKong_Level_0_Mario_17.ply";
    fileManager->WriteModelFile(&modelInfo, "mario18.txt", "XYZNUV");

    // POINTS
    modelInfo.modelName = "points100";
    modelInfo.meshPath = "assets/models/dk_3d_all_obj/DonkeyKong_Level_0_100.ply";
    fileManager->WriteModelFile(&modelInfo, "points100.txt", "XYZNUV");

    modelInfo.modelName = "points200";
    modelInfo.meshPath = "assets/models/dk_3d_all_obj/DonkeyKong_Level_0_200.ply";
    fileManager->WriteModelFile(&modelInfo, "points200.txt", "XYZNUV");

    modelInfo.modelName = "points300";
    modelInfo.meshPath = "assets/models/dk_3d_all_obj/DonkeyKong_Level_0_300.ply";
    fileManager->WriteModelFile(&modelInfo, "points300.txt", "XYZNUV");

    modelInfo.modelName = "points500";
    modelInfo.meshPath = "assets/models/dk_3d_all_obj/DonkeyKong_Level_0_500.ply";
    fileManager->WriteModelFile(&modelInfo, "points500.txt", "XYZNUV");

    modelInfo.modelName = "points800";
    modelInfo.meshPath = "assets/models/dk_3d_all_obj/DonkeyKong_Level_0_800.ply";
    fileManager->WriteModelFile(&modelInfo, "points800.txt", "XYZNUV");

    // BLUE BARREL
    modelInfo.modelName = "blueBarrel0";
    modelInfo.meshPath = "assets/models/dk_3d_all_obj/DonkeyKong_Level_0_BarrelBlue_0.ply";
    fileManager->WriteModelFile(&modelInfo, "blueBarrel0.txt", "XYZNUV");

    modelInfo.modelName = "blueBarrel1";
    modelInfo.meshPath = "assets/models/dk_3d_all_obj/DonkeyKong_Level_0_BarrelBlue_1.ply";
    fileManager->WriteModelFile(&modelInfo, "blueBarrel1.txt", "XYZNUV");

    modelInfo.modelName = "blueBarrel2";
    modelInfo.meshPath = "assets/models/dk_3d_all_obj/DonkeyKong_Level_0_BarrelBlue_2.ply";
    fileManager->WriteModelFile(&modelInfo, "blueBarrel2.txt", "XYZNUV");

    modelInfo.modelName = "blueBarrel3";
    modelInfo.meshPath = "assets/models/dk_3d_all_obj/DonkeyKong_Level_0_BarrelBlue_3.ply";
    fileManager->WriteModelFile(&modelInfo, "blueBarrel3.txt", "XYZNUV");

    modelInfo.modelName = "blueBarrel4";
    modelInfo.meshPath = "assets/models/dk_3d_all_obj/DonkeyKong_Level_0_BarrelBlue_4.ply";
    fileManager->WriteModelFile(&modelInfo, "blueBarrel4.txt", "XYZNUV");

    modelInfo.modelName = "blueBarrel5";
    modelInfo.meshPath = "assets/models/dk_3d_all_obj/DonkeyKong_Level_0_BarrelBlue_5.ply";
    fileManager->WriteModelFile(&modelInfo, "blueBarrel5.txt", "XYZNUV");

    modelInfo.modelName = "barrelStack";
    modelInfo.meshPath = "assets/models/dk_3d_all_obj/DonkeyKong_Level_0_BarrelStack.ply";
    fileManager->WriteModelFile(&modelInfo, "barrelStack.txt", "XYZNUV");

    // BARRELS
    modelInfo.modelName = "barrel0";
    modelInfo.meshPath = "assets/models/dk_3d_all_obj/DonkeyKong_Level_0_Barrel_0.ply";
    fileManager->WriteModelFile(&modelInfo, "barrel0.txt", "XYZNUVRGBA");

    modelInfo.modelName = "barrel1";
    modelInfo.meshPath = "assets/models/dk_3d_all_obj/DonkeyKong_Level_0_Barrel_1.ply";
    fileManager->WriteModelFile(&modelInfo, "barrel1.txt", "XYZNUVRGBA");

    modelInfo.modelName = "barrel2";
    modelInfo.meshPath = "assets/models/dk_3d_all_obj/DonkeyKong_Level_0_Barrel_2.ply";
    fileManager->WriteModelFile(&modelInfo, "barrel2.txt", "XYZNUV");

    modelInfo.modelName = "barrel3";
    modelInfo.meshPath = "assets/models/dk_3d_all_obj/DonkeyKong_Level_0_Barrel_3.ply";
    fileManager->WriteModelFile(&modelInfo, "barrel3.txt", "XYZNUVRGBA");

    modelInfo.modelName = "barrel4";
    modelInfo.meshPath = "assets/models/dk_3d_all_obj/DonkeyKong_Level_0_Barrel_4.ply";
    fileManager->WriteModelFile(&modelInfo, "barrel4.txt", "XYZNUVRGBA");

    modelInfo.modelName = "barrel5";
    modelInfo.meshPath = "assets/models/dk_3d_all_obj/DonkeyKong_Level_0_Barrel_5.ply";
    fileManager->WriteModelFile(&modelInfo, "barrel5.txt", "XYZNUVRGBA");

    modelInfo.modelName = "barrel6";
    modelInfo.meshPath = "assets/models/dk_3d_all_obj/DonkeyKong_Level_0_Barrel_6.ply";
    fileManager->WriteModelFile(&modelInfo, "barrel6.txt", "XYZNUVRGBA");

    //BLUE FLAME ENEMY
    modelInfo.modelName = "blueFlameEnemy0";
    modelInfo.meshPath = "assets/models/dk_3d_all_obj/DonkeyKong_Level_0_BlueFlameEnemy_0.ply";
    fileManager->WriteModelFile(&modelInfo, "blueFlameEnemy0.txt", "XYZNUV");

    modelInfo.modelName = "blueFlameEnemy1";
    modelInfo.meshPath = "assets/models/dk_3d_all_obj/DonkeyKong_Level_0_BlueFlameEnemy_1.ply";
    fileManager->WriteModelFile(&modelInfo, "blueFlameEnemy1.txt", "XYZNUV");

    modelInfo.modelName = "blueFlameEnemy2";
    modelInfo.meshPath = "assets/models/dk_3d_all_obj/DonkeyKong_Level_0_BlueFlameEnemy_2.ply";
    fileManager->WriteModelFile(&modelInfo, "blueFlameEnemy2.txt", "XYZNUV");

    modelInfo.modelName = "blueFlameEnemy3";
    modelInfo.meshPath = "assets/models/dk_3d_all_obj/DonkeyKong_Level_0_BlueFlameEnemy_3.ply";
    fileManager->WriteModelFile(&modelInfo, "blueFlameEnemy3.txt", "XYZNUV");

    // BLUE NUMBERS
    modelInfo.modelName = "blueNumber0";
    modelInfo.meshPath = "assets/models/dk_3d_all_obj/DonkeyKong_Level_0_BlueNumber_0.ply";
    fileManager->WriteModelFile(&modelInfo, "blueNumber0.txt", "XYZNUV");

    modelInfo.modelName = "blueNumber1";
    modelInfo.meshPath = "assets/models/dk_3d_all_obj/DonkeyKong_Level_0_BlueNumber_1.ply";
    fileManager->WriteModelFile(&modelInfo, "blueNumber1.txt", "XYZNUV");

    modelInfo.modelName = "blueNumber2";
    modelInfo.meshPath = "assets/models/dk_3d_all_obj/DonkeyKong_Level_0_BlueNumber_2.ply";
    fileManager->WriteModelFile(&modelInfo, "blueNumber2.txt", "XYZNUV");

    modelInfo.modelName = "blueNumber3";
    modelInfo.meshPath = "assets/models/dk_3d_all_obj/DonkeyKong_Level_0_BlueNumber_3.ply";
    fileManager->WriteModelFile(&modelInfo, "blueNumber3.txt", "XYZNUV");

    modelInfo.modelName = "blueNumber4";
    modelInfo.meshPath = "assets/models/dk_3d_all_obj/DonkeyKong_Level_0_BlueNumber_4.ply";
    fileManager->WriteModelFile(&modelInfo, "blueNumber4.txt", "XYZNUV");

    modelInfo.modelName = "blueNumber5";
    modelInfo.meshPath = "assets/models/dk_3d_all_obj/DonkeyKong_Level_0_BlueNumber_5.ply";
    fileManager->WriteModelFile(&modelInfo, "blueNumber5.txt", "XYZNUV");

    modelInfo.modelName = "blueNumber6";
    modelInfo.meshPath = "assets/models/dk_3d_all_obj/DonkeyKong_Level_0_BlueNumber_6.ply";
    fileManager->WriteModelFile(&modelInfo, "blueNumber6.txt", "XYZNUV");

    modelInfo.modelName = "blueNumber7";
    modelInfo.meshPath = "assets/models/dk_3d_all_obj/DonkeyKong_Level_0_BlueNumber_7.ply";
    fileManager->WriteModelFile(&modelInfo, "blueNumber7.txt", "XYZNUV");

    modelInfo.modelName = "blueNumber8";
    modelInfo.meshPath = "assets/models/dk_3d_all_obj/DonkeyKong_Level_0_BlueNumber_8.ply";
    fileManager->WriteModelFile(&modelInfo, "blueNumber8.txt", "XYZNUV");

    modelInfo.modelName = "blueNumber9";
    modelInfo.meshPath = "assets/models/dk_3d_all_obj/DonkeyKong_Level_0_BlueNumber_9.ply";
    fileManager->WriteModelFile(&modelInfo, "blueNumber9.txt", "XYZNUV");

    // CYAN NUMBERS
    modelInfo.modelName = "cyanNumber0";
    modelInfo.meshPath = "assets/models/dk_3d_all_obj/DonkeyKong_Level_0_CyanNumber_0.ply";
    fileManager->WriteModelFile(&modelInfo, "cyanNumber0.txt", "XYZNUV");

    modelInfo.modelName = "cyanNumber1";
    modelInfo.meshPath = "assets/models/dk_3d_all_obj/DonkeyKong_Level_0_CyanNumber_1.ply";
    fileManager->WriteModelFile(&modelInfo, "cyanNumber1.txt", "XYZNUV");

    modelInfo.modelName = "cyanNumber2";
    modelInfo.meshPath = "assets/models/dk_3d_all_obj/DonkeyKong_Level_0_CyanNumber_2.ply";
    fileManager->WriteModelFile(&modelInfo, "cyanNumber2.txt", "XYZNUV");

    modelInfo.modelName = "cyanNumber3";
    modelInfo.meshPath = "assets/models/dk_3d_all_obj/DonkeyKong_Level_0_CyanNumber_3.ply";
    fileManager->WriteModelFile(&modelInfo, "cyanNumber3.txt", "XYZNUV");

    modelInfo.modelName = "cyanNumber4";
    modelInfo.meshPath = "assets/models/dk_3d_all_obj/DonkeyKong_Level_0_CyanNumber_4.ply";
    fileManager->WriteModelFile(&modelInfo, "cyanNumber4.txt", "XYZNUV");

    modelInfo.modelName = "cyanNumber5";
    modelInfo.meshPath = "assets/models/dk_3d_all_obj/DonkeyKong_Level_0_CyanNumber_5.ply";
    fileManager->WriteModelFile(&modelInfo, "cyanNumber5.txt", "XYZNUV");

    modelInfo.modelName = "cyanNumber6";
    modelInfo.meshPath = "assets/models/dk_3d_all_obj/DonkeyKong_Level_0_CyanNumber_6.ply";
    fileManager->WriteModelFile(&modelInfo, "cyanNumber6.txt", "XYZNUV");

    modelInfo.modelName = "cyanNumber7";
    modelInfo.meshPath = "assets/models/dk_3d_all_obj/DonkeyKong_Level_0_CyanNumber_7.ply";
    fileManager->WriteModelFile(&modelInfo, "cyanNumber7.txt", "XYZNUV");

    modelInfo.modelName = "cyanNumber8";
    modelInfo.meshPath = "assets/models/dk_3d_all_obj/DonkeyKong_Level_0_CyanNumber_8.ply";
    fileManager->WriteModelFile(&modelInfo, "cyanNumber8.txt", "XYZNUV");

    modelInfo.modelName = "cyanNumber9";
    modelInfo.meshPath = "assets/models/dk_3d_all_obj/DonkeyKong_Level_0_CyanNumber_9.ply";
    fileManager->WriteModelFile(&modelInfo, "cyanNumber9.txt", "XYZNUV");

    // EXPLOSION
    modelInfo.modelName = "explosion0";
    modelInfo.meshPath = "assets/models/dk_3d_all_obj/DonkeyKong_Level_0_Explosion_0.ply";
    fileManager->WriteModelFile(&modelInfo, "explosion0.txt", "XYZNUV");

    modelInfo.modelName = "explosion1";
    modelInfo.meshPath = "assets/models/dk_3d_all_obj/DonkeyKong_Level_0_Explosion_1.ply";
    fileManager->WriteModelFile(&modelInfo, "explosion1.txt", "XYZNUV");

    modelInfo.modelName = "explosion2";
    modelInfo.meshPath = "assets/models/dk_3d_all_obj/DonkeyKong_Level_0_Explosion_2.ply";
    fileManager->WriteModelFile(&modelInfo, "explosion2.txt", "XYZNUV");

    modelInfo.modelName = "explosion3";
    modelInfo.meshPath = "assets/models/dk_3d_all_obj/DonkeyKong_Level_0_Explosion_3.ply";
    fileManager->WriteModelFile(&modelInfo, "explosion3.txt", "XYZNUV");

    // FIRE
    modelInfo.modelName = "fire0";
    modelInfo.meshPath = "assets/models/dk_3d_all_obj/DonkeyKong_Level_0_Fire_0.ply";
    fileManager->WriteModelFile(&modelInfo, "fire0.txt", "XYZNUV");

    modelInfo.modelName = "fire1";
    modelInfo.meshPath = "assets/models/dk_3d_all_obj/DonkeyKong_Level_0_Fire_1.ply";
    fileManager->WriteModelFile(&modelInfo, "fire1.txt", "XYZNUV");

    modelInfo.modelName = "fire2";
    modelInfo.meshPath = "assets/models/dk_3d_all_obj/DonkeyKong_Level_0_Fire_2.ply";
    fileManager->WriteModelFile(&modelInfo, "fire2.txt", "XYZNUV");

    modelInfo.modelName = "fire3";
    modelInfo.meshPath = "assets/models/dk_3d_all_obj/DonkeyKong_Level_0_Fire_3.ply";
    fileManager->WriteModelFile(&modelInfo, "fire3.txt", "XYZNUV");

    // FLAME ENEMY
    modelInfo.modelName = "flameEnemy0";
    modelInfo.meshPath = "assets/models/dk_3d_all_obj/DonkeyKong_Level_0_FlameEnemy_0.ply";
    fileManager->WriteModelFile(&modelInfo, "flameEnemy0.txt", "XYZNUV");

    modelInfo.modelName = "flameEnemy1";
    modelInfo.meshPath = "assets/models/dk_3d_all_obj/DonkeyKong_Level_0_FlameEnemy_1.ply";
    fileManager->WriteModelFile(&modelInfo, "flameEnemy1.txt", "XYZNUV");

    modelInfo.modelName = "flameEnemy2";
    modelInfo.meshPath = "assets/models/dk_3d_all_obj/DonkeyKong_Level_0_FlameEnemy_2.ply";
    fileManager->WriteModelFile(&modelInfo, "flameEnemy2.txt", "XYZNUV");

    modelInfo.modelName = "flameEnemy3";
    modelInfo.meshPath = "assets/models/dk_3d_all_obj/DonkeyKong_Level_0_FlameEnemy_3.ply";
    fileManager->WriteModelFile(&modelInfo, "flameEnemy3.txt", "XYZNUV");

    // HAMMER
    modelInfo.modelName = "hammer";
    modelInfo.meshPath = "assets/models/dk_3d_all_obj/DonkeyKong_Level_0_Hammer.ply";
    fileManager->WriteModelFile(&modelInfo, "hammer.txt", "XYZNUV");

    // HEART
    modelInfo.modelName = "heart";
    modelInfo.meshPath = "assets/models/dk_3d_all_obj/DonkeyKong_Level_0_Heart.ply";
    fileManager->WriteModelFile(&modelInfo, "heart.txt", "XYZNUV");

    modelInfo.modelName = "heartBroken";
    modelInfo.meshPath = "assets/models/dk_3d_all_obj/DonkeyKong_Level_0_HeartBroken.ply";
    fileManager->WriteModelFile(&modelInfo, "heartBroken.txt", "XYZNUV");

    // HELP SIGNS
    modelInfo.modelName = "helpCyan";
    modelInfo.meshPath = "assets/models/dk_3d_all_obj/DonkeyKong_Level_0_Help_Cyan.ply";
    fileManager->WriteModelFile(&modelInfo, "helpCyan.txt", "XYZNUV");

    modelInfo.modelName = "helpWhite";
    modelInfo.meshPath = "assets/models/dk_3d_all_obj/DonkeyKong_Level_0_Help_White.ply";
    fileManager->WriteModelFile(&modelInfo, "helpWhite.txt", "XYZNUV");

    // KISS ICONS
    modelInfo.modelName = "kissIcon0";
    modelInfo.meshPath = "assets/models/dk_3d_all_obj/DonkeyKong_Level_0_KissIcon_0.ply";
    fileManager->WriteModelFile(&modelInfo, "kissIcon0.txt", "XYZNUV");

    modelInfo.modelName = "kissIcon1";
    modelInfo.meshPath = "assets/models/dk_3d_all_obj/DonkeyKong_Level_0_KissIcon_1.ply";
    fileManager->WriteModelFile(&modelInfo, "kissIcon1.txt", "XYZNUV");

    modelInfo.modelName = "kissIcon2";
    modelInfo.meshPath = "assets/models/dk_3d_all_obj/DonkeyKong_Level_0_KissIcon_2.ply";
    fileManager->WriteModelFile(&modelInfo, "kissIcon2.txt", "XYZNUV");

    // MARIO HAMMER FRAMES
    modelInfo.modelName = "marioHammer0";
    modelInfo.meshPath = "assets/models/dk_3d_all_obj/DonkeyKong_Level_0_MarioHammer_0.ply";
    fileManager->WriteModelFile(&modelInfo, "marioHammer0.txt", "XYZNUV");

    modelInfo.modelName = "marioHammer1";
    modelInfo.meshPath = "assets/models/dk_3d_all_obj/DonkeyKong_Level_0_MarioHammer_1.ply";
    fileManager->WriteModelFile(&modelInfo, "marioHammer1.txt", "XYZNUV");

    modelInfo.modelName = "marioHammer2";
    modelInfo.meshPath = "assets/models/dk_3d_all_obj/DonkeyKong_Level_0_MarioHammer_2.ply";
    fileManager->WriteModelFile(&modelInfo, "marioHammer2.txt", "XYZNUV");

    modelInfo.modelName = "marioHammer3";
    modelInfo.meshPath = "assets/models/dk_3d_all_obj/DonkeyKong_Level_0_MarioHammer_3.ply";
    fileManager->WriteModelFile(&modelInfo, "marioHammer3.txt", "XYZNUV");

    modelInfo.modelName = "marioHammer4";
    modelInfo.meshPath = "assets/models/dk_3d_all_obj/DonkeyKong_Level_0_MarioHammer_4.ply";
    fileManager->WriteModelFile(&modelInfo, "marioHammer4.txt", "XYZNUV");

    modelInfo.modelName = "marioHammer5";
    modelInfo.meshPath = "assets/models/dk_3d_all_obj/DonkeyKong_Level_0_MarioHammer_5.ply";
    fileManager->WriteModelFile(&modelInfo, "marioHammer5.txt", "XYZNUV");

    modelInfo.modelName = "marioHammer6";
    modelInfo.meshPath = "assets/models/dk_3d_all_obj/DonkeyKong_Level_0_MarioHammer_6.ply";
    fileManager->WriteModelFile(&modelInfo, "marioHammer6.txt", "XYZNUV");

    modelInfo.modelName = "marioHammer7";
    modelInfo.meshPath = "assets/models/dk_3d_all_obj/DonkeyKong_Level_0_MarioHammer_7.ply";
    fileManager->WriteModelFile(&modelInfo, "marioHammer7.txt", "XYZNUV");

    modelInfo.modelName = "marioHammer8";
    modelInfo.meshPath = "assets/models/dk_3d_all_obj/DonkeyKong_Level_0_MarioHammer_8.ply";
    fileManager->WriteModelFile(&modelInfo, "marioHammer8.txt", "XYZNUV");

    modelInfo.modelName = "marioHammer9";
    modelInfo.meshPath = "assets/models/dk_3d_all_obj/DonkeyKong_Level_0_MarioHammer_9.ply";
    fileManager->WriteModelFile(&modelInfo, "marioHammer9.txt", "XYZNUV");

    modelInfo.modelName = "marioHammer10";
    modelInfo.meshPath = "assets/models/dk_3d_all_obj/DonkeyKong_Level_0_MarioHammer_10.ply";
    fileManager->WriteModelFile(&modelInfo, "marioHammer10.txt", "XYZNUV");

    modelInfo.modelName = "marioHammer11";
    modelInfo.meshPath = "assets/models/dk_3d_all_obj/DonkeyKong_Level_0_MarioHammer_11.ply";
    fileManager->WriteModelFile(&modelInfo, "marioHammer11.txt", "XYZNUV");

    // MARIO LITTLE
    modelInfo.modelName = "marioLittle";
    modelInfo.meshPath = "assets/models/dk_3d_all_obj/DonkeyKong_Level_0_MarioLittle.ply";
    fileManager->WriteModelFile(&modelInfo, "marioLittle.txt", "XYZNUV");

    // OIL DRUM
    modelInfo.modelName = "oilDrum0";
    modelInfo.meshPath = "assets/models/dk_3d_all_obj/DonkeyKong_Level_0_OilDrum_0.ply";
    fileManager->WriteModelFile(&modelInfo, "oilDrum0.txt", "XYZNUV");

    modelInfo.modelName = "oilDrum1";
    modelInfo.meshPath = "assets/models/dk_3d_all_obj/DonkeyKong_Level_0_OilDrum_1.ply";
    fileManager->WriteModelFile(&modelInfo, "oilDrum1.txt", "XYZNUV");

    modelInfo.modelName = "oilDrum2";
    modelInfo.meshPath = "assets/models/dk_3d_all_obj/DonkeyKong_Level_0_OilDrum_2.ply";
    fileManager->WriteModelFile(&modelInfo, "oilDrum2.txt", "XYZNUV");

    modelInfo.modelName = "oilDrum3";
    modelInfo.meshPath = "assets/models/dk_3d_all_obj/DonkeyKong_Level_0_OilDrum_3.ply";
    fileManager->WriteModelFile(&modelInfo, "oilDrum3.txt", "XYZNUV");

    modelInfo.modelName = "oilDrum4";
    modelInfo.meshPath = "assets/models/dk_3d_all_obj/DonkeyKong_Level_0_OilDrum_4.ply";
    fileManager->WriteModelFile(&modelInfo, "oilDrum4.txt", "XYZNUV");

    // PAULINE ACCESSORIES
    modelInfo.modelName = "paulineHat";
    modelInfo.meshPath = "assets/models/dk_3d_all_obj/DonkeyKong_Level_0_PaulineHat.ply";
    fileManager->WriteModelFile(&modelInfo, "paulineHat.txt", "XYZNUV");

    modelInfo.modelName = "paulinePurse";
    modelInfo.meshPath = "assets/models/dk_3d_all_obj/DonkeyKong_Level_0_PaulinePurse.ply";
    fileManager->WriteModelFile(&modelInfo, "paulinePurse.txt", "XYZNUV");

    modelInfo.modelName = "paulineUmbrella";
    modelInfo.meshPath = "assets/models/dk_3d_all_obj/DonkeyKong_Level_0_PaulineUmbrella.ply";
    fileManager->WriteModelFile(&modelInfo, "paulineUmbrella.txt", "XYZNUV");

    // PAULINE ANIMATIONS
    modelInfo.modelName = "pauline0";
    modelInfo.meshPath = "assets/models/dk_3d_all_obj/DonkeyKong_Level_0_Pauline_0.ply";
    fileManager->WriteModelFile(&modelInfo, "pauline0.txt", "XYZNUV");

    modelInfo.modelName = "pauline1";
    modelInfo.meshPath = "assets/models/dk_3d_all_obj/DonkeyKong_Level_0_Pauline_1.ply";
    fileManager->WriteModelFile(&modelInfo, "pauline1.txt", "XYZNUV");

    modelInfo.modelName = "pauline2";
    modelInfo.meshPath = "assets/models/dk_3d_all_obj/DonkeyKong_Level_0_Pauline_2.ply";
    fileManager->WriteModelFile(&modelInfo, "pauline2.txt", "XYZNUV");

    modelInfo.modelName = "pauline3";
    modelInfo.meshPath = "assets/models/dk_3d_all_obj/DonkeyKong_Level_0_Pauline_3.ply";
    fileManager->WriteModelFile(&modelInfo, "pauline3.txt", "XYZNUV");

    modelInfo.modelName = "pauline4";
    modelInfo.meshPath = "assets/models/dk_3d_all_obj/DonkeyKong_Level_0_Pauline_4.ply";
    fileManager->WriteModelFile(&modelInfo, "pauline4.txt", "XYZNUV");

    // PIE ANIMATIONS
    modelInfo.modelName = "pie0";
    modelInfo.meshPath = "assets/models/dk_3d_all_obj/DonkeyKong_Level_0_Pie_0.ply";
    fileManager->WriteModelFile(&modelInfo, "pie0.txt", "XYZNUV");

    modelInfo.modelName = "pie1";
    modelInfo.meshPath = "assets/models/dk_3d_all_obj/DonkeyKong_Level_0_Pie_1.ply";
    fileManager->WriteModelFile(&modelInfo, "pie1.txt", "XYZNUV");

    // PLATFORM & LADDER
    modelInfo.modelName = "platformSection0";
    modelInfo.meshPath = "assets/models/dk_3d_all_obj/DonkeyKong_Level_0_PllatformSection.ply";
    fileManager->WriteModelFile(&modelInfo, "platformSection0.txt", "XYZNUV");

    modelInfo.modelName = "whiteLadder";
    modelInfo.meshPath = "assets/models/dk_3d_all_obj/DonkeyKong_Level_0_WhiteLadder.ply";
    fileManager->WriteModelFile(&modelInfo, "whiteLadder.txt", "XYZNUV");

    // RED FIRE ENEMY
    modelInfo.modelName = "redFire0";
    modelInfo.meshPath = "assets/models/dk_3d_all_obj/DonkeyKong_Level_0_RedFire_0.ply";
    fileManager->WriteModelFile(&modelInfo, "redFire0.txt", "XYZNUV");

    modelInfo.modelName = "redFire1";
    modelInfo.meshPath = "assets/models/dk_3d_all_obj/DonkeyKong_Level_0_RedFire_1.ply";
    fileManager->WriteModelFile(&modelInfo, "redFire1.txt", "XYZNUV");

    modelInfo.modelName = "redFire2";
    modelInfo.meshPath = "assets/models/dk_3d_all_obj/DonkeyKong_Level_0_RedFire_2.ply";
    fileManager->WriteModelFile(&modelInfo, "redFire2.txt", "XYZNUV");

    modelInfo.modelName = "redFire3";
    modelInfo.meshPath = "assets/models/dk_3d_all_obj/DonkeyKong_Level_0_RedFire_3.ply";
    fileManager->WriteModelFile(&modelInfo, "redFire3.txt", "XYZNUV");

    // WHITE NUMBERS
    modelInfo.modelName = "whiteNumber0";
    modelInfo.meshPath = "assets/models/dk_3d_all_obj/DonkeyKong_Level_0_WhiteNumber_0.ply";
    fileManager->WriteModelFile(&modelInfo, "whiteNumber0.txt", "XYZNUV");

    modelInfo.modelName = "whiteNumber1";
    modelInfo.meshPath = "assets/models/dk_3d_all_obj/DonkeyKong_Level_0_WhiteNumber_1.ply";
    fileManager->WriteModelFile(&modelInfo, "whiteNumber1.txt", "XYZNUV");

    modelInfo.modelName = "whiteNumber2";
    modelInfo.meshPath = "assets/models/dk_3d_all_obj/DonkeyKong_Level_0_WhiteNumber_2.ply";
    fileManager->WriteModelFile(&modelInfo, "whiteNumber2.txt", "XYZNUV");

    modelInfo.modelName = "whiteNumber3";
    modelInfo.meshPath = "assets/models/dk_3d_all_obj/DonkeyKong_Level_0_WhiteNumber_3.ply";
    fileManager->WriteModelFile(&modelInfo, "whiteNumber3.txt", "XYZNUV");

    modelInfo.modelName = "whiteNumber4";
    modelInfo.meshPath = "assets/models/dk_3d_all_obj/DonkeyKong_Level_0_WhiteNumber_4.ply";
    fileManager->WriteModelFile(&modelInfo, "whiteNumber4.txt", "XYZNUV");

    modelInfo.modelName = "whiteNumber5";
    modelInfo.meshPath = "assets/models/dk_3d_all_obj/DonkeyKong_Level_0_WhiteNumber_5.ply";
    fileManager->WriteModelFile(&modelInfo, "whiteNumber5.txt", "XYZNUV");

    modelInfo.modelName = "whiteNumber6";
    modelInfo.meshPath = "assets/models/dk_3d_all_obj/DonkeyKong_Level_0_WhiteNumber_6.ply";
    fileManager->WriteModelFile(&modelInfo, "whiteNumber6.txt", "XYZNUV");

    modelInfo.modelName = "whiteNumber7";
    modelInfo.meshPath = "assets/models/dk_3d_all_obj/DonkeyKong_Level_0_WhiteNumber_7.ply";
    fileManager->WriteModelFile(&modelInfo, "whiteNumber7.txt", "XYZNUV");

    modelInfo.modelName = "whiteNumber8";
    modelInfo.meshPath = "assets/models/dk_3d_all_obj/DonkeyKong_Level_0_WhiteNumber_8.ply";
    fileManager->WriteModelFile(&modelInfo, "whiteNumber8.txt", "XYZNUV");

    modelInfo.modelName = "whiteNumber9";
    modelInfo.meshPath = "assets/models/dk_3d_all_obj/DonkeyKong_Level_0_WhiteNumber_9.ply";
    fileManager->WriteModelFile(&modelInfo, "whiteNumber9.txt", "XYZNUV");

    // LEVEL 1 OBJECTS
    modelInfo.modelName = "elevatorMotor";
    modelInfo.meshPath = "assets/models/dk_3d_all_obj/DonkeyKong_Level_1_ElevatorMotor.ply";
    fileManager->WriteModelFile(&modelInfo, "elevatorMotor.txt", "XYZNUV");

    modelInfo.modelName = "flameEnemyBlue0";
    modelInfo.meshPath = "assets/models/dk_3d_all_obj/DonkeyKong_Level_1_FlameEnemyBlue_0.ply";
    fileManager->WriteModelFile(&modelInfo, "flameEnemyBlue0.txt", "XYZNUV");

    modelInfo.modelName = "flameEnemyBlue1";
    modelInfo.meshPath = "assets/models/dk_3d_all_obj/DonkeyKong_Level_1_FlameEnemyBlue_1.ply";
    fileManager->WriteModelFile(&modelInfo, "flameEnemyBlue1.txt", "XYZNUV");

    modelInfo.modelName = "flameEnemyRed0";
    modelInfo.meshPath = "assets/models/dk_3d_all_obj/DonkeyKong_Level_1_FlameEnemyRed_0.ply";
    fileManager->WriteModelFile(&modelInfo, "flameEnemyRed0.txt", "XYZNUV");

    modelInfo.modelName = "flameEnemyRed1";
    modelInfo.meshPath = "assets/models/dk_3d_all_obj/DonkeyKong_Level_1_FlameEnemyRed_1.ply";
    fileManager->WriteModelFile(&modelInfo, "flameEnemyRed1.txt", "XYZNUV");

    modelInfo.modelName = "platformSection1";
    modelInfo.meshPath = "assets/models/dk_3d_all_obj/DonkeyKong_Level_1_PlatformSection.ply";
    fileManager->WriteModelFile(&modelInfo, "platformSection1.txt", "XYZNUV");

    modelInfo.modelName = "rivet";
    modelInfo.meshPath = "assets/models/dk_3d_all_obj/DonkeyKong_Level_1_Rivet.ply";
    fileManager->WriteModelFile(&modelInfo, "rivet.txt", "XYZNUV");

    modelInfo.modelName = "spring0";
    modelInfo.meshPath = "assets/models/dk_3d_all_obj/DonkeyKong_Level_1_Spring_0.ply";
    fileManager->WriteModelFile(&modelInfo, "spring0.txt", "XYZNUV");

    modelInfo.modelName = "spring1";
    modelInfo.meshPath = "assets/models/dk_3d_all_obj/DonkeyKong_Level_1_Spring_1.ply";
    fileManager->WriteModelFile(&modelInfo, "spring1.txt", "XYZNUV");


    //SelectBox
    modelInfo.modelName = "Select_Box";
    modelInfo.meshPath = "assets/models/Cube_xyz_n_uv.ply";
    fileManager->WriteModelFile(&modelInfo, "selectBox.txt", "XYZNUV");

    // end of question 1 creating models


    // Read the model from the file (assuming the file exists)
   // Read the model from the file (assuming the file exists)
    sModelDrawInfo readModel = fileManager->ReadModelFile("bunny.txt");

    // Output the result to verify the correct reading
    std::cout << "Model Name: " << readModel.modelName << std::endl;
    std::cout << "Mesh Path: " << readModel.meshPath << std::endl;

    // Read the scene from the file (assuming the file exists)
    Scene* scene = fileManager->ReadSceneFile("SaveScene.txt");


    for (Object* object : scene->sceneObjects)
    {
        std::cout << object->name << std::endl;
    }


    glfwSetErrorCallback(error_callback);

    if (!glfwInit())
        exit(EXIT_FAILURE);


    ///I really want to put this into a function, vut IDK how
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //

    GLFWwindow* window = glfwCreateWindow(640, 480, "OpenGL Triangle", NULL, NULL);

    GLuint program = PrepareOpenGL(window);





    // Loading the TYPES of models I can draw...

    ::g_pMeshManager = new cVAOManager();


    PhysicsManager* physicsMan = new PhysicsManager();


    PreparePhysics();


    //// Prepare the camera
    //PrepareFlyCamera(); // This should initialize `g_pFlyCamera`

    //// Initialize scene and player object
    //Object* playerObject = new Object();
    //playerObject->startTranform = new Transform();
    //playerObject->startTranform->position = glm::vec3(0.0f, 0.0f, 0.0f); // Set initial position

    //// Adding player object to scene
    //scene->sceneObjects.push_back(playerObject);

    //// Set up camera offset and attach `aPlayerCamera`
    //glm::vec3 cameraOffset(0.0f, 5.0f, -10.0f); // Customize offset as needed
    //aPlayerCamera* playerCameraAction = new aPlayerCamera(g_pFlyCamera, cameraOffset);

    //// Add `aPlayerCamera` action to `playerObject`
    //scene->AddActionToObj(playerCameraAction, playerObject);
    Object* playerObject = scene->sceneObjects[1];

    PrepareFlyCamera();

    // Add the player camera action (with an offset for camera positioning)
    aPlayerCamera* playerCameraAction = new aPlayerCamera(::g_pFlyCamera, glm::vec3(0.0f, 10.0f, 0.0f));
    scene->AddActionToObj(playerCameraAction, playerObject);




    scene->Prepare(g_pMeshManager, program, g_vecMeshesToDraw, physicsMan, window, g_pFlyCamera);
    physicsMan->AddTriangleMesh("assets/models/Cube_xyz_n_uv.ply", scene->sceneObjects[0]->startTranform->position, scene->sceneObjects[0]->startTranform->rotation, scene->sceneObjects[0]->startTranform->scale.x);






    RayCastPhysics* phys = new RayCastPhysics;
    phys->gravityAcceleration.y = -5;
    phys->baseRayCastLength = 10.0;
    scene->AddActionToObj(phys, scene->sceneObjects[1]);



    aDrawAim* drawAimAction = new aDrawAim();
    drawAimAction->program = program;
    scene->AddActionToObj(drawAimAction, scene->sceneObjects[1]);


    aPlayerItemsController* itemsControllerAction = new aPlayerItemsController();

    scene->AddActionToObj(itemsControllerAction, scene->sceneObjects[1]);




    //MoveForward* action = new MoveForward();

    //scene->AddActionToObj(action, scene->sceneObjects[0]);

    //MoveForward* action2 = new MoveForward();
    //scene->AddActionToObj(action2, scene->sceneObjects[1]);

   /* ExplosionLogic* action = new ExplosionLogic();

    scene->AddActionToObj(action, scene->sceneObjects[1]);

    aMoveXYZSpeed* xyzSpeed = new aMoveXYZSpeed();
    scene->AddActionToObj(xyzSpeed, scene->sceneObjects[1]);
    xyzSpeed->speed = glm::vec3(-0.05, 0, 0);
    */




    // AddModelsToScene();



    glUseProgram(program);

    // Enable depth buffering (z buffering)
    // https://registry.khronos.org/OpenGL-Refpages/gl4/html/glEnable.xhtml
    glEnable(GL_DEPTH_TEST);

    double currentFrameTime = glfwGetTime();
    double lastFrameTime = glfwGetTime();


    // Set up the lights
    //I'll do this for now, but we better remove g_pLightManager and just use scene.lightManager.
    //DONE: Do that
    //scene->lightManager;
    ::g_pLightManager = scene->lightManager;
    // Called only once
    scene->lightManager->loadUniformLocations(program);

    cLightHelper TheLightHelper;



    SceneEditor* sceneEditor = new SceneEditor();
    cLightManager* lightManager = new cLightManager;

    sceneEditor->Start("selectBox.txt", fileManager, program, window, g_pMeshManager, scene);

    MazeGenerator* mazeGenerator = new MazeGenerator("assets/models/maze.txt", scene, lightManager);

    mazeGenerator->generateMaze();


    scene->programs.push_back(program);





    ////SCENE SETUP
    for (Object* object : scene->sceneObjects)
        if (object->name == "platformLong")
            scene->physicsManager->AddTriangleMesh("assets/models/DonkeyKong_Level_1_PlatformSectionLong.ply", object->mesh->positionXYZ, object->mesh->rotationEulerXYZ, object->mesh->uniformScale);

    Object* player = scene->sceneObjects[2];
    aRayCastPhysics2D* physics2D = new aRayCastPhysics2D();
    physics2D->gravityAcceleration = glm::vec3(0, -0.18f, 0);
    physics2D->baseRayCastLength = 2.f;
    scene->AddActionToObj(physics2D, player);

    //physics2D->speed = glm::vec3(0.f, 0.f, 1.f);
    Object* ground = scene->sceneObjects[3];
    Object* ladder = scene->sceneObjects[4];
    Object* kong = scene->sceneObjects[5];









    aModelsFramesAnimator* kongAnimator = new aModelsFramesAnimator();
    kongAnimator->animationSpeed = 3;
    std::vector<std::string> kongFrames0 = { "assets/models/dk_3d_all_obj/DonkeyKong_Level_0_Kong_0.ply" };

    std::vector<std::string> kongFrames1 = { "assets/models/dk_3d_all_obj/DonkeyKong_Level_0_Kong_1.ply" ,
                                             "assets/models/dk_3d_all_obj/DonkeyKong_Level_0_Kong_2.ply" ,
                                             "assets/models/dk_3d_all_obj/DonkeyKong_Level_0_Kong_3.ply" };

    std::vector<std::string> kongFrames2 = { "assets/models/dk_3d_all_obj/DonkeyKong_Level_0_Kong_4.ply"
                                           ,"assets/models/dk_3d_all_obj/DonkeyKong_Level_0_Kong_5.ply"
                                           ,"assets/models/dk_3d_all_obj/DonkeyKong_Level_0_Kong_6.ply"
                                           ,"assets/models/dk_3d_all_obj/DonkeyKong_Level_0_Kong_7.ply"
                                           ,"assets/models/dk_3d_all_obj/DonkeyKong_Level_0_Kong_8.ply"
                                           ,"assets/models/dk_3d_all_obj/DonkeyKong_Level_0_Kong_9.ply" };

    kongAnimator->animations.push_back(kongFrames0);
    kongAnimator->animations.push_back(kongFrames1);
    kongAnimator->animations.push_back(kongFrames2);

    scene->AddActionToObj(kongAnimator, kong);







    Climb* climbAction = new Climb();
    climbAction->climbSpeed = 85.0f;
    climbAction->physics = physics2D;
    scene->AddActionToObj(climbAction, player);

    physics2D->gravityAcceleration = glm::vec3(0, -0.18f, 0);
    physics2D->baseRayCastLength = 2.f;
    scene->AddActionToObj(physics2D, player);



    aModelsFramesAnimator* marioAnimator = new aModelsFramesAnimator();
    marioAnimator->animationSpeed = 5;
    std::vector<std::string> marioFrames0 = { "assets/models/dk_3d_all_obj/DonkeyKong_Level_0_Mario_0.ply" };

    std::vector<std::string> marioFrames1 = { "assets/models/dk_3d_all_obj/DonkeyKong_Level_0_Mario_1.ply" ,
                                             "assets/models/dk_3d_all_obj/DonkeyKong_Level_0_Mario_2.ply" ,
    };

    std::vector<std::string> marioFrames2 = { };

    marioAnimator->animations.push_back(marioFrames0);
    marioAnimator->animations.push_back(marioFrames1);
    marioAnimator->animations.push_back(marioFrames2);

    scene->AddActionToObj(marioAnimator, player);

    aPlayerMovement2D* playerMovement2D = new aPlayerMovement2D();
    playerMovement2D->animator = marioAnimator;

    scene->AddActionToObj(playerMovement2D, player);
    scene->physicsManager->AddTriangleMesh("assets/models/DonkeyKong_Level_0_base.ply", ground->mesh->positionXYZ, ground->mesh->rotationEulerXYZ, ground->mesh->uniformScale);
    scene->physicsManager->AddTriangleMesh("assets/models/DonkeyKong_Level_0_Ladders.ply", ladder->mesh->positionXYZ, ladder->mesh->rotationEulerXYZ, ladder->mesh->uniformScale);

    bool barrelSpawned = false;
    bool kongAnimChanged = false;
    float spawnBerrelTime = 3.75f;
    float restartTimerTime = 4.f;
    float SpawnTimer = 0;
    float kongAnimationTime = 3.f;

    BarrelFactory factory;
    factory.scene = scene;


    bool kongChangge = false;
    //START
    scene->Start();
    //LOOP
    while (!glfwWindowShouldClose(window))
    {


        //KINDA SCENE GLOBAL UPDATE
        //Better put this all to action later
        SpawnTimer += scene->deltaTime;

        if (SpawnTimer > kongAnimationTime)
        {
            if (!kongAnimChanged)
            {
                kongAnimator->ChangeAnimation(1);
                kongAnimChanged = true;
            }

        }

        if (SpawnTimer > spawnBerrelTime)
        {
            if (!barrelSpawned)
            {
                factory.SpawnRegularBarrel();
                barrelSpawned = true;
            }
        }

        if (SpawnTimer > restartTimerTime)
        {
            SpawnTimer = 0;

            kongAnimator->ChangeAnimation(0);
            barrelSpawned = false;
            kongAnimChanged = false;

        }




        //std::cout << player->mesh->positionXYZ.x << " " << player->mesh->positionXYZ.y << " " << player->mesh->positionXYZ.x << std::endl;
        //std::cout << physics2D->speed.x << " " << physics2D->speed.y  << " " << physics2D->speed.z << std::endl;
        std::cout << scene->deltaTime << std::endl;

        float ratio;
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        ratio = width / (float)height;
        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);




        SetCameraAndProjectionMatrices(ratio, program);

        //        // *******************************************************************


                // Update the light info in the shader
                // (Called every frame)
        scene->lightManager->updateShaderWithLightInfo();
        // *******************************************************************


        //    ____                       _                      
        //   |  _ \ _ __ __ ___      __ | |    ___   ___  _ __  
        //   | | | | '__/ _` \ \ /\ / / | |   / _ \ / _ \| '_ \ 
        //   | |_| | | | (_| |\ V  V /  | |__| (_) | (_) | |_) |
        //   |____/|_|  \__,_| \_/\_/   |_____\___/ \___/| .__/ 
        //                                               |_|    
        // Draw all the objects
        //for (unsigned int meshIndex = 0; meshIndex != ::g_NumberOfMeshesToDraw; meshIndex++)

        sceneEditor->Update();
        scene->Update();



        for (Object* object : scene->sceneObjects)
        {
            //            sMesh* pCurMesh = ::g_myMeshes[meshIndex];
            sMesh* pCurMesh = object->mesh;

            DrawMesh(pCurMesh, program);

        }//for (unsigned int meshIndex..




        //DrawLazer(program);


        // **********************************************************************************
        if (::g_bShowDebugSpheres)
        {
            DrawDebugObjects(TheLightHelper, program, g_pLightManager);
        }
        // **********************************************************************************




        // Calculate elapsed time
        // We'll enhance this
        currentFrameTime = glfwGetTime();
        double deltaTime = currentFrameTime - lastFrameTime;
        lastFrameTime = currentFrameTime;


        //  UpdateBallShadow();

          // Physic update and test 
        //  ::g_pPhysicEngine->StepTick(deltaTime);

         // HandleCollisions();




          // Point the spot light to the ball
        sMesh* pBouncy_5_Ball = pFindMeshByFriendlyName("Bouncy_5");
        if (pBouncy_5_Ball)
        {
            glm::vec3 directionToBal
                = pBouncy_5_Ball->positionXYZ - glm::vec3(scene->lightManager->theLights[1].position);

            // Normalize to get the direction only
            directionToBal = glm::normalize(directionToBal);

            // Point the spot light at the bouncy ball
            scene->lightManager->theLights[1].direction = glm::vec4(directionToBal, 1.0f);
        }






        // Handle async IO stuff
        handleKeyboardAsync(window, scene);
        handleMouseAsync(window);

        glfwSwapBuffers(window);
        glfwPollEvents();

        UpdateWindowTitle(window, g_pLightManager);



    }// End of the draw loop


    // Delete everything
    delete ::g_pFlyCamera;
    delete ::g_pPhysicEngine;

    glfwDestroyWindow(window);

    glfwTerminate();
    exit(EXIT_SUCCESS);
}

//DONE: Add this mesh to vector with all meshes on the screen + return this sMesh*
//return and push_back
sMesh* GenerateMeshObjects(std::string filePath, glm::vec3 posXYZ, glm::vec3 rotXYZ,
    bool bOverrideColor, glm::vec4 objectColor, bool bDoLightingExist)
{
    sMesh* Meshes = new sMesh();
    Meshes->modelFileName = filePath;
    Meshes->positionXYZ = posXYZ;
    Meshes->rotationEulerXYZ = rotXYZ;
    Meshes->bOverrideObjectColour = bOverrideColor;
    Meshes->objectColourRGBA = objectColor;

    // Set lighting based on the parameter
    Meshes->bDoNotLight = !bDoLightingExist;

    // Add this mesh to the global vector of meshes to draw
    ::g_vecMeshesToDraw.push_back(Meshes);

    return Meshes;
}




//using namespace std;

void ConsoleStuff(void)
{


    std::ifstream myFile2("assets/models/bun_zipper_res3.ply");
    if (myFile2.is_open())
    {

        std::string aword;
        while (myFile2 >> aword)
        {
            std::cout << aword << std::endl;
        };
    }
    else
    {
        std::cout << "Can't find file" << std::endl;
    }


    // iostream
    std::cout << "Type a number:" << std::endl;

    int x = 0;
    std::cin >> x;

    std::cout << "You typed: " << x << std::endl;

    std::cout << "Type your name:" << std::endl;
    std::string name;
    std::cin >> name;

    std::cout << "Hello " << name << std::endl;
    return;
}