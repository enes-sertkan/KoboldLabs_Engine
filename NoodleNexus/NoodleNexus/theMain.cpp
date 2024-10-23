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

//
//const unsigned int MAX_NUMBER_OF_MESHES = 1000;
//unsigned int g_NumberOfMeshesToDraw;
//sMesh* g_myMeshes[MAX_NUMBER_OF_MESHES] = { 0 };    // All zeros

std::vector<sMesh*> g_vecMeshesToDraw;

cPhysics* g_pPhysicEngine = NULL;
// This loads the 3D models for drawing, etc.
cVAOManager* g_pMeshManager = NULL;

//cLightManager* g_pLightManager = NULL;

void AddModelsToScene(void);

void DrawMesh(sMesh* pCurMesh, GLuint program);

//glm::vec3 cameraEye = glm::vec3(0.0, 0.0, 4.0f);


static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

bool isControlDown(GLFWwindow* window);
//{
//    if ((glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) ||
//        (glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS))
//    {
//        return true;
//    }
//    return false;
//}

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
            //// 0 to 10
            //if (::g_selectedLightIndex < 0)
            //{
            //    ::g_selectedLightIndex = 0;
            //}

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
    }//if (mods == GLFW_KEY_LEFT_CONTROL)

//    if (key == GLFW_KEY_A)
//    {
//        cameraEye.x -= CAMERA_MOVE_SPEED;
////        g_myMeshes[::g_SelectedObjectIndex]->positionXYZ.x += CAMERA_MOVE_SPEED;
//    }
//
//    if (key == GLFW_KEY_D)
//    {
//        cameraEye.x += CAMERA_MOVE_SPEED;
////        g_myMeshes[0]->positionXYZ.x -= CAMERA_MOVE_SPEED;
//    }
//
//    if (key == GLFW_KEY_W)
//    {
//        cameraEye.z += CAMERA_MOVE_SPEED;
//    }
//
//    if (key == GLFW_KEY_S)
//    {
//        cameraEye.z -= CAMERA_MOVE_SPEED;
//    }
//
//    if (key == GLFW_KEY_Q)
//    {
//        cameraEye.y -= CAMERA_MOVE_SPEED;
//    }
//    if (key == GLFW_KEY_E)
//    {
//        cameraEye.y += CAMERA_MOVE_SPEED;
//    }


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


int main(void)
{

    //    ConsoleStuff();
    //
        // On the stack, at compile time.
        // Limited by the size of the stack.
        // Also: Can't change the size.
    //    sVertex vertices[3] =
    //    {
    //        { { -0.6f, -0.4f }, { 1.0f, 0.0f, 0.0f } },
    //        { {  0.6f, -0.4f }, { 0.0f, 1.0f, 0.0f } },
    //       { {  0.0f,  0.6f }, { 0.0f, 0.0f, 1.0f } }
    //  };
    //
    //    sizeof(sVertex) * 3;
    ////    std::cout << "vertices: " << vertices << '\n';
    //    std::cout << (*(vertices + 1)).
    //
        // On the HEAP, so dynamically allocated at run time
    //    sVertex* pVertices = new sVertex[3];
    //
    //    pVertices[0] = { { -0.6f, -0.4f }, { 1.0f, 0.0f, 0.0f } };
    //    pVertices[1] = { {  0.6f, -0.4f }, { 0.0f, 1.0f, 0.0f } };
    //    pVertices[2] = { {  0.0f,  0.6f }, { 0.0f, 0.0f, 1.0f } };
    //
        //s3DFileData plyFileInfoBunny;
        //plyFileInfoBunny.fileName = "assets/models/bun_zipper_res3.ply";
        //ReadPlyModelFromFile_xyz_ci(plyFileInfoBunny);
    //
        //s3DFileData plyFileInfo;
        //plyFileInfo.fileName = "assets/models/VintageRacingCar_xyz_only.ply";
        //ReadPlyModelFromFile_xyz(plyFileInfo);
    //
        //s3DFileData plyFileInfo;
        //plyFileInfo.fileName = "assets/models/Dragon 2.5Edited_xyz_only.ply";
        //ReadPlyModelFromFile_xyz(plyFileInfo);
    //
    // ******************************************************
    //
    //    // This is the array we are giving the GPU 
    ////    unsigned int numberOfVertices_TO_DRAW = numberOfTriangles * 3;
    //    unsigned int numberOfVertices_TO_DRAW = plyFileInfo.numberOfTriangles * 3;
    //    // Each triangle has 3 vertices
    //
    //    sVertex* pVertices = new sVertex[numberOfVertices_TO_DRAW];
    //
    //    // Copy the data form the "ply" (i.e. file) arrays
    //    // to the format that the GPU expects
    ////     struct sPlyVertex
    ////    {
    ////        float x, y, z, confidence, intensity;
    ////    };
    ////
    ////    struct sTriangle
    ////    {
    ////        unsigned int vertIndex_0;
    ////        unsigned int vertIndex_1;
    ////        unsigned int vertIndex_2;
    ////    };
    ////
    ////  to... 
    //// 
    ////    struct sVertex
    ////    {
    ////        glm::vec2 pos;      // position   or "float x, y"
    ////        glm::vec3 col;      //
    //
    //    unsigned int vertexIndex = 0;
    //
    ////    for (unsigned int triIndex = 0; triIndex != numberOfTriangles; triIndex++)
    //    for (unsigned int triIndex = 0; triIndex != plyFileInfo.numberOfTriangles; triIndex++)
    //    {
    ////        { { -0.6f, -0.4f }, { 1.0f, 0.0f, 0.0f } },
    ////        { {  0.6f, -0.4f }, { 0.0f, 1.0f, 0.0f } },
    ////        { {  0.0f,  0.6f }, { 0.0f, 0.0f, 1.0f } }
    //
    ////        pVertices[vertexIndex + 0].pos.x = pPlyVertices[ pPlyTriangles[triIndex].vertIndex_0 ].x;
    //        pVertices[vertexIndex + 0].pos.x = plyFileInfo.pPlyVertices[plyFileInfo.pPlyTriangles[triIndex].vertIndex_0 ].x;
    //        pVertices[vertexIndex + 0].pos.y = plyFileInfo.pPlyVertices[plyFileInfo.pPlyTriangles[triIndex].vertIndex_0 ].y;
    //        pVertices[vertexIndex + 0].pos.z = plyFileInfo.pPlyVertices[plyFileInfo.pPlyTriangles[triIndex].vertIndex_0 ].z;
    //        pVertices[vertexIndex + 0].col.r = 1.0f;
    //        pVertices[vertexIndex + 0].col.g = 1.0f;
    //        pVertices[vertexIndex + 0].col.b = 1.0f;
    //
    //        pVertices[vertexIndex + 1].pos.x = plyFileInfo.pPlyVertices[plyFileInfo.pPlyTriangles[triIndex].vertIndex_1 ].x;
    //        pVertices[vertexIndex + 1].pos.y = plyFileInfo.pPlyVertices[plyFileInfo.pPlyTriangles[triIndex].vertIndex_1 ].y;
    //        pVertices[vertexIndex + 1].pos.z = plyFileInfo.pPlyVertices[plyFileInfo.pPlyTriangles[triIndex].vertIndex_1 ].z;
    //        pVertices[vertexIndex + 1].col.r = 1.0f;
    //        pVertices[vertexIndex + 1].col.g = 1.0f;
    //        pVertices[vertexIndex + 1].col.b = 1.0f;
    //
    //        pVertices[vertexIndex + 2].pos.x = plyFileInfo.pPlyVertices[plyFileInfo.pPlyTriangles[triIndex].vertIndex_2 ].x;
    //        pVertices[vertexIndex + 2].pos.y = plyFileInfo.pPlyVertices[plyFileInfo.pPlyTriangles[triIndex].vertIndex_2 ].y;
    //        pVertices[vertexIndex + 2].pos.z = plyFileInfo.pPlyVertices[plyFileInfo.pPlyTriangles[triIndex].vertIndex_2 ].z;
    //        pVertices[vertexIndex + 2].col.r = 1.0f;
    //        pVertices[vertexIndex + 2].col.g = 1.0f;
    //        pVertices[vertexIndex + 2].col.b = 1.0f;
    //
    //        vertexIndex += 3;
    //    }
    //
    //
    //    // Scale the dragon
    ////    for (unsigned int index = 0; index != numberOfVertices_TO_DRAW; index++)
    ////    {
    ////        pVertices[index].pos.x *= 0.01f;
    ////        pVertices[index].pos.y *= 0.01f;
    ////        pVertices[index].pos.z *= 0.01f;
    ////    }
    //
    ////    for (unsigned int index = 0; index != numberOfVertices_TO_DRAW; index++)
    ////    {
    ////        pVertices[index].pos.x += 1.0f;
    ////    }
    //
//
//    cPhysics testPhys;
//
//    cPhysics::sLine theLine;
//    // Line goes left to right on x axis
//    theLine.startXYZ = glm::vec3(+5.0f, 0.0f, -20.0f);
//    theLine.endXYZ = glm::vec3(+5.0f, 0.0f, -20.0f);
//
//    cPhysics::sTriangle theTriangle;
//    // Triangle intersects x axis
//    theTriangle.vertices[0] = glm::vec3(0.0f, 10.0f, 0.0f);
//    theTriangle.vertices[1] = glm::vec3(0.0f, -10.0f, -10.0f);
//    theTriangle.vertices[2] = glm::vec3(0.0f, -10.0f, 10.0f);
//
//    if (testPhys.bLineSegment_TriangleCollision(theLine, theTriangle))
//    {
//        std::cout << "itersected" << std::endl;
//    }
//    else
//    {
//        std::cout << "missed" << std::endl;
//    }

    glfwSetErrorCallback(error_callback);

    if (!glfwInit())
        exit(EXIT_FAILURE);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(640, 480, "OpenGL Triangle", NULL, NULL);
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



//    GLuint vertex_buffer;
//    glGenBuffers(1, &vertex_buffer);
//    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
//
////    int size_in_bytes_of_vertex_array = sizeof(sVertex) * 3;
//    int size_in_bytes_of_vertex_array = sizeof(sVertex) * numberOfVertices_TO_DRAW;
//
//    glBufferData(GL_ARRAY_BUFFER,
//                 size_in_bytes_of_vertex_array,     // sizeof(vertices),
//                 pVertices,                         // vertices,
//                 GL_STATIC_DRAW);

    cShaderManager* pShaderManager = new cShaderManager();

    cShaderManager::cShader vertexShader;
    vertexShader.fileName = "assets/shaders/vertex01.glsl";

    cShaderManager::cShader fragmentShader;
    fragmentShader.fileName = "assets/shaders/fragment01.glsl";

    if ( ! pShaderManager->createProgramFromFile("shader01",
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

//    const GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
//    glShaderSource(vertex_shader, 1, &vertex_shader_text, NULL);
//    glCompileShader(vertex_shader);
//
//    const GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
//    glShaderSource(fragment_shader, 1, &fragment_shader_text, NULL);
//    glCompileShader(fragment_shader);
//
//    const GLuint program = glCreateProgram();
//    glAttachShader(program, vertex_shader);
//    glAttachShader(program, fragment_shader);
//    glLinkProgram(program);
//
//    const GLint mvp_location = glGetUniformLocation(program, "MVP");
//
//    const GLint vpos_location = glGetAttribLocation(program, "vPos");   
//    const GLint vcol_location = glGetAttribLocation(program, "vCol");
//
//    GLuint vertex_array;
//    glGenVertexArrays(1, &vertex_array);
//    glBindVertexArray(vertex_array);
//
//
//
//    // Where the data specifically is.
//    // Called the "vertex layout"
//
//    //glm::vec3 pos;       
//    //glm::vec3 col;    
////    { { -0.6f, -0.4f, 0.0f }, { 1.0f, 0.0f, 0.0f } },
////    { {  0.6f, -0.4f, 0.0f }, { 0.0f, 1.0f, 0.0f } },
////    { {  0.0f,  0.6f, 0.0f }, { 0.0f, 0.0f, 1.0f } }
//
//
//    glEnableVertexAttribArray(vpos_location);
//    glVertexAttribPointer(
//        vpos_location, 
//        3, 
//        GL_FLOAT, 
//        GL_FALSE,
//        sizeof(sVertex),                        // 6 floats or 24 bytes
//        (void*)offsetof(sVertex, pos));         // 0 bytes into the sVertex structure
//
//    glEnableVertexAttribArray(vcol_location);
//    glVertexAttribPointer(
//        vcol_location, 
//        3, 
//        GL_FLOAT, 
//        GL_FALSE,
//        sizeof(sVertex), 
//        (void*)offsetof(sVertex, col));     // 3 floats or 12 bytes into the sVertex structure


    // Loading the TYPES of models I can draw...

//    cVAOManager* pMeshManager = new cVAOManager();
    ::g_pMeshManager = new cVAOManager();

    //sModelDrawInfo carModelInfo;
    //pMeshManager->LoadModelIntoVAO("assets/models/VintageRacingCar_xyz_only.ply", 
    //                               carModelInfo, program);
    //std::cout << carModelInfo.numberOfVertices << " vertices loaded" << std::endl;

    //sModelDrawInfo dragonModel;
    //pMeshManager->LoadModelIntoVAO("assets/models/Dragon 2.5Edited_xyz_only.ply", 
    //    dragonModel, program);
    //std::cout << dragonModel.numberOfVertices << " vertices loaded" << std::endl;

    sModelDrawInfo terrainModel;
//    pMeshManager->LoadModelIntoVAO("assets/models/Simple_MeshLab_terrain_xyz_only.ply", 
    ::g_pMeshManager->LoadModelIntoVAO("assets/models/Simple_MeshLab_terrain_xyz_N.ply",
        terrainModel, program);
    std::cout << terrainModel.numberOfVertices << " vertices loaded" << std::endl;

    sModelDrawInfo warehouseModel;
    ::g_pMeshManager->LoadModelIntoVAO("assets/models/Warehouse_xyz_n.ply",
        warehouseModel, program);
    std::cout << warehouseModel.numberOfVertices << " vertices loaded" << std::endl;

    sModelDrawInfo bunnyModel;
//    ::g_pMeshManager->LoadModelIntoVAO("assets/models/bun_zipper_res2_10x_size_xyz_only.ply",
    ::g_pMeshManager->LoadModelIntoVAO("assets/models/bun_zipper_res2_10x_size_xyz_N_only.ply",
        bunnyModel, program);
    std::cout << bunnyModel.numberOfVertices << " vertices loaded" << std::endl;

    sModelDrawInfo platPlaneDrawInfo;
//    ::g_pMeshManager->LoadModelIntoVAO("assets/models/Flat_Plane_xyz.ply", 
    ::g_pMeshManager->LoadModelIntoVAO("assets/models/Flat_Plane_xyz_N.ply", 
        platPlaneDrawInfo, program);
    std::cout << platPlaneDrawInfo.numberOfVertices << " vertices loaded" << std::endl;
    
    sModelDrawInfo sphereMesh;
//    ::g_pMeshManager->LoadModelIntoVAO("assets/models/Sphere_radius_1_xyz.ply",
    ::g_pMeshManager->LoadModelIntoVAO("assets/models/Sphere_radius_1_xyz_N.ply",
        sphereMesh, program);
    std::cout << sphereMesh.numberOfVertices << " vertices loaded" << std::endl;

    sModelDrawInfo sphereShadowMesh;
    ::g_pMeshManager->LoadModelIntoVAO("assets/models/Sphere_radius_1_Flat_Shadow_xyz_N.ply",
        sphereShadowMesh, program);
    std::cout << sphereShadowMesh.numberOfVertices << " vertices loaded" << std::endl;

    sModelDrawInfo hangarMesh;
    ::g_pMeshManager->LoadModelIntoVAO("assets/models/Demonstration_Interior - DO NOT USE THIS xyz_N.ply",
        hangarMesh, program);
    std::cout << hangarMesh.numberOfVertices << " vertices loaded" << std::endl;

    //sModelDrawInfo backroomLevelMesh;
    //::g_pMeshManager->LoadModelIntoVAO("assets/models/Backrooms_Level_0/tinker_adjusted_xyz_N.ply",
    //    backroomLevelMesh, program);
    //std::cout << backroomLevelMesh.numberOfVertices << " vertices loaded" << std::endl;

    //    pMeshManager->LoadTheListOfModelsIWantFromASexyFile("MyModels.sexy");


    ::g_pPhysicEngine = new cPhysics();

    // For triangle meshes, let the physics object "know" about the VAO manager
    ::g_pPhysicEngine->setVAOManager(::g_pMeshManager);


    AddModelsToScene();

   
    ::g_pFlyCamera = new cBasicFlyCamera();
    ::g_pFlyCamera->setEyeLocation(glm::vec3(0.0f, 10.0f, 50.0f));
    // Rotate the camera 180 degrees
    ::g_pFlyCamera->rotateLeftRight_Yaw_NoScaling(glm::radians(180.0f));



    glUseProgram(program);

    // Enable depth buffering (z buffering)
    // https://registry.khronos.org/OpenGL-Refpages/gl4/html/glEnable.xhtml
    glEnable(GL_DEPTH_TEST);

    double currentFrameTime = glfwGetTime();
    double lastFrameTime = glfwGetTime();


    // Set up the lights
    ::g_pLightManager = new cLightManager();
    // Called only once
    ::g_pLightManager->loadUniformLocations(program);

    // Set up one of the lights in the scene
    ::g_pLightManager->theLights[0].position = glm::vec4(-15.0f, 30.0f, 0.0f, 1.0f);
    ::g_pLightManager->theLights[0].diffuse = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    ::g_pLightManager->theLights[0].atten.y = 0.01f;
    ::g_pLightManager->theLights[0].atten.z = 0.001f;

    ::g_pLightManager->theLights[0].param1.x = 0.0f;    // Point light (see shader)
    ::g_pLightManager->theLights[0].param2.x = 1.0f;    // Turn on (see shader)


    // Set up one of the lights in the scene
    ::g_pLightManager->theLights[1].position = glm::vec4(0.0f, 20.0f, 0.0f, 1.0f);
    ::g_pLightManager->theLights[1].diffuse = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    ::g_pLightManager->theLights[1].atten.y = 0.01f;
    ::g_pLightManager->theLights[1].atten.z = 0.001f;

    ::g_pLightManager->theLights[1].param1.x = 1.0f;    // Spot light (see shader)
    ::g_pLightManager->theLights[1].direction = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
    ::g_pLightManager->theLights[1].param1.y = 5.0f;   //  y = inner angle
    ::g_pLightManager->theLights[1].param1.z = 10.0f;  //  z = outer angle

    ::g_pLightManager->theLights[1].param2.x = 1.0f;    // Turn on (see shader)


    cLightHelper TheLightHelper;

    while (!glfwWindowShouldClose(window))
    {
        float ratio;
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        ratio = width / (float)height;
        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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
        //        matView = glm::lookAt( cameraEye,
        //                               cameraTarget,
        //                               upVector);


        const GLint matView_UL = glGetUniformLocation(program, "matView");
        glUniformMatrix4fv(matView_UL, 1, GL_FALSE, (const GLfloat*)&matView);

        const GLint matProjection_UL = glGetUniformLocation(program, "matProjection");
        glUniformMatrix4fv(matProjection_UL, 1, GL_FALSE, (const GLfloat*)&matProjection);

//        // *******************************************************************
//        // Place light #0 where the little yellow "light sphere" is
//        // Find the Light_Sphere
//        sMesh* pLightSphere = pFindMeshByFriendlyName("Light_Sphere");
//        // 
//        pLightSphere->positionXYZ = ::g_pLightManager->theLights[::g_selectedLightIndex].position;

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
        for (unsigned int meshIndex = 0; meshIndex != ::g_vecMeshesToDraw.size(); meshIndex++)
        {
            //            sMesh* pCurMesh = ::g_myMeshes[meshIndex];
            sMesh* pCurMesh = ::g_vecMeshesToDraw[meshIndex];

            DrawMesh(pCurMesh, program);

        }//for (unsigned int meshIndex..


        // Draw the LASER beam
        cPhysics::sLine LASERbeam;
        glm::vec3 LASERbeam_Offset = glm::vec3(0.0f, -2.0f, 0.0f);

        if (::g_bShowLASERBeam)
        {
            // Draw a bunch of little balls along a line from the camera
            //  to some place in the distance

            // The fly camera is always "looking at" something 1.0 unit away
            glm::vec3 cameraDirection = ::g_pFlyCamera->getTargetRelativeToCamera();     //0,0.1,0.9


            LASERbeam.startXYZ = ::g_pFlyCamera->getEyeLocation();

            // Move the LASER below the camera
            LASERbeam.startXYZ += LASERbeam_Offset;
            glm::vec3 LASER_ball_location = LASERbeam.startXYZ;

            // Is the LASER less than 500 units long?
            // (is the last LAZER ball we drew beyond 500 units form the camera?)
            while ( glm::distance(::g_pFlyCamera->getEyeLocation(), LASER_ball_location) < 150.0f )
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
                // Draw a sphere at the centre of the triangle
//                glm::vec3 triCentre = (itTri->vertices[0] + itTri->vertices[1] + itTri->vertices[2]) / 3.0f;
//                DrawDebugSphere(triCentre, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), 0.5f, program);

//                DrawDebugSphere(itTri->intersectionPoint, glm::vec4(1.0f, 1.0f, 0.0f, 1.0f), 0.25f, program);
                DrawDebugSphere(itTri->intersectionPoint, triColour, triangleSize, program);
                triColour.r -= 0.1f;
                triColour.g -= 0.1f;
                triColour.b += 0.2f;
                triangleSize *= 1.25f;


            }//for (std::vector<cPhysics::sTriangle>::iterator itTri = itTriList->vecTriangles

        }//for (std::vector<cPhysics::sCollision_RayTriangleInMesh>::iterator itTriList = vec_RayTriangle_Collisions


        // **********************************************************************************
        if (::g_bShowDebugSpheres)
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
        // **********************************************************************************

        //for (float x = -50.0f; x < 50.0f; x += 5.0f)
        //{
        //    for (float y = 0.0f; y < 50.0f; y += 5.0f)
        //    {
        //        for (float z = -50.0f; z < 50.0f; z += 5.0f)
        //        {
        //            DrawDebugSphere(glm::vec3(x, y, z),
        //                glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
        //                1.0f, program);

        //        }
        //    }
        //}



        // Calculate elapsed time
        // We'll enhance this
        currentFrameTime = glfwGetTime();
        double deltaTime = currentFrameTime - lastFrameTime;
        lastFrameTime = currentFrameTime;

        //sMesh* pBall = pFindMeshByFriendlyName("Ball");
        //if (pBall)
        //{
        //    pBall->positionXYZ.y -= 1.0f * deltaTime;
        //}

        // HACK: Update "shadow" of ball to be where the ball hits the large block ground
        sMesh* pBallShadow = pFindMeshByFriendlyName("Ball_Shadow");
        sMesh* pBall = pFindMeshByFriendlyName("Ball");
        pBallShadow->positionXYZ.x = pBall->positionXYZ.x;
        pBallShadow->positionXYZ.z = pBall->positionXYZ.z;
        // Don't update the y - keep the shadow near the plane


        // Physic update and test 
        ::g_pPhysicEngine->StepTick(deltaTime);


        // Handle any collisions
        if (::g_pPhysicEngine->vec_SphereAABB_Collisions.size() > 0 )
        {
            // Yes, there were collisions

            for (unsigned int index = 0; index != ::g_pPhysicEngine->vec_SphereAABB_Collisions.size(); index++)
            {
                cPhysics::sCollision_SphereAABB thisCollisionEvent = ::g_pPhysicEngine->vec_SphereAABB_Collisions[index];

                if (thisCollisionEvent.pTheSphere->pPhysicInfo->velocity.y  < 0.0f)
                {
                    // Yes, it's heading down
                    // So reverse the direction of velocity
                    thisCollisionEvent.pTheSphere->pPhysicInfo->velocity.y = fabs(thisCollisionEvent.pTheSphere->pPhysicInfo->velocity.y);
                }

            }//for (unsigned int index
 
        }//if (::g_pPhysicEngine->vec_SphereAABB_Collisions


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


    }// End of the draw loop


    // Delete everything
    delete ::g_pFlyCamera;
    delete ::g_pPhysicEngine;

    glfwDestroyWindow(window);

    glfwTerminate();
    exit(EXIT_SUCCESS);
}


void AddModelsToScene(void)
{

    // Load some models to draw

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
//    {
//        sMesh* pSphereMesh = new sMesh();
////        pSphereMesh->modelFileName = "assets/models/Sphere_radius_1_xyz.ply";
//        pSphereMesh->modelFileName = "assets/models/Sphere_radius_1_xyz_N.ply";
//        pSphereMesh->positionXYZ = glm::vec3(0.0f, 30.0f, 0.0f);
//        pSphereMesh->bIsWireframe = true;
//        pSphereMesh->objectColourRGBA = glm::vec4(1.0f, 1.0f, 0.0f, 1.0f);
//        pSphereMesh->uniformScale = 0.1f;
//        pSphereMesh->uniqueFriendlyName = "Light_Sphere";
//
//        ::g_vecMeshesToDraw.push_back(pSphereMesh);
//    }

    //{
    //    sMesh* pDebugSphere = new sMesh();
    //    pDebugSphere->modelFileName = "assets/models/Sphere_radius_1_xyz_N.ply";
    //    pDebugSphere->positionXYZ = glm::vec3(0.0f, 5.0f, 0.0f);
    //    pDebugSphere->bIsWireframe = true;
    //    pDebugSphere->objectColourRGBA = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
    //    pDebugSphere->uniqueFriendlyName = "Debug_Sphere";
    //    pDebugSphere->uniformScale = 10.0f;
    //    pDebugSphere->bDoNotLight = true;
    //    ::g_vecMeshesToDraw.push_back(pDebugSphere);
    //}

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


    //{
    //    sMesh* pBackrooms = new sMesh();
    //    pBackrooms->modelFileName = "assets/models/Backrooms_Level_0/tinker_adjusted_xyz_N.ply";
    //    pBackrooms->objectColourRGBA = glm::vec4(::g_rgb_from_HTML(240, 230, 140), 1.0f);
    //    pBackrooms->bIsWireframe = true;
    //    pBackrooms->bDoNotLight = true;
    //    pBackrooms->bOverrideObjectColour = true;
    //    pBackrooms->uniqueFriendlyName = "Backrooms";
    //     ::g_vecMeshesToDraw.push_back(pBackrooms);
    //}

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
    //    //                                                                
    //    sMesh* pWarehouse = new sMesh();
    //    pWarehouse->modelFileName = "assets/models/Warehouse_xyz_n.ply";
    //    pWarehouse->positionXYZ = glm::vec3(100.0f, -5.0f, 0.0f);
    //    pWarehouse->rotationEulerXYZ.y = -90.0f;
    //    pWarehouse->objectColourRGBA = glm::vec4(0.6f, 0.6f, 0.6f, 1.0f);
    //    pWarehouse->uniqueFriendlyName = "Warehouse";
    //    ::g_vecMeshesToDraw.push_back(pWarehouse);

    //    //    ____  _               _                  _     _           _   
    //    //   |  _ \| |__  _   _ ___(_) ___ ___    ___ | |__ (_) ___  ___| |_ 
    //    //   | |_) | '_ \| | | / __| |/ __/ __|  / _ \| '_ \| |/ _ \/ __| __|
    //    //   |  __/| | | | |_| \__ \ | (__\__ \ | (_) | |_) | |  __/ (__| |_ 
    //    //   |_|   |_| |_|\__, |___/_|\___|___/  \___/|_.__// |\___|\___|\__|
    //    //                |___/                           |__/               
    //    ::g_pPhysicEngine->addTriangleMesh(
    //        "assets/models/Warehouse_xyz_n.ply",
    //        pWarehouse->positionXYZ,
    //        pWarehouse->rotationEulerXYZ,
    //        pWarehouse->uniformScale);

    //}


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

    //    sMesh* pDragon = new sMesh();
    //    pDragon->modelFileName = "assets/models/Dragon 2.5Edited_xyz_only.ply";
    //    pDragon->positionXYZ = glm::vec3(20.0f, 0.0f, 0.0f);
    //    pDragon->rotationEulerXYZ.x = -90.0f;
    //    pDragon->uniformScale = 0.1f;
    //    pDragon->objectColourRGBA = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f); 
    //
    //    ::g_myMeshes[0] = pDragon;
    //
    //
    //    sMesh* pDragon2 = new sMesh();
    //    pDragon2->modelFileName = "assets/models/Dragon 2.5Edited_xyz_only.ply";
    //    pDragon2->positionXYZ = glm::vec3(-20.0f, 0.0f, 0.0f);
    //    pDragon2->rotationEulerXYZ.x = 90.0f;
    //    pDragon2->objectColourRGBA = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
    //    pDragon2->uniformScale = 0.2f;
    //
    //
    //    ::g_myMeshes[1] = pDragon2;
    //
    //    ::g_NumberOfMeshesToDraw = 2;
    //
    //
    //    sMesh* pTerrainMesh = new sMesh();
    //    pTerrainMesh->modelFileName = "assets/models/Simple_MeshLab_terrain_xyz_only.ply";
    //    pTerrainMesh->positionXYZ = glm::vec3(0.0f, -25.0f, 0.0f);
    //    //pTerrainMesh->rotationEulerXYZ.x = 90.0f;
    //    pTerrainMesh->objectColourRGBA = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    //    pTerrainMesh->bOverrideObjectColour = true;
    ////    pTerrainMesh->bIsWireframe = true;
    //    ::g_myMeshes[::g_NumberOfMeshesToDraw] = pTerrainMesh;
    //    ::g_NumberOfMeshesToDraw++;
    //
    //
    //    for (int count = 0; count != 100; count++)
    //    {
    //        sMesh* pDragon = new sMesh();
    //        pDragon->modelFileName = "assets/models/VintageRacingCar_xyz_only.ply";
    //        pDragon->positionXYZ = glm::vec3(getRandomFloat(-5.0f, 5.0f),
    //                                         getRandomFloat(-5.0f, 5.0f),
    //                                         getRandomFloat(-5.0f, 5.0f));
    //        pDragon->rotationEulerXYZ.x = 90.0f;
    //        pDragon->objectColourRGBA
    //            = glm::vec4(getRandomFloat(0.0f, 1.0f),
    //                        getRandomFloat(0.0f, 1.0f),
    //                        getRandomFloat(0.0f, 1.0f),
    //                        1.0f);
    //
    //
    //        pDragon->uniformScale = 0.2f;
    //
    //        // This is evil, nasty code. 
    //        // Do this if you hate humanity...
    ////        pDragon->bIsWireframe = rand() % 2;
    //
    //        ::g_myMeshes[::g_NumberOfMeshesToDraw] = pDragon;
    //
    //        ::g_NumberOfMeshesToDraw++;
    //    }



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