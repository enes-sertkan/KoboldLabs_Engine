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
#include "cCommandFactory.hpp"
#include "cCommandGroup.hpp"

#include "aRayCastPhysics.h"
#include "aDrawAim.hpp"
#include "aPlayerItemsController.h"
#include "ModelsLoader.hpp"

std::vector<sMesh*> g_vecMeshesToDraw;

cPhysics* g_pPhysicEngine = NULL;
// This loads the 3D models for drawing, etc.
cVAOManager* g_pMeshManager = NULL;

cBasicTextureManager* g_pTextures = NULL;

cCommandGroup* g_pCommandDirector = NULL;
cCommandFactory* g_pCommandFactory = NULL;

void DrawMesh(sMesh* pCurMesh, GLuint program, cVAOManager* vaoManager, cBasicTextureManager* textureManager);



static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}



static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);

    const float CAMERA_MOVE_SPEED = 0.1f;


    return;
}




//TODO: Cut it  from here, but make shure we still have this.
// https://stackoverflow.com/questions/5289613/generate-random-float-between-two-floats
float getRandomFloat(float a, float b) {
    float random = ((float)rand()) / (float)RAND_MAX;
    float diff = b - a;
    float r = random * diff;
    return a + r;
}

GLuint PrepareOpenGL(GLFWwindow* const &window)
{

    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwSetErrorCallback(error_callback);
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


    
    GLint iMaxCombinedTextureInmageUnits = 0;
    glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &iMaxCombinedTextureInmageUnits);
    std::cout << "GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS = " << iMaxCombinedTextureInmageUnits << std::endl;

    // data returns one value, the maximum number of components of the inputs read by the fragment shader, 
    // which must be at least 128.
    GLint iMaxFragmentInputComponents = 0;
    glGetIntegerv(GL_MAX_FRAGMENT_INPUT_COMPONENTS, &iMaxFragmentInputComponents);
    std::cout << "GL_MAX_FRAGMENT_INPUT_COMPONENTS = " << iMaxFragmentInputComponents << std::endl;
    

    // data returns one value, the maximum number of individual floating - point, integer, or boolean values 
    // that can be held in uniform variable storage for a fragment shader.The value must be at least 1024. 
    GLint iMaxFragmentUniformComponents = 0;
    glGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_COMPONENTS, &iMaxFragmentUniformComponents);
    std::cout << "GL_MAX_FRAGMENT_UNIFORM_COMPONENTS = " << iMaxFragmentUniformComponents << std::endl;
        



    
    return program;
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


void DrawDebugObjects(cLightHelper TheLightHelper ,GLuint program, cLightManager* lightManager, Scene* scene)
{


    DrawDebugSphere(lightManager->theLights[::g_selectedLightIndex].position,
        glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), 0.1f, program, scene);

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
        program, scene);


    float distance_50_percent =
        TheLightHelper.calcApproxDistFromAtten(0.5f, ACCURACY, FLT_MAX,
            lightManager->theLights[::g_selectedLightIndex].atten.x,   // Const attent
            lightManager->theLights[::g_selectedLightIndex].atten.y,   // Linear attenuation
            lightManager->theLights[::g_selectedLightIndex].atten.z);  // Quadratic attenuation

    DrawDebugSphere(lightManager->theLights[::g_selectedLightIndex].position,
        glm::vec4(0.0f, DEBUG_LIGHT_BRIGHTNESS, 0.0f, 1.0f),
        distance_50_percent,
        program, scene);

    float distance_25_percent =
        TheLightHelper.calcApproxDistFromAtten(0.25f, ACCURACY, FLT_MAX,
            lightManager->theLights[::g_selectedLightIndex].atten.x,   // Const attent
            lightManager->theLights[::g_selectedLightIndex].atten.y,   // Linear attenuation
            lightManager->theLights[::g_selectedLightIndex].atten.z);  // Quadratic attenuation

    DrawDebugSphere(lightManager->theLights[::g_selectedLightIndex].position,
        glm::vec4(0.0f, 0.0f, DEBUG_LIGHT_BRIGHTNESS, 1.0f),
        distance_25_percent,
        program, scene);

    float distance_05_percent =
        TheLightHelper.calcApproxDistFromAtten(0.05f, ACCURACY, FLT_MAX,
            lightManager->theLights[::g_selectedLightIndex].atten.x,   // Const attent
            lightManager->theLights[::g_selectedLightIndex].atten.y,   // Linear attenuation
            lightManager->theLights[::g_selectedLightIndex].atten.z);  // Quadratic attenuation

    DrawDebugSphere(lightManager->theLights[::g_selectedLightIndex].position,
        glm::vec4(DEBUG_LIGHT_BRIGHTNESS, DEBUG_LIGHT_BRIGHTNESS, 0.0f, 1.0f),
        distance_05_percent,
        program, scene);
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


void UpdateWindowTitle(GLFWwindow* window, cLightManager* lightManager)
{

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

void AddActions(Scene* scene, GLuint program)
{


    //Object* playerObject = scene->sceneObjects[1];

    //// Add the player camera action (with an offset for camera positioning)
    //aPlayerCamera* playerCameraAction = new aPlayerCamera(::g_pFlyCamera, glm::vec3(0.0f, 10.0f, 0.0f));
    //scene->AddActionToObj(playerCameraAction, playerObject);


    //aPlayerMovement* playerMovement = new aPlayerMovement();
    //playerMovement->program = program;
    //scene->AddActionToObj(playerMovement, scene->sceneObjects[1]);

    //RayCastPhysics* phys = new RayCastPhysics;
    //phys->gravityAcceleration.y = -5;
    //phys->baseRayCastLength = 10.0;
    //scene->AddActionToObj(phys, scene->sceneObjects[1]);



    //aDrawAim* drawAimAction = new aDrawAim();
    //drawAimAction->program = program;
    //scene->AddActionToObj(drawAimAction, scene->sceneObjects[1]);


    //aPlayerItemsController* itemsControllerAction = new aPlayerItemsController();
    //scene->AddActionToObj(itemsControllerAction, scene->sceneObjects[1]);


    scene->sceneObjects[0]->mesh->textures[0] = "baloon.bmp";
    scene->sceneObjects[0]->mesh->blendRatio[0] = 2;
    scene->sceneObjects[0]->mesh->bOverrideObjectColour = false;

    scene->sceneObjects[1]->mesh->textures[0] = "banners.bmp";
    scene->sceneObjects[1]->mesh->blendRatio[0] = 3;
    scene->sceneObjects[1]->mesh->bOverrideObjectColour = false;

    scene->sceneObjects[2]->mesh->textures[0] = "barriers.bmp";
    scene->sceneObjects[2]->mesh->blendRatio[0] = 1;
    scene->sceneObjects[2]->mesh->bOverrideObjectColour = false;

    scene->sceneObjects[3]->mesh->textures[0] = "garages.bmp";
    scene->sceneObjects[3]->mesh->blendRatio[0] = 3;
    scene->sceneObjects[3]->mesh->bOverrideObjectColour = false;

    scene->sceneObjects[4]->mesh->textures[0] = "grass_1.bmp";
    scene->sceneObjects[4]->mesh->blendRatio[0] = 1;
    scene->sceneObjects[4]->mesh->bOverrideObjectColour = false;

    scene->sceneObjects[5]->mesh->textures[0] = "grass_2.bmp";
    scene->sceneObjects[5]->mesh->blendRatio[0] = 3;
    scene->sceneObjects[5]->mesh->bOverrideObjectColour = false;

    scene->sceneObjects[6]->mesh->textures[0] = "gravel.bmp";
    scene->sceneObjects[6]->mesh->blendRatio[0] = 1;
    scene->sceneObjects[6]->mesh->bOverrideObjectColour = false;

    scene->sceneObjects[7]->mesh->textures[0] = "metal_fence.bmp";
    scene->sceneObjects[7]->mesh->blendRatio[0] = 3;
    scene->sceneObjects[7]->mesh->bOverrideObjectColour = false;

    scene->sceneObjects[8]->mesh->textures[0] = "road.bmp";
    scene->sceneObjects[8]->mesh->blendRatio[0] = 1;
    scene->sceneObjects[8]->mesh->bOverrideObjectColour = false;

    scene->sceneObjects[9]->mesh->textures[0] = "rock.bmp";
    scene->sceneObjects[9]->mesh->blendRatio[0] = 3;
    scene->sceneObjects[9]->mesh->bOverrideObjectColour = false;

    scene->sceneObjects[10]->mesh->textures[0] = "trees.bmp";
    scene->sceneObjects[10]->mesh->blendRatio[0] = 3;
    scene->sceneObjects[10]->mesh->bOverrideObjectColour = false;

    scene->sceneObjects[11]->mesh->textures[0] = "tyres.bmp";
    scene->sceneObjects[11]->mesh->blendRatio[0] = 3;
    scene->sceneObjects[11]->mesh->bOverrideObjectColour = false;
}





//MAIN
//-----------------------------------------------------------------------------------------------------------------------------


int main(void)
{

//   READING FILES
//   -------------


    // Instantiate KLFileManager
    KLFileManager* fileManager = new KLFileManager();

    CreateModelFiles(fileManager);

    // Read the scene from the file (assuming the file exists)
    Scene* scene = fileManager->ReadSceneFile("SaveScene.txt");



    for (Object* object : scene->sceneObjects)
    {
        std::cout<<object->name<<std::endl;
    }

    



//   CHECK IF INIT
//   -------------

    if (!glfwInit())
        exit(EXIT_FAILURE);





//   PREPARING WINDOW
//   ----------------

    ///I really want to put this into a function, vut IDK how
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(640, 480, "OpenGL Triangle", NULL, NULL);
    




//   PREPARING OPENGL
//   ----------------

    GLuint program = PrepareOpenGL(window);
   


//   PREPARING ENGINE STUFF
//   ----------------------

    scene->vaoManager= new cVAOManager();
    PhysicsManager* physicsMan = new PhysicsManager();
    PrepareFlyCamera();




//   PREPARING SCENE
//   ---------------
    scene->Prepare(scene->vaoManager, program, physicsMan, window, g_pFlyCamera);
    AddActions(scene, program);





//   PREPARING SOMETHING ELSE (TODO: Try to put it away into functions)
//   ------------------------


    glUseProgram(program);


    scene->textureManager->SetBasePath("assets/textures");
    scene->textureManager->Create2DTextureFromBMPFile("baloon.bmp");
    scene->textureManager->Create2DTextureFromBMPFile("banners.bmp");
    scene->textureManager->Create2DTextureFromBMPFile("barriers.bmp");
    scene->textureManager->Create2DTextureFromBMPFile("garages.bmp");
    scene->textureManager->Create2DTextureFromBMPFile("grass_1.bmp");
    scene->textureManager->Create2DTextureFromBMPFile("grass_2.bmp");
    scene->textureManager->Create2DTextureFromBMPFile("gravel.bmp");
    scene->textureManager->Create2DTextureFromBMPFile("metal_fence.bmp");
    scene->textureManager->Create2DTextureFromBMPFile("road.bmp");
    scene->textureManager->Create2DTextureFromBMPFile("rock.bmp");
    scene->textureManager->Create2DTextureFromBMPFile("trees.bmp");
    scene->textureManager->Create2DTextureFromBMPFile("tyres.bmp");



    // lookings
    std::string errorString;
    if (scene->textureManager->CreateCubeTextureFromBMPFiles("Space",
        "CubeMaps/SpaceBox_right1_posX.bmp",
        "CubeMaps/SpaceBox_left2_negX.bmp",
        "CubeMaps/SpaceBox_top3_posY.bmp",
        "CubeMaps/SpaceBox_bottom4_negY.bmp",
        "CubeMaps/SpaceBox_front5_posZ.bmp",
        "CubeMaps/SpaceBox_back6_negZ.bmp", true, errorString))
    {
        std::cout << "Loaded space skybox" << std::endl;
    }
    else
    {
        std::cout << "ERROR: Didn't load space skybox because: " << errorString << std::endl;
    }


    // Enable depth buffering (z buffering)
    // https://registry.khronos.org/OpenGL-Refpages/gl4/html/glEnable.xhtml
    glEnable(GL_DEPTH_TEST);

    double currentFrameTime = glfwGetTime();
    double lastFrameTime = glfwGetTime();

    
    cLightHelper TheLightHelper;


//   PREPARING SCENE EDITOR
//   ----------------------

    SceneEditor* sceneEditor = new SceneEditor();

    sceneEditor->Start("selectBox.txt",fileManager, program, window, scene->vaoManager, scene);




//   GENERATING MAZE
//   ---------------
    //MazeGenerator* mazeGenerator = new MazeGenerator("assets/models/maze.txt", scene, scene->lightManager);
  
    //mazeGenerator->generateMaze();



    scene->Start();

    while (!glfwWindowShouldClose(window))
    {
        float ratio;
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        ratio = width / (float)height;
        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



//      UPDATE
//      ------------------------------------------
        SetCameraAndProjectionMatrices(ratio, program);
        scene->lightManager->updateShaderWithLightInfo();
        sceneEditor->Update();
        scene->Update();



        // lookings
        // Sky box
 //Move the sky sphere with the camera
        Object* SkySphere = scene->GenerateMeshObjectsFromObject("assets/models/Sphere_radius_1_xyz_N_uv.ply",
            glm::vec3(0, 0, 0), 
            1, 
            glm::vec3(0, 0, 0), 
            false, 
            glm::vec4(0.5f, 0.5f, 0.5f, 1.0f),
            true,
            scene->sceneObjects);
        SkySphere->mesh->textures[0];
        SkySphere->mesh->positionXYZ = scene->fCamera->getEyeLocation();

        // Disable backface culling (so BOTH sides are drawn)
        glDisable(GL_CULL_FACE);
        // Don't perform depth buffer testing
        glDisable(GL_DEPTH_TEST);
        // Don't write to the depth buffer when drawing to colour (back) buffer
        glDepthMask(GL_FALSE);
        glDepthFunc(GL_ALWAYS);// or GL_LESS (default)
        // GL_DEPTH_TEST : do or not do the test against what's already on the depth buffer

        SkySphere->mesh->bIsVisible = true;
        //        pSkySphere->bDoNotLight = true;

        SkySphere->mesh->uniformScale = 1.0f;

        // Tell the shader this is the skybox, so use the cube map
        // uniform samplerCube skyBoxTexture;
        // uniform bool bIsSkyBoxObject;
        GLuint bIsSkyBoxObject_UL = glGetUniformLocation(program, "bIsSkyBoxObject");
        glUniform1f(bIsSkyBoxObject_UL, (GLfloat)GL_TRUE);

        // Set the cube map texture, just like we do with the 2D
        GLuint cubeSamplerID = scene->textureManager->getTextureIDFromName("Space");
        //        GLuint cubeSamplerID = ::g_pTextures->getTextureIDFromName("SunnyDay");
                // Make sure this is an unused texture unit
        glActiveTexture(GL_TEXTURE0 + 60);
        // *****************************************
        // NOTE: This is a CUBE_MAP, not a 2D
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubeSamplerID);
        //        glBindTexture(GL_TEXTURE_2D, cubeSamplerID);
                // *****************************************
        GLint skyBoxTextureSampler_UL = glGetUniformLocation(program, "skyBoxTextureSampler");
        glUniform1i(skyBoxTextureSampler_UL, 60);       // <-- Note we use the NUMBER, not the GL_TEXTURE3 here

        DrawMesh(SkySphere->mesh, program, scene->vaoManager, scene->textureManager);

        SkySphere->mesh->bIsVisible = false;

        glUniform1f(bIsSkyBoxObject_UL, (GLfloat)GL_FALSE);

        glEnable(GL_CULL_FACE);
        // Enable depth test and write to depth buffer (normal rendering)
        glEnable(GL_DEPTH_TEST);
        //        glDepthMask(GL_FALSE);
        //        glDepthFunc(GL_LESS);
                // **************************************************************

    

//      DRAW LOOP
//      ------------------------------------------       

        for (Object* object:scene->sceneObjects)
        {
  
            sMesh* pCurMesh = object->mesh;

            DrawMesh(pCurMesh, program, scene->vaoManager, scene->textureManager);

        }



//      ADDITIONAL DRAW STUFF
//      ------------------------------------------     
        if (::g_bShowDebugSpheres)
        {
            DrawDebugObjects(TheLightHelper,program, scene->lightManager, scene);
        }
//      ------------------------------------------     



//      DELTA TIME
//      ------------------------------------------ 
        currentFrameTime = glfwGetTime();
        double deltaTime = currentFrameTime - lastFrameTime;
        lastFrameTime = currentFrameTime;
//      ------------------------------------------ 

        



//      HANDLE ASYNC CONTROLS
//      ------------------------------------------ 
        handleKeyboardAsync(window, scene);
        handleMouseAsync(window);


//      SWAP VISUAL BUFFERS
//      ------------------------------------------ 
        glfwSwapBuffers(window);


//      EVENTS
//      ------------------------------------------ 
        glfwPollEvents();


//      WINDOW NAME
//      ------------------------------------------ 
        UpdateWindowTitle(window, scene->lightManager);



    }




//      DELETE EVERYTHING
//      ------------------------------------------ 
    delete ::g_pFlyCamera;


    glfwDestroyWindow(window);

    glfwTerminate();
    exit(EXIT_SUCCESS);
}





