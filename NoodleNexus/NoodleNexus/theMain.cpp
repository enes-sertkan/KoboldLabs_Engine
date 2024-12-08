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
#include <thread>
#include <chrono>

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
#include "cLuaBrain.hpp"
#include "LuaScript.h"
#include "ObjectManager.h"

#include "aRayCastPhysics.h"
#include "aDrawAim.hpp"
#include "aPlayerItemsController.h"
#include "ModelsLoader.hpp"
#include "aLuaScript.h"
#include "aLuaScriptsSerial.h"
#include "aLocTriggersLua.h"



 Scene* currentScene=nullptr;



std::vector<sMesh*> g_vecMeshesToDraw;

cPhysics* g_pPhysicEngine = NULL;
// This loads the 3D models for drawing, etc.
cVAOManager* g_pMeshManager = NULL;

cBasicTextureManager* g_pTextures = NULL;

cCommandGroup* g_pCommandDirector = NULL;
cCommandFactory* g_pCommandFactory = NULL;

void DrawMesh(sMesh* pCurMesh, GLuint program, cVAOManager* vaoManager, cBasicTextureManager* textureManager);
void DrawSkyBox(sMesh* pCurMesh, GLuint program, cVAOManager* vaoManager, cBasicTextureManager* textureManager);


std::string g_floatToString(float theFloat)
{
    std::stringstream ssFloat;
    ssFloat << theFloat;
    return ssFloat.str();
}


// Function to be called in Lua
int lua_MoveObject(lua_State* L) {
    // Retrieve arguments from the Lua stack
    const char* objectName = luaL_checkstring(L, 1);  // Object name
    float x = luaL_checknumber(L, 2);  // New x position
    float y = luaL_checknumber(L, 3);  // New y position
    float z = luaL_checknumber(L, 4);  // New z position

    // Find the object and update its position
    for (Object* obj : currentScene->sceneObjects) {
        if (obj->name == objectName) {
            std::cout <<"[C++] ["<<obj->name << "] current Position X: " << x << "Y: " << y << "Z: " << z << std::endl;

            obj->mesh->positionXYZ = glm::vec3(x, y, z);
            break;
        }
    }

    return 0;  // No values are returned to Lua
}

// Function to rotate an object to a new orientation (this will be called from Lua)
int lua_RotateTo(lua_State* L) {
    // Retrieve arguments from the Lua stack
    const char* objectName = luaL_checkstring(L, 1);  // Object name
    float rotationX = luaL_checknumber(L, 2);  // Rotation around the X-axis
    float rotationY = luaL_checknumber(L, 3);  // Rotation around the Y-axis
    float rotationZ = luaL_checknumber(L, 4);  // Rotation around the Z-axis

    // Find the object and update its rotation
    for (Object* obj : currentScene->sceneObjects) {
        if (obj->name == objectName) {
            std::cout << "current Rotation X: " << rotationX << "Y: " << rotationY << "Z: " << rotationZ << std::endl;
            // Apply rotation (Assuming we use Euler angles here)
            obj->mesh->rotationEulerXYZ = glm::vec3(rotationX, rotationY, rotationZ);
            break;
        }
        
    }

    return 0;  // No return values
}


// Function to interpolate along a quadratic Bézier curve
glm::vec3 QuadraticBezier(glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, float t) {
    return (1 - t) * (1 - t) * p0 + 2 * (1 - t) * t * p1 + t * t * p2;
}

// Lua binding function for following a curve
int lua_FollowACurve(lua_State* L) {
    // Retrieve arguments from Lua stack
    const char* objectName = luaL_checkstring(L, 1);  // Object name
    float startX = luaL_checknumber(L, 2);  // Start point X
    float startY = luaL_checknumber(L, 3);  // Start point Y
    float startZ = luaL_checknumber(L, 4);  // Start point Z
    float controlX = luaL_checknumber(L, 5);  // Control point X
    float controlY = luaL_checknumber(L, 6);  // Control point Y
    float controlZ = luaL_checknumber(L, 7);  // Control point Z
    float endX = luaL_checknumber(L, 8);  // End point X
    float endY = luaL_checknumber(L, 9);  // End point Y
    float endZ = luaL_checknumber(L, 10);  // End point Z
    float seconds = luaL_checknumber(L, 11);  // Duration
    float deltaTime = luaL_checknumber(L, 12);  // Frame time

    // Calculate time increment based on deltaTime and seconds
    static float elapsedTime = 0.0f;  // Track elapsed time across frames
    elapsedTime += deltaTime;
    float t = elapsedTime / seconds;
    if (t > 1.0f) t = 1.0f;  // Clamp t to 1.0f to ensure movement stops at end

    // Interpolate position along the curve
    glm::vec3 start(startX, startY, startZ);
    glm::vec3 control(controlX, controlY, controlZ);
    glm::vec3 end(endX, endY, endZ);
    glm::vec3 newPos = QuadraticBezier(start, control, end, t);

    // Find the object and update its position
    for (Object* obj : currentScene->sceneObjects) {
        if (obj->name == objectName) {
            obj->mesh->positionXYZ = newPos;
            std::cout << "Updated Position: (" << newPos.x << ", " << newPos.y << ", " << newPos.z << ")\n";
            break;
        }
    }

    // Reset elapsed time if movement is complete
    if (t >= 1.0f) {
        elapsedTime = 0.0f;
    }

    return 0;  // No return values to Lua
}

int lua_FollowObject(lua_State* L) {
    const char* followerName = lua_tostring(L, 1);
    const char* targetName = lua_tostring(L, 2);
    float followDistance = lua_tonumber(L, 3);
    float followOffsetX = lua_tonumber(L, 4);
    float followOffsetY = lua_tonumber(L, 5);
    float followOffsetZ = lua_tonumber(L, 6);
    float maxSpeed = lua_tonumber(L, 7);
    float slowDownRange = lua_tonumber(L, 8);
    float speedUpRange = lua_tonumber(L, 9);
    float deltaTime = lua_tonumber(L, 10);

    // Call the Lua function here or use logic as needed
    // This implementation assumes Lua handles the actual movement.

    return 0;
}


int lua_FollowPosition(lua_State* L) {
    const char* objectName = lua_tostring(L, 1);
    float targetX = lua_tonumber(L, 2);
    float targetY = lua_tonumber(L, 3);
    float targetZ = lua_tonumber(L, 4);
    float followDistance = lua_tonumber(L, 5);
    float maxSpeed = lua_tonumber(L, 6);
    float slowDownRange = lua_tonumber(L, 7);
    float deltaTime = lua_tonumber(L, 8);

    // Handle logic in C++ or pass it directly to Lua
    // Assuming `GetObjectPosition` is available in your C++ code to retrieve the position of the object.
    // If you have the function implemented, pass the object position to Lua

    return 0;
}

// Function to change the texture of an object (called from Lua)
int lua_SetTexture(lua_State* L) {
    // Retrieve arguments from the Lua stack
    const char* objectName = luaL_checkstring(L, 1);  // Object name
    const char* textureName = luaL_checkstring(L, 2);  // Texture name
    int textureId = luaL_checkinteger(L, 3);  // Texture ID
    float blendAmount = luaL_checknumber(L, 4);  // Blend amount (float)

    lua_pushstring(L, "Texture applied successfully.");
    return 1;  // Return 1 value to Lua (success message)
}



// This is the function that Lua will call when 
//void g_Lua_AddSerialCommand(std::string theCommandText)
int g_Lua_AddSerialCommand(lua_State* L)
{
    //    std::cout << "**************************" << std::endl;
    //    std::cout << "g_Lua_AddSerialCommand() called" << std::endl;
    //    std::cout << "**************************" << std::endl;
        // AddSerialCommand() has been called
        // eg: AddSerialCommand('New_Viper_Player', -50.0, 15.0, 30.0, 5.0)

    std::string objectFriendlyName = lua_tostring(L, 1);      // 'New_Viper_Player'
    float x = (float)lua_tonumber(L, 2);                   // -50.0
    float y = (float)lua_tonumber(L, 3);                   // 15.0
    float z = (float)lua_tonumber(L, 4);                   // 30.0
    float timeSeconds = (float)lua_tonumber(L, 5);                   // 5.0

    std::vector<std::string> vecCommandDetails;
    vecCommandDetails.push_back(objectFriendlyName);    // Object command controls
    vecCommandDetails.push_back(::g_floatToString(x));
    vecCommandDetails.push_back(::g_floatToString(y));
    vecCommandDetails.push_back(::g_floatToString(z));
    vecCommandDetails.push_back(::g_floatToString(timeSeconds));

    iCommand* pMoveViper = ::g_pCommandFactory->pCreateCommandObject(
        "Move Relative ConstVelocity+Time", vecCommandDetails);

    ::g_pCommandDirector->addSerial(pMoveViper);

    // We'll return some value to indicate if the command worked or not
    // Here, we'll push "true" if it worked
    lua_pushboolean(L, true);
    // return 1 because we pushed 1 thing onto the stack
    return 1;
}

// Add object to scene through Lua
// AddMeshToScene('plyname.ply', 'friendlyName', x, y, z);
int g_Lua_AddMeshToScene(lua_State* L)
{


    sMesh* pNewMesh = new sMesh();
    pNewMesh->modelFileName = lua_tostring(L, 1);       // 'plyname.ply'
    pNewMesh->uniqueFriendlyName = lua_tostring(L, 2);  // Friendly name
    pNewMesh->positionXYZ.x = (float)lua_tonumber(L, 3);
    pNewMesh->positionXYZ.y = (float)lua_tonumber(L, 4);
    pNewMesh->positionXYZ.z = (float)lua_tonumber(L, 5);
    pNewMesh->textures[0] = lua_tostring(L, 6);
    pNewMesh->blendRatio[0] = (float)lua_tonumber(L, 7);
    //
    pNewMesh->bIsVisible = true;
    ::g_vecMeshesToDraw.push_back(pNewMesh);

    return 0;
}

static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}



static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);

    const float CAMERA_MOVE_SPEED = 10.0f;


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
    scene->sceneObjects[0]->mesh->textures[1] = "cloud.bmp";
    scene->sceneObjects[0]->mesh->blendRatio[0] = 2;
    scene->sceneObjects[0]->mesh->blendRatio[1] = 2;
    scene->sceneObjects[0]->mesh->bOverrideObjectColour = false;
    scene->sceneObjects[0]->mesh->transperency = 1;

    scene->sceneObjects[1]->mesh->textures[0] = "banners.bmp";
    scene->sceneObjects[1]->mesh->blendRatio[0] = 3;
    scene->sceneObjects[1]->mesh->bOverrideObjectColour = false;
    scene->sceneObjects[1]->mesh->transperency = 0.2;
    scene->sceneObjects[1]->mesh->textureSpeed.x = 1;


    scene->sceneObjects[2]->mesh->textures[0] = "barriers.bmp";
    scene->sceneObjects[2]->mesh->blendRatio[0] = 1;
    scene->sceneObjects[2]->mesh->bOverrideObjectColour = false;

    scene->sceneObjects[3]->mesh->textures[0] = "garages.bmp";
    scene->sceneObjects[3]->mesh->blendRatio[0] = 3;
    scene->sceneObjects[3]->mesh->bOverrideObjectColour = false;

    scene->sceneObjects[4]->mesh->textures[0] = "grass_1.bmp";
    scene->sceneObjects[4]->mesh->textures[1] = "Plant.bmp";
    scene->sceneObjects[4]->mesh->blendRatio[0] = 1;
    scene->sceneObjects[4]->mesh->blendRatio[1] = 0.2;
    scene->sceneObjects[4]->mesh->bOverrideObjectColour = false;

    scene->sceneObjects[5]->mesh->textures[0] = "grass_2.bmp";
    scene->sceneObjects[5]->mesh->blendRatio[0] = 3;
    scene->sceneObjects[5]->mesh->bOverrideObjectColour = false;

    scene->sceneObjects[6]->mesh->textures[0] = "gravel.bmp";
    scene->sceneObjects[6]->mesh->textures[1] = "Pebbles_island.bmp";
    scene->sceneObjects[6]->mesh->blendRatio[0] = 1;
    scene->sceneObjects[6]->mesh->blendRatio[1] = 0.2;
    scene->sceneObjects[6]->mesh->bOverrideObjectColour = false;

    scene->sceneObjects[7]->mesh->textures[0] = "metal_fence.bmp";
    scene->sceneObjects[7]->mesh->blendRatio[0] = 3;
    scene->sceneObjects[7]->mesh->bOverrideObjectColour = false;

    scene->sceneObjects[8]->mesh->textures[0] = "road.bmp";
    scene->sceneObjects[8]->mesh->blendRatio[0] = 1;
    scene->sceneObjects[8]->mesh->bOverrideObjectColour = false;
    scene->sceneObjects[8]->mesh->transperency = 1;

    scene->sceneObjects[8]->mesh->bIsStencilTexture = true;
    scene->sceneObjects[8]->mesh->stencilTexture = "kobold_stencil.bmp";
    scene->sceneObjects[8]->mesh->stencilTextureID = 61;
    scene->sceneObjects[8]->mesh->textureSpeed.x = 0.01f;

    scene->sceneObjects[9]->mesh->textures[0] = "rock.bmp";
    scene->sceneObjects[9]->mesh->textures[1] = "Pebbles_small.bmp";
    scene->sceneObjects[9]->mesh->textures[2] = "Ground.bmp";
    scene->sceneObjects[9]->mesh->blendRatio[0] = 2.5;
    scene->sceneObjects[9]->mesh->blendRatio[1] = 0.4;
    scene->sceneObjects[9]->mesh->blendRatio[2] = 0.4;
    scene->sceneObjects[9]->mesh->bOverrideObjectColour = false;

    scene->sceneObjects[10]->mesh->textures[0] = "trees.bmp";
    scene->sceneObjects[10]->mesh->blendRatio[0] = 3;
    scene->sceneObjects[10]->mesh->bOverrideObjectColour = false;

    scene->sceneObjects[11]->mesh->textures[0] = "tyres.bmp";
    scene->sceneObjects[11]->mesh->blendRatio[0] = 3;
    scene->sceneObjects[11]->mesh->bOverrideObjectColour = false;

    scene->sceneObjects[13]->mesh->textures[0] = "cloud.bmp";
    scene->sceneObjects[13]->mesh->blendRatio[0] = 2;
    scene->sceneObjects[13]->mesh->bOverrideObjectColour = false;
    scene->sceneObjects[13]->mesh->transperency = 0.9;

    scene->sceneObjects[14]->mesh->textures[0] = "yellow.bmp";
    scene->sceneObjects[14]->mesh->blendRatio[0] = 2;
    scene->sceneObjects[14]->mesh->bOverrideObjectColour = false;
    scene->sceneObjects[14]->mesh->transperency = 0.2;

    //class room
    scene->sceneObjects[15]->mesh->textures[0] = "chairs.bmp";
    scene->sceneObjects[15]->mesh->blendRatio[0] = 2;
    scene->sceneObjects[15]->mesh->bOverrideObjectColour = false;

    scene->sceneObjects[16]->mesh->textures[0] = "board.bmp";
    scene->sceneObjects[16]->mesh->blendRatio[0] = 2;
    scene->sceneObjects[16]->mesh->bOverrideObjectColour = false;

    scene->sceneObjects[17]->mesh->textures[0] = "casier.bmp";
    scene->sceneObjects[17]->mesh->blendRatio[0] = 2;
    scene->sceneObjects[17]->mesh->bOverrideObjectColour = false;

    scene->sceneObjects[18]->mesh->textures[0] = "ceiling.bmp";
    scene->sceneObjects[18]->mesh->blendRatio[0] = 2;
    scene->sceneObjects[18]->mesh->bOverrideObjectColour = false;

    scene->sceneObjects[19]->mesh->textures[0] = "clock.bmp";
    scene->sceneObjects[19]->mesh->blendRatio[0] = 2;
    scene->sceneObjects[19]->mesh->bOverrideObjectColour = false;

    scene->sceneObjects[20]->mesh->textures[0] = "desk.bmp";
    scene->sceneObjects[20]->mesh->blendRatio[0] = 2;
    scene->sceneObjects[20]->mesh->bOverrideObjectColour = false;

    scene->sceneObjects[21]->mesh->textures[0] = "door.bmp";
    scene->sceneObjects[21]->mesh->blendRatio[0] = 2;
    scene->sceneObjects[21]->mesh->bOverrideObjectColour = false;

    scene->sceneObjects[22]->mesh->textures[0] = "etagere.bmp";
    scene->sceneObjects[22]->mesh->blendRatio[0] = 2;
    scene->sceneObjects[22]->mesh->bOverrideObjectColour = false;

    scene->sceneObjects[23]->mesh->textures[0] = "grand_casier.bmp";
    scene->sceneObjects[23]->mesh->blendRatio[0] = 2;
    scene->sceneObjects[23]->mesh->bOverrideObjectColour = false;

    scene->sceneObjects[24]->mesh->textures[0] = "ground.bmp";
    scene->sceneObjects[24]->mesh->blendRatio[0] = 2;
    scene->sceneObjects[24]->mesh->bOverrideObjectColour = false;

    scene->sceneObjects[25]->mesh->textures[0] = "teacher_d.bmp";
    scene->sceneObjects[25]->mesh->blendRatio[0] = 2;
    scene->sceneObjects[25]->mesh->bOverrideObjectColour = false;

    scene->sceneObjects[26]->mesh->textures[0] = "wall_c.bmp";
    scene->sceneObjects[26]->mesh->blendRatio[0] = 2;
    scene->sceneObjects[26]->mesh->bOverrideObjectColour = false;



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
    currentScene = scene;
    

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
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(1024, 780, "OpenGL Triangle", NULL, NULL);
    




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
    scene->textureManager->Create2DTextureFromBMPFile("yellow.bmp");
    scene->textureManager->Create2DTextureFromBMPFile("cloud.bmp");
    scene->textureManager->Create2DTextureFromBMPFile("Ground.bmp");
    scene->textureManager->Create2DTextureFromBMPFile("Pebbles_small.bmp");
    scene->textureManager->Create2DTextureFromBMPFile("Pebbles_island.bmp");
    scene->textureManager->Create2DTextureFromBMPFile("Plant.bmp");

    //Effects
    scene->textureManager->Create2DTextureFromBMPFile("kobold_stencil.bmp");


    //class
    scene->textureManager->Create2DTextureFromBMPFile("chairs.bmp");
    scene->textureManager->Create2DTextureFromBMPFile("board.bmp");
    scene->textureManager->Create2DTextureFromBMPFile("casiers.bmp");
    scene->textureManager->Create2DTextureFromBMPFile("ceiling.bmp");
    scene->textureManager->Create2DTextureFromBMPFile("clock.bmp");
    scene->textureManager->Create2DTextureFromBMPFile("desk.bmp");
    scene->textureManager->Create2DTextureFromBMPFile("door.bmp");
    scene->textureManager->Create2DTextureFromBMPFile("etagere.bmp");
    scene->textureManager->Create2DTextureFromBMPFile("grand_casier.bmp");
    scene->textureManager->Create2DTextureFromBMPFile("ground.bmp");
    scene->textureManager->Create2DTextureFromBMPFile("teacher_d.bmp");
    scene->textureManager->Create2DTextureFromBMPFile("wall_c.bmp");





    std::cout << "Skybox Texture Load Start" << std::endl;

    // lookings
    std::string errorString;
    if (scene->textureManager->CreateCubeTextureFromBMPFiles("Space",
        "CubeMaps/TropicalSunnyDayLeft2048.bmp",
        "CubeMaps/TropicalSunnyDayRight2048.bmp",
        "CubeMaps/TropicalSunnyDayUp2048.bmp",
        "CubeMaps/TropicalSunnyDayDown2048.bmp",
        "CubeMaps/TropicalSunnyDayFront2048.bmp",
        "CubeMaps/TropicalSunnyDayBack2048.bmp", true, errorString))
    {
        std::cout << "Loaded space skybox" << std::endl;
    }
    else
    {
        std::cout << "ERROR: Didn't load space skybox because: " << errorString << std::endl;
    }

    std::cout << "Skybox Texture Load End" << std::endl;
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

    Object* SkySphere = scene->GenerateMeshObjectsFromObject("assets/models/Sphere_radius_1_xyz_N_uv.ply",
        glm::vec3(0, 0, 0),
        1,
        glm::vec3(0, 0, 0),
        false,
        glm::vec4(0.5f, 0.5f, 0.5f, 1.0f),
        true,
        scene->sceneObjects);
    SkySphere->mesh->textures[0] = "tyres.bmp";
    SkySphere->isTemporary = true;


    SkySphere->mesh->transperency = 1;
    glUniform1f(glGetUniformLocation(program, "wholeObjectTransparencyAlpha"),  SkySphere->mesh->transperency);

    Object* RacingCar = scene->GenerateMeshObjectsFromObject(
        "assets/models/Cube_xyz_n_uv.ply",
        glm::vec3(0, 100, 0),
        0.1,
        glm::vec3(0, 0, 0),
        false,
        glm::vec4(0.5f, 0.5f, 0.5f, 1.0f),
        true,
        scene->sceneObjects
    );
    RacingCar->mesh->textures[0] = "desk.bmp";
    RacingCar->mesh->uniformScale = 30.f;
    RacingCar->isTemporary = true;
    RacingCar->name = "racing_desk";

    //glm::vec3 currentPos(0.0f, 0.0f, 0.0f);  // Initial position
    //glm::vec3 startXYZ(0.0f, 0.0f, 0.0f);   // Start point
    //glm::vec3 endXYZ(2000.0f, 0.0f, 0.0f);    // End point
    //float seconds = 30.0f;                   // Time to reach the target
    //float deltaTime = 0.016f;               // Simulated frame time (60 FPS)

    //for (int i = 0; i < 300; ++i) {         // Simulate 300 frames (~5 seconds)
    //    MoveTo(currentPos, startXYZ, endXYZ, seconds, deltaTime);
    //}



    aLuaScript* luaScript = new aLuaScript();
    luaScript->luaPath = "LuaRotate2Lerp.lua";
    scene->AddActionToObj(luaScript, RacingCar);

    aLuaScriptsSerial* luaAction = new aLuaScriptsSerial();
  //  scene->AddActionToObj(luaAction, RacingCar);
  /*  luaAction->AddMoveScript("LuaMove2Lerp.lua", glm::vec3(0, 0, 0), glm::vec3(10, 100, 0), 3, glm::vec3(0,0,0));
    luaAction->AddMoveScript("LuaRotate2Lerp.lua", glm::vec3(0, 0, 0), glm::vec3(10, 100, 0), 3, glm::vec3(0, 0, 0));*/
    luaAction->AddMoveScript("LuaMove2Curve.lua", glm::vec3(10, 300, 0), glm::vec3(200, 300, 0), 1, glm::vec3(100, 500, 0));
    luaAction->AddMoveScript("LuaRotate2Lerp.lua", glm::vec3(0, 0, 0), glm::vec3(360, 0, 0), 0.4, glm::vec3(360, 0, 0)); 
    luaAction->AddMoveScript("LuaMove2Curve.lua", glm::vec3(200, 300, 0), glm::vec3(10, 300, 0), 1, glm::vec3(100, 100, 0));
    luaAction->AddMoveScript("LuaRotate2Lerp.lua", glm::vec3(360, 0, 0), glm::vec3(0, 0, 0), 0.4, glm::vec3(0, 0, 0));

  
    aLocTrggersLua* triggerAction = new aLocTrggersLua();
    triggerAction->AddTrigger(glm::vec3(100, 100, 0), 50, luaScript);
    scene->AddActionToObj(triggerAction, RacingCar);


    scene->Start();



    //  Turn on the blend operation
    glEnable(GL_BLEND);
    // Do alpha channel transparency
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);

    // HACK:
    unsigned int numberOfNarrowPhaseTrianglesInAABB_BroadPhaseThing = 0;

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

        SkySphere->mesh->positionXYZ = scene->fCamera->getEyeLocation();
        //SkySphere->mesh->positionXYZ.x -= 5.0f;


        SkySphere->mesh->bIsVisible = true;
        //        pSkySphere->bDoNotLight = true;

        SkySphere->mesh->uniformScale = 25.0f;
        DrawSkyBox(SkySphere->mesh, program, scene->vaoManager, scene->textureManager);
//      DRAW LOOP
//      ------------------------------------------       
        scene->SortObjectsForDrawing();
        for (Object* object:scene->sceneObjectsSorted)
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





