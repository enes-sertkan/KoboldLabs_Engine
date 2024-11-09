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


#include "aRayCastPhysics.h"
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

void DrawRay(glm::vec3 pos, glm::vec3 posEnd, GLuint program)
{
    float distance = glm::distance(pos, posEnd);
    glm::vec3 direction = glm::normalize(posEnd - pos);
    glm::vec3 movingPoint=pos;
    while (glm::distance(pos, movingPoint) < distance)
    {
        // Move the next ball 0.1 times the normalized camera direction
        movingPoint += (direction * 0.10f);
        DrawDebugSphere(movingPoint, glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), 0.05f, program);
    }

}



GLuint PrepareOpenGL(GLFWwindow* const &window)
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

sModelDrawInfo LoadPlyModel(std::string modelPath,GLuint program)
{
    sModelDrawInfo modelInfo;
    ::g_pMeshManager->LoadModelIntoVAO(modelPath,
        modelInfo, program);
    std::cout <<modelInfo.meshPath<< "-Loaded"<< std::endl << modelInfo.numberOfVertices << " vertices loaded" << std::endl;
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




void DrawDebugObjects(cLightHelper TheLightHelper ,GLuint program, cLightManager* lightManager)
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

    modelInfo.modelName = "Hangar";
    modelInfo.meshPath = "assets/models/SM_Bld_Hanger_01_xyz_n_uv.ply";
    fileManager->WriteModelFile(&modelInfo, "Hangar.txt", "XYZNUV");

    modelInfo.modelName = "wall";
    modelInfo.meshPath = "assets/models/Ply/SM_Env_Wall_02_xyz_n_rgba_uv.ply";
    fileManager->WriteModelFile(&modelInfo, "wall.txt", "XYZNUVRGBA");

    modelInfo.modelName = "rotatedWall";
    modelInfo.meshPath = "assets/models/Ply/SM_Env_Wall_02_xyz_n_rgba_uv.ply";
    fileManager->WriteModelFile(&modelInfo, "rotatedWall.txt", "XYZNUVRGBA");

    modelInfo.modelName = "floor";
    modelInfo.meshPath = "assets/models/Ply/SM_Env_Floor_01_xyz_n_rgba_uv.ply";
    fileManager->WriteModelFile(&modelInfo, "floor.txt", "XYZNUVRGBA");

    modelInfo.modelName = "ceiling";
    modelInfo.meshPath = "assets/models/Ply/SM_Env_Ceiling_01_xyz_n_rgba_uv.ply";
    fileManager->WriteModelFile(&modelInfo, "ceiling.txt", "XYZNUVRGBA");

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
        std::cout<<object->name<<std::endl;
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






    scene->Prepare(g_pMeshManager, program, g_vecMeshesToDraw, physicsMan, window, g_pFlyCamera);
    physicsMan->AddTriangleMesh("assets/models/Cube_xyz_n_uv.ply", scene->sceneObjects[0]->startTranform->position, scene->sceneObjects[0]->startTranform->rotation, scene->sceneObjects[0]->startTranform->scale.x);


    // Add the player camera action (with an offset for camera positioning)
    aPlayerCamera* playerCameraAction = new aPlayerCamera(::g_pFlyCamera, glm::vec3(0.0f, 2.0f, 0.0f));
    scene->AddActionToObj(playerCameraAction, playerObject);
   


    aPlayerMovement* playerMovement = new aPlayerMovement();
    playerMovement->program = program;
    scene->AddActionToObj(playerMovement, scene->sceneObjects[1]);

    RayCastPhysics* phys = new RayCastPhysics;
    phys->gravityAcceleration.y = -0.02;
    phys->baseRayCastLength =  2.0;
    scene->AddActionToObj(phys, scene->sceneObjects[1]);


    RayCastPhysics* phys2 = new RayCastPhysics;
    phys2->gravityAcceleration.y = -0.01;
    phys2->gravityAcceleration.z = -0.005;
    phys2->baseRayCastLength = 1.f;
    scene->AddActionToObj(phys2, scene->sceneObjects[2]);



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

    sceneEditor->Start("selectBox.txt",fileManager, program, window, g_pMeshManager, scene);

    MazeGenerator* mazeGenerator = new MazeGenerator("assets/models/maze.txt", scene);
  
    mazeGenerator->generateMaze();

    while (!glfwWindowShouldClose(window))
    {
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

        glm::vec3 pos = scene->sceneObjects[1]->mesh->positionXYZ;
        glm::vec3 posEnd = scene->sceneObjects[1]->mesh->positionXYZ;

        std::vector<sCollision_RayTriangleInMesh> collisions;
        posEnd.x = -2;
        if (physicsMan->RayCast(pos, posEnd, collisions, false)) {}//printf("HIT!\n");
        for (auto col : collisions)
        {
           // printf("HIT!\n");
        }

        posEnd = pos;
        posEnd.x -= 2;

        DrawRay(pos, posEnd, program);

        for (Object* object:scene->sceneObjects)
        {
            //            sMesh* pCurMesh = ::g_myMeshes[meshIndex];
            sMesh* pCurMesh = object->mesh;

            DrawMesh(pCurMesh, program);

        }//for (unsigned int meshIndex..



      
        //DrawLazer(program);


        // **********************************************************************************
        if (::g_bShowDebugSpheres)
        {
            DrawDebugObjects(TheLightHelper,program, g_pLightManager);
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