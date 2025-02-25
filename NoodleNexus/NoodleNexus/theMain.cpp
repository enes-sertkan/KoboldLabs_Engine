//#define GLAD_GL_IMPLEMENTATION
//#include <glad/glad.h>
//
//#define GLFW_INCLUDE_NONE
//#include <GLFW/glfw3.h>

#define GLM_ENABLE_EXPERIMENTAL 
#define GLFW_INCLUDE_NONE
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
//#include "GridRenderer.h"
#include "aRayCastPhysics.h"
#include "aDrawAim.hpp"
#include "aPlayerItemsController.h"
#include "ModelsLoader.hpp"
#include "aLuaScript.h"
#include "aLuaScriptsSerial.h"
#include "aLocTriggersLua.h"
#include "Animator.h"
#include "aTextureWiggler.h"
#include "aPlaneMovement.hpp"
#include "aPlanePhysics.h"
#include "cFBO_RGB_depth.hpp"
#include "aCameraToTexture.h"
 Scene* currentScene=nullptr;



std::vector<sMesh*> g_vecMeshesToDraw;

cPhysics* g_pPhysicEngine = NULL;
// This loads the 3D models for drawing, etc.
cVAOManager* g_pMeshManager = NULL;

cBasicTextureManager* g_pTextures = NULL;

cCommandGroup* g_pCommandDirector = NULL;
cCommandFactory* g_pCommandFactory = NULL;

void DrawMesh(sMesh* pCurMesh, GLuint program, cVAOManager* vaoManager, cBasicTextureManager* textureManager, Scene* scene);
void DrawCameraViewToFramebufer(Camera* camera, int programID, int framebufferID);
void DrawCameraView(Camera* camera, int programID);
void DrawSkyBox(sMesh* pCurMesh, GLuint program, cVAOManager* vaoManager, cBasicTextureManager* textureManager, Camera* camera);


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
            std::cout << "current Rotation X: " << rotationX << " Y: " << rotationY << " Z: " << rotationZ << std::endl;
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



// Function to change the texture of an object (called from Lua)
int lua_SetTexture(lua_State* L) {
    // Retrieve arguments from the Lua stack
    const char* objectName = luaL_checkstring(L, 1);  // Object name
    const char* textureName = luaL_checkstring(L, 2);  // Texture name
    int textureId = luaL_checkinteger(L, 3);  // Texture ID
    float blendAmount = luaL_checknumber(L, 4);  // Blend amount (float)

    for (Object* obj : currentScene->sceneObjects) {
        if (obj->name == objectName) {
           
            obj->mesh->textures[textureId] = textureName;
            obj->mesh->blendRatio[textureId] = blendAmount;
            std::cout << "[C++] Texture applied successfully." << std::endl;
            break;
        }
    }

    return 0; 
}

int lua_SetMeshTransparency(lua_State* L) {
    // Retrieve arguments from the Lua stack
    const char* objectName = luaL_checkstring(L, 1); // Object name
    float transparency = luaL_checknumber(L, 2);    // Transparency value


    bool found = false;
    for (Object* obj : currentScene->sceneObjects) {
        if (obj->name == objectName) {
            found = true;
            if (obj->mesh != nullptr) {
                obj->mesh->transperency = transparency;
                std::cout << "[C++] Transparency for object " << objectName
                    << " set to " << transparency << "." << std::endl;
                lua_pushstring(L, "Mesh transparency updated successfully.");
                return 1; // Return success
            }
            else {
                std::cerr << "[C++] Error: Mesh for object " << objectName << " is nullptr." << std::endl;
                lua_pushstring(L, "Mesh transparency update failed: Mesh is nullptr.");
                return 0; // Return failure
            }
        }
    }

    if (!found) {
        std::cerr << "[C++] Transparency update failed: Object not found: " << objectName << "." << std::endl;
        lua_pushstring(L, "Mesh transparency update failed: Object not found.");
        return 0; // Return failure
    }

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

//TODO : GRIDS

//GLuint LoadShaders(const char* vertex_file_path, const char* fragment_file_path) {
//    // Create the shaders
//    GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
//    GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
//
//    // Read the Vertex Shader code from the file
//    std::string VertexShaderCode;
//    std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
//    if (VertexShaderStream.is_open()) {
//        std::stringstream sstr;
//        sstr << VertexShaderStream.rdbuf();
//        VertexShaderCode = sstr.str();
//        VertexShaderStream.close();
//    }
//    else {
//        printf("Impossible to open %s. Are you in the right directory?\n", vertex_file_path);
//        return 0;
//    }
//
//    // Read the Fragment Shader code from the file
//    std::string FragmentShaderCode;
//    std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
//    if (FragmentShaderStream.is_open()) {
//        std::stringstream sstr;
//        sstr << FragmentShaderStream.rdbuf();
//        FragmentShaderCode = sstr.str();
//        FragmentShaderStream.close();
//    }
//    else {
//        printf("Impossible to open %s. Are you in the right directory?\n", fragment_file_path);
//        return 0;
//    }
//
//    GLint Result = GL_FALSE;
//    int InfoLogLength;
//
//    // Compile Vertex Shader
//    printf("Compiling shader : %s\n", vertex_file_path);
//    char const* VertexSourcePointer = VertexShaderCode.c_str();
//    glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
//    glCompileShader(VertexShaderID);
//
//    // Check Vertex Shader
//    glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
//    glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
//    if (InfoLogLength > 0) {
//        std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
//        glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
//        printf("%s\n", &VertexShaderErrorMessage[0]);
//    }
//
//    // Compile Fragment Shader
//    printf("Compiling shader : %s\n", fragment_file_path);
//    char const* FragmentSourcePointer = FragmentShaderCode.c_str();
//    glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
//    glCompileShader(FragmentShaderID);
//
//    // Check Fragment Shader
//    glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
//    glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
//    if (InfoLogLength > 0) {
//        std::vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
//        glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
//        printf("%s\n", &FragmentShaderErrorMessage[0]);
//    }
//
//    // Link the program
//    printf("Linking program\n");
//    GLuint ProgramID = glCreateProgram();
//    glAttachShader(ProgramID, VertexShaderID);
//    glAttachShader(ProgramID, FragmentShaderID);
//    glLinkProgram(ProgramID);
//
//    // Check the program
//    glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
//    glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
//    if (InfoLogLength > 0) {
//        std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
//        glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
//        printf("%s\n", &ProgramErrorMessage[0]);
//    }
//
//    glDetachShader(ProgramID, VertexShaderID);
//    glDetachShader(ProgramID, FragmentShaderID);
//
//    glDeleteShader(VertexShaderID);
//    glDeleteShader(FragmentShaderID);
//
//    return ProgramID;
//}
//
//void SetupGridVAO(GLuint& vao, GLuint& vbo) {
//    // Vertex data for the grid (a single quad covering the entire screen)
//    const float vertices[] = {
//        -1.0f, 0.0f, -1.0f,
//         1.0f, 0.0f, -1.0f,
//         1.0f, 0.0f,  1.0f,
//        -1.0f, 0.0f,  1.0f
//    };
//
//    // Indices for the quad
//    const unsigned int indices[] = {
//        0, 1, 2,
//        2, 3, 0
//    };
//
//    // Generate and bind the VAO
//    glGenVertexArrays(1, &vao);
//    glBindVertexArray(vao);
//
//    // Generate and bind the VBO
//    glGenBuffers(1, &vbo);
//    glBindBuffer(GL_ARRAY_BUFFER, vbo);
//    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
//
//    // Set up vertex attribute pointers
//    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
//    glEnableVertexAttribArray(0);
//
//    // Generate and bind the EBO
//    GLuint ebo;
//    glGenBuffers(1, &ebo);
//    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
//    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
//
//    // Unbind the VAO
//    glBindVertexArray(0);
//}
//
//void SetShaderUniforms(GLuint shaderProgram, const glm::mat4& VP, const glm::vec3& cameraPos, float gridSize, float gridCellSize) {
//    glUseProgram(shaderProgram);
//
//    // Pass the VP matrix
//    GLuint VPUniform = glGetUniformLocation(shaderProgram, "gVP");
//    glUniformMatrix4fv(VPUniform, 1, GL_FALSE, glm::value_ptr(VP)); // Use glm::value_ptr
//
//    // Pass the camera position
//    GLuint cameraPosUniform = glGetUniformLocation(shaderProgram, "gCameraWorldPos");
//    glUniform3f(cameraPosUniform, cameraPos.x, cameraPos.y, cameraPos.z);
//
//    // Pass the grid size and cell size
//    GLuint gridSizeUniform = glGetUniformLocation(shaderProgram, "gGridSize");
//    glUniform1f(gridSizeUniform, gridSize);
//
//    GLuint gridCellSizeUniform = glGetUniformLocation(shaderProgram, "gGridCellSize");
//    glUniform1f(gridCellSizeUniform, gridCellSize);
//}


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

    //cShaderManager::cShader infinite_grid_vs;
    //infinite_grid_vs.fileName = "assets/shaders/infinite_grid.vs";

    //cShaderManager::cShader infinite_grid_frag;
    //infinite_grid_vs.fileName = "assets/shaders/infinite_grid.frag";

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

    //TODO : GRIDS
    //GLuint gridVAO, gridVBO;
    //SetupGridVAO(gridVAO, gridVBO);

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

void calcBoxXYFromCoord(float x, float y, int& xIndex, int& yIndex, float boxSize)
{
    xIndex = (int)(x / boxSize);
    yIndex = (int)(y / boxSize);
    return;
}

void AABBOctTree(void)
{
    struct sSquare
    {
        //       vector< cTriangles* > vecTriangleInThisSquare
        glm::vec2 minXY;
        glm::vec2 maxXY;
        float width;
        unsigned int indexColRow;
    };

    sSquare grid[10][10];
    float sqaureWidth = 10;

    for (unsigned int x = 0; x < 10; x++)
    {
        for (unsigned int y = 0; y < 10; y++)
        {
            grid[x][y].width = sqaureWidth;
            grid[x][y].minXY.x = sqaureWidth * x;
            grid[x][y].minXY.y = sqaureWidth * y;

            grid[x][y].maxXY.x = sqaureWidth * x + sqaureWidth;
            grid[x][y].maxXY.y = sqaureWidth * y + sqaureWidth;
        }
    }

    int xIndex, yIndex;
    calcBoxXYFromCoord(5.0f, 15.0f, xIndex, yIndex, sqaureWidth);
    std::cout << xIndex << ", " << yIndex << std::endl;


    calcBoxXYFromCoord(40.0f, 80.0f, xIndex, yIndex, sqaureWidth);
    std::cout << xIndex << ", " << yIndex << std::endl;




    return;
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
    Object* cameraObj = scene->sceneObjects[0];
    CameraToTexture* textureCamera = new CameraToTexture();
    scene->AddActionToObj(textureCamera, cameraObj);



    //Object* playerObject = scene->sceneObjects[15];

    //// Add the player camera action (with an offset for camera positioning)
    //aPlayerCamera* playerCameraAction = new aPlayerCamera(::g_pFlyCamera, glm::vec3(0.0f, 10.0f, 0.0f));
    //scene->AddActionToObj(playerCameraAction, playerObject);



    //RayCastPhysics* phys = new RayCastPhysics;
    //phys->gravityAcceleration.y = -5;
    //phys->baseRayCastLength = 10.0;
    //scene->AddActionToObj(phys, scene->sceneObjects[1]);



    //aDrawAim* drawAimAction = new aDrawAim();
    //drawAimAction->program = program;
    //scene->AddActionToObj(drawAimAction, scene->sceneObjects[1]);


    //aPlayerItemsController* itemsControllerAction = new aPlayerItemsController();
    //scene->AddActionToObj(itemsControllerAction, scene->sceneObjects[1]);


    {

        scene->sceneObjects[0]->mesh->textures[0] = "camera1";
        scene->sceneObjects[0]->mesh->blendRatio[0] = 10;
        scene->sceneObjects[0]->mesh->bOverrideObjectColour = false;
        //scene->sceneObjects[0]->mesh->transperency = 1;
        //scene->sceneObjects[0]->mesh->textureFillType[0] = 1;

        scene->sceneObjects[1]->mesh->textures[0] = "camera1";
        scene->sceneObjects[1]->mesh->blendRatio[0] = 3;
        scene->sceneObjects[1]->mesh->bOverrideObjectColour = false;
        //scene->sceneObjects[1]->mesh->transperency = 0.2;
        //scene->sceneObjects[1]->mesh->textureSpeed.x = 1;

        scene->sceneObjects[2]->mesh->textures[0] = "camera1";
        scene->sceneObjects[2]->mesh->blendRatio[0] = 1;
        scene->sceneObjects[2]->mesh->bOverrideObjectColour = false;

        scene->sceneObjects[3]->mesh->textures[0] = "camera1";
        scene->sceneObjects[3]->mesh->blendRatio[0] = 3;
        scene->sceneObjects[3]->mesh->bOverrideObjectColour = false;

        scene->sceneObjects[4]->mesh->textures[0] = "SpaceInteriors_Texture.bmp";
        scene->sceneObjects[4]->mesh->textures[1] = "SpaceInteriors_Emmision.bmp";
        scene->sceneObjects[4]->mesh->blendRatio[0] = 1;
        scene->sceneObjects[4]->mesh->blendRatio[1] = 1;
        scene->sceneObjects[4]->mesh->textureFillType[1] = 1;
        scene->sceneObjects[4]->mesh->bOverrideObjectColour = false;

        scene->sceneObjects[5]->mesh->textures[0] = "camera1.bmp";
        scene->sceneObjects[5]->mesh->textures[1] = "SpaceInteriors_Emmision.bmp";
        scene->sceneObjects[5]->mesh->blendRatio[0] = 1;
        scene->sceneObjects[5]->mesh->blendRatio[1] = 1;
        scene->sceneObjects[5]->mesh->textureFillType[0] = 1;
        scene->sceneObjects[5]->mesh->bOverrideObjectColour = false;

        scene->sceneObjects[6]->mesh->textures[0] = "SpaceInteriors_Texture.bmp";
        scene->sceneObjects[6]->mesh->textures[1] = "SpaceInteriors_Emmision.bmp";
        scene->sceneObjects[6]->mesh->blendRatio[0] = 1;
        scene->sceneObjects[6]->mesh->blendRatio[1] = 1;
        scene->sceneObjects[0]->mesh->textureFillType[1] = 1;
        scene->sceneObjects[6]->mesh->bOverrideObjectColour = false;

        scene->sceneObjects[7]->mesh->textures[0] = "camera1";
        scene->sceneObjects[7]->mesh->blendRatio[0] = 3;


        scene->sceneObjects[8]->mesh->textures[0] = "grass_2.bmp";
        scene->sceneObjects[8]->mesh->blendRatio[0] = 1;
        scene->sceneObjects[8]->mesh->bOverrideObjectColour = false;
        //scene->sceneObjects[8]->mesh->transperency = 1;

        scene->sceneObjects[9]->mesh->textures[0] = "camera1";
        scene->sceneObjects[9]->mesh->blendRatio[0] = 2.5;


        scene->sceneObjects[10]->mesh->textures[0] = "uv_mapper.bmp";
        scene->sceneObjects[10]->mesh->blendRatio[0] = 3;
        scene->sceneObjects[10]->mesh->bOverrideObjectColour = false;
        scene->sceneObjects[10]->mesh->bIsStencilTexture = true;
        scene->sceneObjects[10]->mesh->stencilTexture = "WorldMap.bmp";
        scene->sceneObjects[10]->mesh->stencilTextureID = 61;
        scene->sceneObjects[10]->mesh->textureSpeed.x = 0.1f;

        scene->sceneObjects[11]->mesh->textures[0] = "WorldMap.bmp";
        scene->sceneObjects[11]->mesh->blendRatio[0] = 3;
        scene->sceneObjects[11]->mesh->bOverrideObjectColour = false;

        scene->sceneObjects[13]->mesh->textures[0] = "uv_mapper.bmp";
        scene->sceneObjects[13]->mesh->blendRatio[0] = 2;
        scene->sceneObjects[13]->mesh->bOverrideObjectColour = false;

        scene->sceneObjects[14]->mesh->textures[0] = "uv_mapper.bmp";
        scene->sceneObjects[14]->mesh->blendRatio[0] = 2;
        scene->sceneObjects[14]->mesh->bOverrideObjectColour = false;
        scene->sceneObjects[14]->mesh->bIsStencilTexture = true;
        scene->sceneObjects[14]->mesh->stencilTexture = "WorldMap.bmp";
        scene->sceneObjects[14]->mesh->stencilTextureID = 61;
        scene->sceneObjects[14]->mesh->textureSpeed.x = 0.1f;

        //class room

        //scene->sceneObjects[15]->mesh->textures[0] = "SHD_UFO_AO.bmp";
        //scene->sceneObjects[15]->mesh->textures[1] = "SHD_UFO_roughness.bmp";
        //scene->sceneObjects[15]->mesh->blendRatio[0] = 1;
        //scene->sceneObjects[15]->mesh->blendRatio[1] = 1;
        //scene->sceneObjects[15]->mesh->bOverrideObjectColour = false;


        //scene->sceneObjects[16]->mesh->textures[0] = "board.bmp";
        //scene->sceneObjects[16]->mesh->blendRatio[0] = 2;
        //scene->sceneObjects[16]->mesh->bOverrideObjectColour = false;

        //scene->sceneObjects[17]->mesh->textures[0] = "casier.bmp";
        //scene->sceneObjects[17]->mesh->blendRatio[0] = 2;
        //scene->sceneObjects[17]->mesh->bOverrideObjectColour = false;

        //scene->sceneObjects[18]->mesh->textures[0] = "ceiling.bmp";
        //scene->sceneObjects[18]->mesh->blendRatio[0] = 2;
        //scene->sceneObjects[18]->mesh->bOverrideObjectColour = false;

        //scene->sceneObjects[19]->mesh->textures[0] = "clock.bmp";
        //scene->sceneObjects[19]->mesh->blendRatio[0] = 2;
        //scene->sceneObjects[19]->mesh->bOverrideObjectColour = false;

        //scene->sceneObjects[20]->mesh->textures[0] = "desk.bmp";
        //scene->sceneObjects[20]->mesh->blendRatio[0] = 2;
        //scene->sceneObjects[20]->mesh->bOverrideObjectColour = false;

        //scene->sceneObjects[21]->mesh->textures[0] = "door.bmp";
        //scene->sceneObjects[21]->mesh->blendRatio[0] = 2;
        //scene->sceneObjects[21]->mesh->bOverrideObjectColour = false;

        //scene->sceneObjects[22]->mesh->textures[0] = "etagere.bmp";
        //scene->sceneObjects[22]->mesh->blendRatio[0] = 2;
        //scene->sceneObjects[22]->mesh->bOverrideObjectColour = false;

        //scene->sceneObjects[23]->mesh->textures[0] = "grand_casier.bmp";
        //scene->sceneObjects[23]->mesh->blendRatio[0] = 2;
        //scene->sceneObjects[23]->mesh->bOverrideObjectColour = false;

        //scene->sceneObjects[24]->mesh->textures[0] = "ground.bmp";
        //scene->sceneObjects[24]->mesh->blendRatio[0] = 2;
        //scene->sceneObjects[24]->mesh->bOverrideObjectColour = false;

        //scene->sceneObjects[25]->mesh->textures[0] = "teacher_d.bmp";
        //scene->sceneObjects[25]->mesh->blendRatio[0] = 2;
        //scene->sceneObjects[25]->mesh->bOverrideObjectColour = false;

        //scene->sceneObjects[26]->mesh->textures[0] = "wall_c.bmp";
        //scene->sceneObjects[26]->mesh->blendRatio[0] = 2;
        //scene->sceneObjects[26]->mesh->bOverrideObjectColour = false;
    }



}

//MAIN
//-----------------------------------------------------------------------------------------------------------------------------

int main(void)
{


    AABBOctTree();

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

    GLFWwindow* window = glfwCreateWindow(1024, 780, "KoboldLabs Engine", NULL, NULL);
    




//   PREPARING OPENGL
//   ----------------

    GLuint program = PrepareOpenGL(window);

    //NOTE : GRIDS
    //GridRenderer gridRenderer;
    //if (!gridRenderer.Initialize()) {
    //    std::cerr << "Failed to initialize grid renderer!" << std::endl;
    //    return EXIT_FAILURE;
    //}
   



//   PREPARING ENGINE STUFF
//   ----------------------

    scene->vaoManager = new cVAOManager();
    PhysicsManager* physicsMan = new PhysicsManager();
    PrepareFlyCamera();


//   PREPARING SCENE
//   ---------------
    scene->Prepare(scene->vaoManager, program, physicsMan, window, g_pFlyCamera);
    AddActions(scene, program);

    Animator* animator = new Animator();
    animator->scene = scene;
    

    cPhysics* g_pPhysicEngine = new cPhysics();
    g_pPhysicEngine->setVAOManager(scene->vaoManager);


//   PREPARING SOMETHING ELSE (TODO: Try to put it away into functions)
//   ------------------------


    glUseProgram(program);


    scene->textureManager->SetBasePath("assets/textures");
    scene->textureManager->Create2DTextureFromBMPFile("ceilling.bmp");
    scene->textureManager->Create2DTextureFromBMPFile("graphity.bmp");
    scene->textureManager->Create2DTextureFromBMPFile("grass_1.bmp");
    scene->textureManager->Create2DTextureFromBMPFile("grass_2.bmp");
    scene->textureManager->Create2DTextureFromBMPFile("gravel.bmp");
    scene->textureManager->Create2DTextureFromBMPFile("metal.bmp");
    scene->textureManager->Create2DTextureFromBMPFile("Pebbles_small.bmp");
    scene->textureManager->Create2DTextureFromBMPFile("rock.bmp");
    scene->textureManager->Create2DTextureFromBMPFile("wall_c.bmp");
    scene->textureManager->Create2DTextureFromBMPFile("SpaceInteriors_Emmision.bmp");
    scene->textureManager->Create2DTextureFromBMPFile("SpaceInteriors_Texture.bmp");
    scene->textureManager->Create2DTextureFromBMPFile("WorldMap.bmp");
    scene->textureManager->Create2DTextureFromBMPFile("uv_mapper.bmp");


    std::cout << "Skybox Texture Load Start" << std::endl;

    // lookings
    std::string errorString;
    if (scene->textureManager->CreateCubeTextureFromBMPFiles("Space",
        "CubeMaps/SpaceBox_left2_negX.bmp",
        "CubeMaps/SpaceBox_right1_posX.bmp",
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

    {
        Object* RacingCar = scene->GenerateMeshObjectsFromObject(
            "assets/models/Bodiam_Castle.ply",
            glm::vec3(11, -600, 103),
            10,
            glm::vec3(0, 0, 0),
            false,
            glm::vec4(0.5f, 0.5f, 0.5f, 1.0f),
            true,
            scene->sceneObjects
        );
        RacingCar->mesh->textures[0] = "BodiamCastle.bmp";
        RacingCar->mesh->uniformScale = 110.f;
        RacingCar->isTemporary = true;
        RacingCar->name = "Castle";
        RacingCar->mesh->bIsVisible = true;

        std::cout << "Generating Broadfaces." << std::endl;

        g_pPhysicEngine->generateBroadPhaseGrid(
            "assets/models/Bodiam_Castle.ply",
            200.0f,                            // AABB Cube region size
            RacingCar->mesh->positionXYZ,
            RacingCar->mesh->rotationEulerXYZ,
            RacingCar->mesh->uniformScale, scene->vaoManager);

        std::cout << "Generating Broadfaces. DONE." << std::endl;

        scene->physicsManager->AddTriangleMesh("assets/models/Bodiam_Castle.ply",                          // AABB Cube region size
            RacingCar->mesh->positionXYZ,
            RacingCar->mesh->rotationEulerXYZ,
            RacingCar->mesh->uniformScale);
        // Debug AABB shape
        sMesh* pAABBCube_MinAtOrigin = new sMesh();
        pAABBCube_MinAtOrigin->modelFileName = "assets/models/Cube_xyz_n_uv.ply";
        pAABBCube_MinAtOrigin->bIsWireframe = true;
        pAABBCube_MinAtOrigin->objectColourRGBA = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
        pAABBCube_MinAtOrigin->bOverrideObjectColour = true;
        pAABBCube_MinAtOrigin->bDoNotLight = true;
        pAABBCube_MinAtOrigin->bIsVisible = false;
        pAABBCube_MinAtOrigin->uniqueFriendlyName = "AABB_MinXYZ_At_Origin";

        ::g_vecMeshesToDraw.push_back(pAABBCube_MinAtOrigin);

    }

    glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
  


    scene->Start();

    //  Turn on the blend operation
    glEnable(GL_BLEND);
    // Do alpha channel transparency
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);

    // HACK:
    unsigned int numberOfNarrowPhaseTrianglesInAABB_BroadPhaseThing = 0;
    scene->AddCamera(glm::vec3(0.f), glm::vec3(0.f), glm::vec2(1920.f, 1080.f));
  
    while (!glfwWindowShouldClose(window))
    {
        //followObj->start = RacingCar->mesh->positionXYZ;
        //followObj->end = scene->sceneObjects[27]->mesh->positionXYZ;
        //float ratio;
        //int width, height;
        //glfwGetFramebufferSize(window, &width, &height);
        //ratio = width / (float)height;
        //glViewport(0, 0, width, height);
        //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

 /*       followScript->start = RacingCar->mesh->positionXYZ;
        followScript->end = scene->sceneObjects[27]->mesh->positionXYZ;*/


//      UPDATE
//      ------------------------------------------
       // SetCameraAndProjectionMatrices(ratio, program);
        scene->lightManager->updateShaderWithLightInfo();
        sceneEditor->Update();
        scene->Update();

        scene->sceneObjects[0]->mesh->positionXYZ = scene->fCamera->getEyeLocation();
        scene->sceneObjects[0]->mesh->rotationEulerXYZ = scene->fCamera->getCameraData()->rotation;


        SkySphere->mesh->positionXYZ = scene->fCamera->getCameraData()->position;
        //SkySphere->mesh->positionXYZ.x -= 5.0f;


        SkySphere->mesh->bIsVisible = true;
        //        pSkySphere->bDoNotLight = true;

        SkySphere->mesh->uniformScale = 25.0f;

        /*scene->lightManager->theLights[1].position.x = scene->sceneObjects[15]->mesh->positionXYZ.x;
        scene->lightManager->theLights[1].position.y = scene->sceneObjects[15]->mesh->positionXYZ.y;
        scene->lightManager->theLights[1].position.z = scene->sceneObjects[15]->mesh->positionXYZ.z;*/


        DrawSkyBox(SkySphere->mesh, program, scene->vaoManager, scene->textureManager, scene->fCamera->getCameraData());
//      DRAW LOOP
//      ------------------------------------------       
        //scene->SortObjectsForDrawing();
        //for (Object* object:scene->sceneObjectsSorted)
        //{
  
        //    sMesh* pCurMesh = object->mesh;

        //    DrawMesh(pCurMesh, program, scene->vaoManager, scene->textureManager, scene);

        //}


       DrawCameraViewToFramebufer(scene->fCamera->getCameraData(), 0,1);
       DrawCameraViewToFramebufer(scene->fCamera->getCameraData(), 0,0);
       // DrawCameraViewToFramebufer(scene->fCamera->getCameraData(), 0, 0);


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


        //TODO : GRIDS

        {
            //// Render the grid
            //glm::mat4 viewMatrix = scene->fCamera->GetViewMatrix();
            //glm::mat4 projectionMatrix = scene->fCamera->GetProjectionMatrix(16.0f / 9.0f, 45.0f);
            //glm::mat4 viewProjectionMatrix = projectionMatrix * viewMatrix;

            //// Get the camera position
            //glm::vec3 cameraPosition = scene->fCamera->getEyeLocation();

            //// Render the grid
            //gridRenderer.Render(viewProjectionMatrix, cameraPosition);
        }


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





