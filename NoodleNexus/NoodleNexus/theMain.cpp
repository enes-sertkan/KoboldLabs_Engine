//#define GLAD_GL_IMPLEMENTATION
//#include <glad/glad.h>
//
//#define GLFW_INCLUDE_NONE
//#include <GLFW/glfw3.h>
#include "GLCommon.h"

//#include "linmath.h"
#include <glm/glm.hpp>
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> 
// glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr

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

#include "SceneEditor.h"

std::vector<sMesh*> g_vecMeshesToDraw;

cPhysics* g_pPhysicEngine = NULL;
// This loads the 3D models for drawing, etc.
cVAOManager* g_pMeshManager = NULL;


void AddModelsToScene(void);

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
    ::g_pFlyCamera->setEyeLocation(glm::vec3(0.0f, 10.0f, 50.0f));
    // Rotate the camera 180 degrees
    ::g_pFlyCamera->rotateLeftRight_Yaw_NoScaling(glm::radians(180.0f));

}



//TODO: Pick Better Name
void UpdateMatricies(float ratio, GLuint program)
{
//        glm::mat4 m, p, v, mvp;
glm::mat4 matProjection = glm::mat4(1.0f);

matProjection = glm::perspective(0.6f,           // FOV
    ratio,          // Aspect ratio of screen
    0.1f,           // Near plane
    1000.0f);       // Far plane

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

void SetLight(int index,
    const glm::vec4& position,
    const glm::vec4& diffuse,
    const glm::vec3& attenuation,
    const glm::vec4& direction,
    const glm::vec3& param1,
    float param2x)
{
    // Set the properties of the light
    ::g_pLightManager->theLights[index].position = position;
    ::g_pLightManager->theLights[index].diffuse = diffuse;
    ::g_pLightManager->theLights[index].atten.y = attenuation.y;
    ::g_pLightManager->theLights[index].atten.z = attenuation.z;

    // If it's a spotlight, set the direction and angles
    ::g_pLightManager->theLights[index].direction = direction;
    ::g_pLightManager->theLights[index].param1 = glm::vec4(param1, 0.0f);
    ::g_pLightManager->theLights[index].param2.x = param2x;  // Turn on/off
}


void DrawDebugObjects(cLightHelper TheLightHelper ,GLuint program)
{


    DrawDebugSphere(::g_pLightManager->theLights[::g_selectedLightIndex].position,
        glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), 0.1f, program);

    const float DEBUG_LIGHT_BRIGHTNESS = 0.3f;

    const float ACCURACY = 0.1f;       // How many units distance
    float distance_75_percent =
        TheLightHelper.calcApproxDistFromAtten(0.75f, ACCURACY, FLT_MAX,
            ::g_pLightManager->theLights[::g_selectedLightIndex].atten.x,   // Const attent
            ::g_pLightManager->theLights[::g_selectedLightIndex].atten.y,   // Linear attenuation
            ::g_pLightManager->theLights[::g_selectedLightIndex].atten.z);  // Quadratic attenuation

    DrawDebugSphere(::g_pLightManager->theLights[::g_selectedLightIndex].position,
        glm::vec4(DEBUG_LIGHT_BRIGHTNESS, 0.0f, 0.0f, 1.0f),
        distance_75_percent,
        program);


    float distance_50_percent =
        TheLightHelper.calcApproxDistFromAtten(0.5f, ACCURACY, FLT_MAX,
            ::g_pLightManager->theLights[::g_selectedLightIndex].atten.x,   // Const attent
            ::g_pLightManager->theLights[::g_selectedLightIndex].atten.y,   // Linear attenuation
            ::g_pLightManager->theLights[::g_selectedLightIndex].atten.z);  // Quadratic attenuation

    DrawDebugSphere(::g_pLightManager->theLights[::g_selectedLightIndex].position,
        glm::vec4(0.0f, DEBUG_LIGHT_BRIGHTNESS, 0.0f, 1.0f),
        distance_50_percent,
        program);

    float distance_25_percent =
        TheLightHelper.calcApproxDistFromAtten(0.25f, ACCURACY, FLT_MAX,
            ::g_pLightManager->theLights[::g_selectedLightIndex].atten.x,   // Const attent
            ::g_pLightManager->theLights[::g_selectedLightIndex].atten.y,   // Linear attenuation
            ::g_pLightManager->theLights[::g_selectedLightIndex].atten.z);  // Quadratic attenuation

    DrawDebugSphere(::g_pLightManager->theLights[::g_selectedLightIndex].position,
        glm::vec4(0.0f, 0.0f, DEBUG_LIGHT_BRIGHTNESS, 1.0f),
        distance_25_percent,
        program);

    float distance_05_percent =
        TheLightHelper.calcApproxDistFromAtten(0.05f, ACCURACY, FLT_MAX,
            ::g_pLightManager->theLights[::g_selectedLightIndex].atten.x,   // Const attent
            ::g_pLightManager->theLights[::g_selectedLightIndex].atten.y,   // Linear attenuation
            ::g_pLightManager->theLights[::g_selectedLightIndex].atten.z);  // Quadratic attenuation

    DrawDebugSphere(::g_pLightManager->theLights[::g_selectedLightIndex].position,
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

void UpdateWindowTitle(GLFWwindow* window)
{

    //std::cout << "Camera: "
    std::stringstream ssTitle;
    ssTitle << "Camera: "
        << ::g_pFlyCamera->getEyeLocation().x << ", "
        << ::g_pFlyCamera->getEyeLocation().y << ", "
        << ::g_pFlyCamera->getEyeLocation().z
        << "   ";
    ssTitle << "light[" << g_selectedLightIndex << "] "
        << ::g_pLightManager->theLights[g_selectedLightIndex].position.x << ", "
        << ::g_pLightManager->theLights[g_selectedLightIndex].position.y << ", "
        << ::g_pLightManager->theLights[g_selectedLightIndex].position.z
        << "   "
        << "linear: " << ::g_pLightManager->theLights[0].atten.y
        << "   "
        << "quad: " << ::g_pLightManager->theLights[0].atten.z;

    //        glfwSetWindowTitle(window, "Hey!");
    glfwSetWindowTitle(window, ssTitle.str().c_str());
}

int main(void)
{
    // Instantiate KLFileManager
    KLFileManager fileManager;

    // Create an sModelDrawInfo object for testing
    sModelDrawInfo modelInfo;



    modelInfo.modelName = "cube";
    modelInfo.meshPath = "assets/models/Old_vip_Res2_xyz_n_uv.ply";
        // Call WriteModelFile to save the model info
    fileManager.WriteModelFile(&modelInfo, "cube2.txt", "XYZNUV");


    modelInfo.modelName = "bunny";
    modelInfo.meshPath = "assets/models/Old_vip_Res2_xyz_n_uv.ply";
    // Call WriteModelFile to save the model info
    fileManager.WriteModelFile(&modelInfo, "bunny.txt", "XYZNUV");

    modelInfo.modelName = "Battleship_Big";
    modelInfo.meshPath = "assets/models/Old_vip_Res2_xyz_n_uv.ply";
    // Call WriteModelFile to save the model info
    fileManager.WriteModelFile(&modelInfo, "Battleship_Big.txt", "XYZNUV");


    // Read the model from the file (assuming the file exists)
   // Read the model from the file (assuming the file exists)
    sModelDrawInfo readModel = fileManager.ReadModelFile("bunny.txt");

    // Output the result to verify the correct reading
    std::cout << "Model Name: " << readModel.modelName << std::endl;
    std::cout << "Mesh Path: " << readModel.meshPath << std::endl;

    // Read the scene from the file (assuming the file exists)
    Scene* scene = fileManager.ReadSceneFile("SaveScene.txt");



    fileManager.WriteSceneFile(scene, "room.txt");




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


    scene->Prepare(g_pMeshManager, program, g_vecMeshesToDraw);





    PreparePhysics();

    PrepareFlyCamera();

   // AddModelsToScene();



    glUseProgram(program);

    // Enable depth buffering (z buffering)
    // https://registry.khronos.org/OpenGL-Refpages/gl4/html/glEnable.xhtml
    glEnable(GL_DEPTH_TEST);

    double currentFrameTime = glfwGetTime();
    double lastFrameTime = glfwGetTime();


    // Set up the lights
    //I'll do this for now, but we better remove g_pLightManager and just use scene.lightManager.
    //TODO: Do that
    ::g_pLightManager = scene->lightManager;
    // Called only once
    ::g_pLightManager->loadUniformLocations(program);


    // light 01
  /*  SetLight(0,
        glm::vec4(-15.0f, 30.0f, 0.0f, 1.0f),
        glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
        glm::vec3(NULL, 0.01f, 0.001f),
        glm::vec4(0.001f),
        glm::vec3(0.0f),
        1.0f);*/

    //////light 02
    //SetLight(1,
    //    glm::vec4(0.0f, 20.0f, 0.0f, 1.0f),
    //    glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
    //    glm::vec3(NULL, 0.01f, 0.001f),
    //    glm::vec4(0.0f, 1.0f, 0.0f, 1.0f),
    //    glm::vec3(NULL, 5.0f, 10.0f),
    //    1.0f);


    cLightHelper TheLightHelper;



    SceneEditor* sceneEditor = new SceneEditor();

    sceneEditor->Start("cube.txt",fileManager, program, window, g_pMeshManager, scene);



    while (!glfwWindowShouldClose(window))
    {
        float ratio;
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        ratio = width / (float)height;
        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        UpdateMatricies(ratio, program);

//        // *******************************************************************


        // Update the light info in the shader
        // (Called every frame)
        ::g_pLightManager->updateShaderWithLightInfo();
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


        for (unsigned int meshIndex = 0; meshIndex != ::g_vecMeshesToDraw.size(); meshIndex++)
        {
            //            sMesh* pCurMesh = ::g_myMeshes[meshIndex];
            sMesh* pCurMesh = ::g_vecMeshesToDraw[meshIndex];

            DrawMesh(pCurMesh, program);

        }//for (unsigned int meshIndex..

      //  DrawLazer(program);


        // **********************************************************************************
        if (::g_bShowDebugSpheres)
        {
            DrawDebugObjects(TheLightHelper,program);
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
                = pBouncy_5_Ball->positionXYZ - glm::vec3(::g_pLightManager->theLights[1].position);
    
            // Normalize to get the direction only
            directionToBal = glm::normalize(directionToBal);

            // Point the spot light at the bouncy ball
            ::g_pLightManager->theLights[1].direction = glm::vec4(directionToBal, 1.0f);
        }






        // Handle async IO stuff
        handleKeyboardAsync(window);
        handleMouseAsync(window);

        glfwSwapBuffers(window);
        glfwPollEvents();

        UpdateWindowTitle(window);



    }// End of the draw loop


    // Delete everything
    delete ::g_pFlyCamera;
    delete ::g_pPhysicEngine;

    glfwDestroyWindow(window);

    glfwTerminate();
    exit(EXIT_SUCCESS);
}

//TODO: Add this mesh to vector with all meshes on the screen + return this sMesh*
//return and push_back
sMesh* GenerateMeshObjects(std::string filePath, glm::vec3 posXYZ, glm::vec3 rotXYZ,bool bOverrideColor, glm::vec4 objectColor, bool bDoLightingExist)
{
    sMesh* Meshes = new sMesh();
    Meshes->modelFileName = filePath;
    Meshes->positionXYZ = posXYZ;
    Meshes->rotationEulerXYZ = rotXYZ;
    Meshes->bOverrideObjectColour = bOverrideColor;
    Meshes->objectColourRGBA = objectColor;
    Meshes->bDoNotLight = true;

    ::g_vecMeshesToDraw.push_back(Meshes);

    return Meshes;
    
    
}


//WE SHOULD WIPE IT CLEAN BEFORE THE EXAM
void AddModelsToScene(void)
{

    // Load some models to draw
    //TODO: ENES refactor
    
    {
        sMesh* pHangar = new sMesh();
        pHangar->modelFileName = "assets/models/Demonstration_Interior - DO NOT USE THIS xyz_N.ply";
        pHangar->positionXYZ = glm::vec3(0.0f, 30.0f, 0.0f);
        pHangar->bOverrideObjectColour = true;
        pHangar->objectColourRGBA = glm::vec4(1.0f, 1.0f, 0.0f, 1.0f);
        pHangar->rotationEulerXYZ.x = -90.0f;
        pHangar->rotationEulerXYZ.z = 180.0f;
        ::g_vecMeshesToDraw.push_back(pHangar);
    }


    // Add a bunch of bunny rabbits
    float boxLimit = 50.0f;
    float boxStep = 10.0f;
    for (float x = -boxLimit; x <= boxLimit; x += boxStep)
    {
        for (float z = -boxLimit; z <= boxLimit; z += boxStep)
        {
            sMesh* pBunny = new sMesh();
//            pBunny->modelFileName = "assets/models/bun_zipper_res2_10x_size_xyz_only.ply";
            pBunny->modelFileName = "assets/models/bun_zipper_res2_10x_size_xyz_N_only.ply";
            pBunny->positionXYZ = glm::vec3(x, -3.0f, z);
            pBunny->objectColourRGBA 
                = glm::vec4(getRandomFloat(0.0f, 1.0f),
                            getRandomFloat(0.0f, 1.0f),
                            getRandomFloat(0.0f, 1.0f), 
                            1.0f );
            ::g_vecMeshesToDraw.push_back(pBunny);
        }
    }//for (float x = -boxLimit...



    {
//    ____                _            __                   _     
//   |  _ \ ___ _ __   __| | ___ _ __ / / __ ___   ___  ___| |__  
//   | |_) / _ \ '_ \ / _` |/ _ \ '__/ / '_ ` _ \ / _ \/ __| '_ \ 
//   |  _ <  __/ | | | (_| |  __/ | / /| | | | | |  __/\__ \ | | |
//   |_| \_\___|_| |_|\__,_|\___|_|/_/ |_| |_| |_|\___||___/_| |_|
//                                                                
        sMesh* pWarehouse = new sMesh();
        pWarehouse->modelFileName = "assets/models/Warehouse_xyz_n.ply";
        pWarehouse->positionXYZ = glm::vec3(-10.0f, 5.0f, 0.0f);
        pWarehouse->rotationEulerXYZ.y = -90.0f;
        pWarehouse->objectColourRGBA = glm::vec4(0.6f, 0.6f, 0.6f, 1.0f);
        //pWarehouse->bIsWireframe = true;
        pWarehouse->bOverrideObjectColour = true;
        pWarehouse->uniqueFriendlyName = "Warehouse";
         ::g_vecMeshesToDraw.push_back(pWarehouse);

//    ____  _               _                  _     _           _   
//   |  _ \| |__  _   _ ___(_) ___ ___    ___ | |__ (_) ___  ___| |_ 
//   | |_) | '_ \| | | / __| |/ __/ __|  / _ \| '_ \| |/ _ \/ __| __|
//   |  __/| | | | |_| \__ \ | (__\__ \ | (_) | |_) | |  __/ (__| |_ 
//   |_|   |_| |_|\__, |___/_|\___|___/  \___/|_.__// |\___|\___|\__|
//                |___/                           |__/               
         ::g_pPhysicEngine->addTriangleMesh(
             "assets/models/Warehouse_xyz_n.ply",
             pWarehouse->positionXYZ,
             pWarehouse->rotationEulerXYZ,
             pWarehouse->uniformScale);

    }
    //{
    //    //    ____                _            __                   _     
    //    //   |  _ \ ___ _ __   __| | ___ _ __ / / __ ___   ___  ___| |__  
    //    //   | |_) / _ \ '_ \ / _` |/ _ \ '__/ / '_ ` _ \ / _ \/ __| '_ \ 
    //    //   |  _ <  __/ | | | (_| |  __/ | / /| | | | | |  __/\__ \ | | |
    //    //   |_| \_\___|_| |_|\__,_|\___|_|/_/ |_| |_| |_|\___||___/_| |_|


    //    //    ____  _               _                  _     _           _   
    //    //   |  _ \| |__  _   _ ___(_) ___ ___    ___ | |__ (_) ___  ___| |_ 
    //    //   | |_) | '_ \| | | / __| |/ __/ __|  / _ \| '_ \| |/ _ \/ __| __|
    //    //   |  __/| | | | |_| \__ \ | (__\__ \ | (_) | |_) | |  __/ (__| |_ 
    //    //   |_|   |_| |_|\__, |___/_|\___|___/  \___/|_.__// |\___|\___|\__|
    //    //                |___/                           |__/               



    {
        sMesh* pFlatPlane = new sMesh();
        pFlatPlane->modelFileName = "assets/models/Flat_Plane_xyz_N.ply";
        pFlatPlane->positionXYZ = glm::vec3(0.0f, -5.0f, 0.0f);
        pFlatPlane->objectColourRGBA = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
        pFlatPlane->uniqueFriendlyName = "Ground";
        //        pFlatPlane->bIsWireframe = true;
        //        ::g_myMeshes[::g_NumberOfMeshesToDraw] = pFlatPlane;
        //        ::g_NumberOfMeshesToDraw++;
        ::g_vecMeshesToDraw.push_back(pFlatPlane);


        // Add the "ground" to the physcs
        cPhysics::sAABB* pAABBGround = new cPhysics::sAABB();
        pAABBGround->centreXYZ = pFlatPlane->positionXYZ;
        sModelDrawInfo planeMeshInfo;
        ::g_pMeshManager->FindDrawInfoByModelName(pFlatPlane->modelFileName, planeMeshInfo);

       // Manually enter the AABB info:
        pAABBGround->centreXYZ = glm::vec3(0.0f, 0.0f, 0.0f);   // From the mesh model
        // How far from the centre the XYZ min and max are
        // This information is from the mesh we loaded
        // WARNING: We need to be careful about the scale
        pAABBGround->minXYZ.x = -100.0f;
        pAABBGround->maxXYZ.x = 100.0f;

        pAABBGround->minXYZ.z = -100.0f;
        pAABBGround->maxXYZ.z = 100.0f;

        pAABBGround->minXYZ.y = -1.0f;
        pAABBGround->maxXYZ.y = 1.0f;

        // Copy the physics object position from the initial mesh position
        pAABBGround->pPhysicInfo->position = pFlatPlane->positionXYZ;

        // Don't move this ground (skip integration step)
        pAABBGround->pPhysicInfo->bDoesntMove = true;

        pAABBGround->pPhysicInfo->pAssociatedDrawingMeshInstance = pFlatPlane;

        ::g_pPhysicEngine->vecAABBs.push_back(pAABBGround);
    }
    {
        sMesh* pFlatPlane = new sMesh();
//        pFlatPlane->modelFileName = "assets/models/Flat_Plane_xyz.ply";
        pFlatPlane->modelFileName = "assets/models/Flat_Plane_xyz_N.ply";
        pFlatPlane->positionXYZ = glm::vec3(0.0f, -5.0f, 0.0f);
        pFlatPlane->bIsWireframe = true;
        pFlatPlane->uniformScale = 1.01f;
        pFlatPlane->objectColourRGBA = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

        ::g_vecMeshesToDraw.push_back(pFlatPlane);
    }

    sMesh* pBunny = new sMesh();
    //            pBunny->modelFileName = "assets/models/bun_zipper_res2_10x_size_xyz_only.ply";
    pBunny->modelFileName = "assets/models/bun_zipper_res2_10x_size_xyz_N_only.ply";
    pBunny->positionXYZ = glm::vec3(10.0f, 10.0f, 0.0f);
    pBunny->objectColourRGBA
        = glm::vec4(getRandomFloat(0.0f, 1.0f),
            getRandomFloat(0.0f, 1.0f),
            getRandomFloat(0.0f, 1.0f),
            1.0f);
    pBunny->uniqueFriendlyName = "Ground";
    ::g_vecMeshesToDraw.push_back(pBunny);



    {

        //    ____                _            __                   _     
        //   |  _ \ ___ _ __   __| | ___ _ __ / / __ ___   ___  ___| |__  
        //   | |_) / _ \ '_ \ / _` |/ _ \ '__/ / '_ ` _ \ / _ \/ __| '_ \ 
        //   |  _ <  __/ | | | (_| |  __/ | / /| | | | | |  __/\__ \ | | |
        //   |_| \_\___|_| |_|\__,_|\___|_|/_/ |_| |_| |_|\___||___/_| |_|
        //                                                                
        sMesh* pSphereMesh = new sMesh();
//        pSphereMesh->modelFileName = "assets/models/Sphere_radius_1_xyz.ply";
        pSphereMesh->modelFileName = "assets/models/Sphere_radius_1_xyz_N.ply";
        pSphereMesh->positionXYZ = glm::vec3(-15.0f, -3.0f, -20.0f);
        //pSphereMesh->bIsWireframe = true;
        pSphereMesh->objectColourRGBA = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
        pSphereMesh->uniqueFriendlyName = "Ball";

        //::g_myMeshes[::g_NumberOfMeshesToDraw] = pSphere;
        //::g_NumberOfMeshesToDraw++;
        ::g_vecMeshesToDraw.push_back(pSphereMesh);

        {
            sMesh* pSphereShadowMesh = new sMesh();
            pSphereShadowMesh->modelFileName = "assets/models/Sphere_radius_1_Flat_Shadow_xyz_N.ply";
            pSphereShadowMesh->positionXYZ = pSphereMesh->positionXYZ;
            pSphereShadowMesh->positionXYZ.y = -3.95f;  // JUST above the ground
            pSphereShadowMesh->objectColourRGBA = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
            pSphereShadowMesh->uniqueFriendlyName = "Ball_Shadow";
            ::g_vecMeshesToDraw.push_back(pSphereShadowMesh);
        }


        //    ____  _               _                  _     _           _   
        //   |  _ \| |__  _   _ ___(_) ___ ___    ___ | |__ (_) ___  ___| |_ 
        //   | |_) | '_ \| | | / __| |/ __/ __|  / _ \| '_ \| |/ _ \/ __| __|
        //   |  __/| | | | |_| \__ \ | (__\__ \ | (_) | |_) | |  __/ (__| |_ 
        //   |_|   |_| |_|\__, |___/_|\___|___/  \___/|_.__// |\___|\___|\__|
        //                |___/                           |__/               
        // Add sphere
        cPhysics::sSphere* pSphereInfo = new cPhysics::sSphere();

        pSphereInfo->centre = glm::vec3(0.0f);  // Sphere's centre (i.e. an offset from the position)

        pSphereInfo->pPhysicInfo->position = pSphereMesh->positionXYZ;
        // HACK: We know this is 1.0 because...?
        // We could also have pulled that information from the mesh info
        pSphereInfo->radius = 1.0f;

        pSphereInfo->pPhysicInfo->velocity.y = 7.5f;
        
        // Set some x velocity
        pSphereInfo->pPhysicInfo->velocity.x = 1.0f;


        pSphereInfo->pPhysicInfo->acceleration.y = -3.0f;
        
        // Associate this drawing mesh to this physics object
        pSphereInfo->pPhysicInfo->pAssociatedDrawingMeshInstance = pSphereMesh;

        ::g_pPhysicEngine->vecSpheres.push_back(pSphereInfo);
    }


    for ( unsigned int ballCount = 0; ballCount != 10; ballCount++ )
    {
        //    ____                _            __                   _     
        //   |  _ \ ___ _ __   __| | ___ _ __ / / __ ___   ___  ___| |__  
        //   | |_) / _ \ '_ \ / _` |/ _ \ '__/ / '_ ` _ \ / _ \/ __| '_ \ 
        //   |  _ <  __/ | | | (_| |  __/ | / /| | | | | |  __/\__ \ | | |
        //   |_| \_\___|_| |_|\__,_|\___|_|/_/ |_| |_| |_|\___||___/_| |_|
        //                                                                
        sMesh* pSphereMesh = new sMesh();
        //        pSphereMesh->modelFileName = "assets/models/Sphere_radius_1_xyz.ply";
        pSphereMesh->modelFileName = "assets/models/Sphere_radius_1_xyz_N.ply";
        pSphereMesh->positionXYZ.x = getRandomFloat(-30.0f, 30.0f);
        pSphereMesh->positionXYZ.z = getRandomFloat(-30.0f, 30.0f);
        pSphereMesh->positionXYZ.y = getRandomFloat(0.0f, 40.0f);
        //pSphereMesh->bIsWireframe = true;
        pSphereMesh->objectColourRGBA = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
        pSphereMesh->objectColourRGBA.r = getRandomFloat(0.0f, 1.0f);
        pSphereMesh->objectColourRGBA.g = getRandomFloat(0.0f, 1.0f);
        pSphereMesh->objectColourRGBA.b = getRandomFloat(0.0f, 1.0f);
        std::stringstream ssBallName;
        ssBallName << "Bouncy_" << ballCount;
        pSphereMesh->uniqueFriendlyName = ssBallName.str();

        ::g_vecMeshesToDraw.push_back(pSphereMesh);

        //    ____  _               _                  _     _           _   
        //   |  _ \| |__  _   _ ___(_) ___ ___    ___ | |__ (_) ___  ___| |_ 
        //   | |_) | '_ \| | | / __| |/ __/ __|  / _ \| '_ \| |/ _ \/ __| __|
        //   |  __/| | | | |_| \__ \ | (__\__ \ | (_) | |_) | |  __/ (__| |_ 
        //   |_|   |_| |_|\__, |___/_|\___|___/  \___/|_.__// |\___|\___|\__|
        //                |___/                           |__/               
        // Add sphere
        cPhysics::sSphere* pSphereInfo = new cPhysics::sSphere();
        pSphereInfo->centre = glm::vec3(0.0f);  // Sphere's centre (i.e. an offset from the position)
        pSphereInfo->pPhysicInfo->position = pSphereMesh->positionXYZ;
        pSphereInfo->radius = 1.0f;
        pSphereInfo->pPhysicInfo->velocity.y = getRandomFloat(2.0f, 10.0f);
        pSphereInfo->pPhysicInfo->velocity.x = getRandomFloat(-5.0f, 5.0f);
        pSphereInfo->pPhysicInfo->velocity.z = getRandomFloat(-5.0f, 5.0f);
        pSphereInfo->pPhysicInfo->acceleration.y = -3.0f;
        pSphereInfo->pPhysicInfo->pAssociatedDrawingMeshInstance = pSphereMesh;
        ::g_pPhysicEngine->vecSpheres.push_back(pSphereInfo);
    }//for ( unsigned int ballCount



    return;
}




























//using namespace std;

void ConsoleStuff(void)
{
    // "o" for output
//    std::ofstream myFile("someData.txt");
    // Write something
    //myFile << "Hello" << std::endl;
    //myFile << "there";
    //myFile.close();

    // Now read this file
//    std::ifstream myFile2("someData.txt");
//    std::string someString;
//    myFile2 >> someString;
//    std::cout << someString << std::endl;
//
    //std::string aword;
    //while (aword != "END_OF_FILE")
    //{
    //    myFile2 >> aword;
    //    std::cout << aword << std::endl;
    //};

    //std::string aword;
    //while (myFile2 >> aword)
    //{
    //    std::cout << aword << std::endl;
    //};

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