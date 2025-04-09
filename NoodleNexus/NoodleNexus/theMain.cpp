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
#include "aScreenTextureSwitch.h"
#include "aSoftBodyAction.hpp"
#include "aBaseMazeCharacter.h"
#include "aMainCamera.hpp"
#include "aRotate.h"
#include "aConnectSoftBodToObj.hpp"
#include "aRotationWithMinutes.hpp"
#include "aWavesEffect.h"
#include "aMirrorReflection.h"
#include "BruteEnemy.h"
#include "LabAttackFactory.h"
#include "aPlayerShooting.h"
#include "aGrassCollider .h"
#include "aTurretPlacer.h"
#include "aToolManager.h"


// Core MGUI headers
#include "imgui/imgui.h"          // Main MGUI header
#include "imgui/imconfig.h"          // Main MGUI header
#include "imgui/imgui_impl_glfw.h" // GLFW integration (if required)
#include "imgui/imgui_impl_opengl3.h" // OpenGL 3+ integration

#include "aParticleEmitter .h"
#include "player.h"
#include "player_ui.h"

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
void DrawSkyBox(Object* object, GLuint program, cVAOManager* vaoManager, cBasicTextureManager* textureManager, Camera* camera);




KLFileManager* fileMangerImgui = new KLFileManager();
bool showExitPopup = false;
bool isSceneSaved = true;
std::string lastSavedData;


void SetupDearImGui(GLFWwindow* window)
{
    // IMPORTANT: Must be called AFTER GL context is created
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();

    // Match your EXACT OpenGL version
    const char* glsl_version = "#version 420";  // Match your 4.2 context
    if (!ImGui_ImplGlfw_InitForOpenGL(window, true))
    {
        std::cerr << "Failed to initialize GLFW backend!" << std::endl;
        return;
    }

    if (!ImGui_ImplOpenGL3_Init(glsl_version))
    {
        std::cerr << "Failed to initialize OpenGL3 backend!" << std::endl;
        return;
    }

    ImGui::StyleColorsDark();
}


void RenderObjectNode(Object* obj, SceneEditor* sceneEditor, const ImGuiTextFilter& filter) {
    if (!filter.PassFilter(obj->name.c_str())) return;

    ImGui::PushID(obj);

    ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow |
        ImGuiTreeNodeFlags_SpanAvailWidth;

    flags |= obj->m_children.empty() ? ImGuiTreeNodeFlags_Leaf : 0;
    flags |= (sceneEditor->selectedObject == obj) ? ImGuiTreeNodeFlags_Selected : 0;

    bool isOpen = ImGui::TreeNodeEx(obj->name.c_str(), flags);

    // Handle selection
    if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen()) {
        sceneEditor->selectedObject = obj;
    }

    // Context menu for parenting
    if (ImGui::BeginPopupContextItem()) {
        if (ImGui::MenuItem("Make Child")) {
            if (sceneEditor->selectedObject &&
                sceneEditor->selectedObject != obj) {
                obj->AddChild(sceneEditor->selectedObject);
            }
        }

        if (ImGui::MenuItem("Remove Parent")) {
             
            obj->RemoveParent();
            
        }
        ImGui::EndPopup();
    }

    // Recursive render using PROPER child list
    if (isOpen) {
        for (Object* child : obj->m_children) { // Use m_children instead of sceneObjects
            RenderObjectNode(child, sceneEditor, filter);
        }
        ImGui::TreePop();
    }

    ImGui::PopID();
}

void SceneHierarchyExample(SceneEditor* sceneEditor) {
    Scene* scene = sceneEditor->scene;
    static ImGuiTextFilter filter;

    ImGui::Begin("Scene Hierarchy");

    filter.Draw("Search...");
    ImGui::SameLine();
    if (ImGui::Button("Clear")) filter.Clear();
    ImGui::Separator();

    /* PHASE 1: Find all child objects using PROPER child list */
    std::unordered_set<Object*> allChildren;
    for (Object* obj : scene->sceneObjects) {
        for (Object* child : obj->m_children) { // Use m_children here
            allChildren.insert(child);
        }
    }

    /* PHASE 2: Draw hierarchy roots */
    for (Object* obj : scene->sceneObjects) {
        if (allChildren.find(obj) == allChildren.end()) {
            RenderObjectNode(obj, sceneEditor, filter);
        }
    }

    ImGui::End();
}


void ObjectPropertiesExample(Object* selectedObject)
{
    if (!selectedObject || !selectedObject->mesh) return;

    // Temporary copy with sanitized values
    glm::vec3 safePos = selectedObject->mesh->positionXYZ;

    // Clamp extremely large values before editing
    if (glm::length(safePos) > 1000.0f) {
        safePos = glm::vec3(0.0f);
    }



    ImGui::Begin("Mesh Properties");

    // Basic Transform Controls
    if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGui::DragFloat3("Position",
            glm::value_ptr(selectedObject->startTranform->position),
            0.05f);


        ImGui::DragFloat3("Rotation",
            glm::value_ptr(selectedObject->startTranform->rotation),
            0.5f);
    

        ImGui::DragFloat("Scale",
            &selectedObject->startTranform->scale.x,
            0.1f, 0.01f, 100.0f);  // Advanced controls here


        if (ImGui::CollapsingHeader("Current Position"))
        {
            // Position
            ImGui::DragFloat3("Position",
                glm::value_ptr(selectedObject->mesh->positionXYZ),
                0.1f);

            // Rotation (convert radians to degrees for display)
            glm::vec3 rotationDegrees = glm::degrees(
                selectedObject->mesh->rotationEulerXYZ
            );

            if (ImGui::DragFloat3("Rotation",
                glm::value_ptr(rotationDegrees),
                1.0f))
            {
                selectedObject->mesh->rotationEulerXYZ =
                    glm::radians(rotationDegrees);
            }

            // Scale
            ImGui::DragFloat("Scale",
                &selectedObject->mesh->uniformScale,
                0.1f, 0.01f, 100.0f);  // Advanced controls here
        }


    }

    // Material Properties
    if (ImGui::CollapsingHeader("Material Settings"))
    {
        ImGui::ColorEdit4("Object Color", glm::value_ptr(selectedObject->mesh->objectColourRGBA));
        ImGui::Checkbox("Override Color", &selectedObject->mesh->bOverrideObjectColour);
        ImGui::DragFloat("Transparency", &selectedObject->mesh->transperency, 0.01f, 0.0f, 1.0f);
        ImGui::DragFloat("Metalness", &selectedObject->mesh->metal, 0.01f, 0.0f, 1.0f);
        ImGui::DragFloat("Smoothness", &selectedObject->mesh->smoothness, 0.01f, 0.0f, 1.0f);
    }

    // Texture Management
    if (ImGui::CollapsingHeader("Textures"))
    {
        for (int i = 0; i < sMesh::MAX_NUM_TEXTURES; i++)
        {
            ImGui::PushID(i);
            ImGui::Text("Texture Slot %d", i);
           // ImGui::InputText("Path", selectedObject->mesh->textures[i]);
            ImGui::DragFloat("Blend Ratio", &selectedObject->mesh->blendRatio[i], 0.01f, 0.0f, 1.0f);
            ImGui::Combo("Fill Type", &selectedObject->mesh->textureFillType[i], "Repeat\0Clamp\0Mirror\0");
            ImGui::Separator();
            ImGui::PopID();
        }
    }

    // Shell Texturing (STData)
    if (ImGui::CollapsingHeader("Shell Texturing"))
    {
        sSTData& st = selectedObject->mesh->stData;
        ImGui::Checkbox("Enable Shell Texturing", &selectedObject->mesh->shellTexturing);

        ImGui::DragInt("Shell Count", &st.shellCount, 1, 1, 1000);
        ImGui::DragFloat("Vertical Tightening", &st.verticalTightening, 0.01f, 0.0f, 1.0f);
        ImGui::DragFloat("Vertical Exponent", &st.verticalExponent, 0.01f, 0.0f, 5.0f);
        ImGui::DragFloat("Shell Length", &st.shellLength, 0.01f, 0.0f, 2.0f);
    }

    // Collider Management
    if (ImGui::CollapsingHeader("Colliders"))
    {
        // Add new collider
        if (ImGui::Button("Add Collider"))
        {
            int newIndex = selectedObject->mesh->CreateCollider(
                selectedObject->mesh->positionXYZ
            );
            if (newIndex == -1)
                ImGui::TextColored(ImVec4(1, 0, 0, 1), "No free collider slots!");
        }

        // List existing colliders
        for (int i = 0; i < 20; i++)
        {
            sSTCollider& col = selectedObject->mesh->stColliders[i];
            if (!col.isOn) continue;

            ImGui::PushID(i);
            if (ImGui::TreeNodeEx(("Collider " + std::to_string(i)).c_str(), ImGuiTreeNodeFlags_DefaultOpen))
            {
                ImGui::DragFloat3("Position", glm::value_ptr(col.position), 0.1f);
                ImGui::DragFloat("Radius", &col.radius, 0.01f, 0.01f, 10.0f);
                ImGui::DragFloat("Blend Radius", &col.blendingRadius, 0.01f, 0.0f, 10.0f);

                if (ImGui::Button("Remove"))
                {
                    selectedObject->mesh->RemoveCollider(i);
                    ImGui::TreePop();
                    ImGui::PopID();
                    break;
                }

                ImGui::TreePop();
            }
            ImGui::PopID();
        }
    }

    // Special Effects
    if (ImGui::CollapsingHeader("Special Effects"))
    {
        ImGui::DragFloat("Zoom Power", &selectedObject->mesh->zoomPower, 0.01f, 0.0f, 5.0f);
        ImGui::DragFloat("Chromatic Power", &selectedObject->mesh->chromaticPower, 0.01f, 0.0f, 2.0f);
        ImGui::Checkbox("Draw Both Faces", &selectedObject->mesh->drawBothFaces);
    }

    ImGui::End();
}


//IMGUI HELL
void SaveSceneImgui(SceneEditor* sceneEditor, const std::string& name) {
    if (!sceneEditor || !sceneEditor->fileManger || !sceneEditor->scene) {
        std::cerr << "Error: Invalid scene, editor, or file manager!" << std::endl;
        return;
    }

    sceneEditor->fileManger->WriteSceneFile(sceneEditor->scene, name);
    lastSavedData = sceneEditor->fileManger->SerializeSceneToString(sceneEditor->scene);
    isSceneSaved = true;
}

bool HasUnsavedChanges(Scene* scene) {
    if (lastSavedData.empty()) return true; // Assume unsaved if no data stored
    std::string currentData = fileMangerImgui->SerializeSceneToString(scene);
    return (currentData != lastSavedData);
}

void CheckForExit(SceneEditor* sceneEditor) {
    if (glfwWindowShouldClose(sceneEditor->window)) {
        if (HasUnsavedChanges(sceneEditor->scene)) {
            showExitPopup = true;
            glfwSetWindowShouldClose(sceneEditor->window, GLFW_FALSE);
        }
    }
}

void CheckIfSceneClosed(SceneEditor* sceneEditor) {
    // Check for GLFW close request
    if (glfwWindowShouldClose(sceneEditor->window))
    {
        if (isSceneSaved)
        {
            // If saved, allow the window to close
            return;
        }
        else
        {
            // If not saved, show popup and cancel the close request
            showExitPopup = true;
            glfwSetWindowShouldClose(sceneEditor->window, false);
        }
    }
}

void SaveSceneButton(SceneEditor* sceneEditor) {
    ImGui::Begin("Scene Controls");

    if (ImGui::Button("Save Scene")) // Save Scene button
    {
        SaveSceneImgui(sceneEditor, sceneEditor->currentFilename); // Use the actual scene
    }

    if (ImGui::Button("Exit")) {
        if (HasUnsavedChanges(sceneEditor->scene)) {
            showExitPopup = true;
        }
        else {
            glfwSetWindowShouldClose(sceneEditor->window, true);
        }
    }
}

void ExitButtonWithPopUp(SceneEditor* sceneEditor) {
    // Show confirmation popup if needed
    if (showExitPopup)
    {
        ImGui::OpenPopup("Exit Confirmation");
    }

    if (ImGui::BeginPopupModal("Exit Confirmation", NULL, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::Text("Are you sure you want to exit without saving?");
        ImGui::Separator();

        if (ImGui::Button("Yes, Exit"))
        {
            glfwSetWindowShouldClose(sceneEditor->window, true); // Close the window
            ImGui::CloseCurrentPopup();
            showExitPopup = false;
        }
        ImGui::SameLine();
        if (ImGui::Button("Cancel"))
        {
            showExitPopup = false;
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }
}



void ShowSaveAsPopup(SceneEditor* sceneEditor) {
    if (sceneEditor->showSaveAsPopup) {
        ImGui::OpenPopup("Save As");
    }

    // Always center this popup
    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

    if (ImGui::BeginPopupModal("Save As", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
        // Text input for filename
        ImGui::InputText("Filename", sceneEditor->newFilenameBuffer, IM_ARRAYSIZE(sceneEditor->newFilenameBuffer));

        // Save button
        if (ImGui::Button("Save")) {
            sceneEditor->currentFilename = sceneEditor->newFilenameBuffer;
            SaveSceneImgui(sceneEditor, sceneEditor->currentFilename);
            sceneEditor->showSaveAsPopup = false;
            ImGui::CloseCurrentPopup();
        }

        ImGui::SameLine();

        // Cancel button
        if (ImGui::Button("Cancel")) {
            sceneEditor->showSaveAsPopup = false;
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }
}

// ---------------------------
// Implement Load Scene later
// ---------------------------

//void LoadSceneImgui(SceneEditor* sceneEditor, const std::string& filename) {
//    if (!sceneEditor || !sceneEditor->fileManger) {
//        std::cerr << "Error: Invalid scene or file manager!" << std::endl;
//        return;
//    }
//
//    Scene* newScene = sceneEditor->fileManger->ReadSceneFile(filename);
//    if (newScene) {
//        delete sceneEditor->scene;       // Clean up old scene
//        sceneEditor->scene = newScene;
//        sceneEditor->currentFilename = filename;
//        std::cout << "Scene loaded: " << filename << std::endl;
//    }
//    else {
//        std::cerr << "Failed to load scene: " << filename << std::endl;
//    }
//}
//
//void ShowLoadScenePopup(SceneEditor* sceneEditor) {
//    if (sceneEditor->showLoadScenePopup) {
//        ImGui::OpenPopup("Load Scene");
//    }
//
//    // Center the popup
//    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
//    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
//
//    if (ImGui::BeginPopupModal("Load Scene", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
//        // Text input for filename
//        ImGui::InputText("Scene File", sceneEditor->loadFilenameBuffer, IM_ARRAYSIZE(sceneEditor->loadFilenameBuffer));
//
//        // Load button
//        if (ImGui::Button("Load")) {
//            LoadSceneImgui(sceneEditor, sceneEditor->loadFilenameBuffer);
//            sceneEditor->showLoadScenePopup = false;
//            ImGui::CloseCurrentPopup();
//        }
//
//        ImGui::SameLine();
//
//        // Cancel button
//        if (ImGui::Button("Cancel")) {
//            sceneEditor->showLoadScenePopup = false;
//            ImGui::CloseCurrentPopup();
//        }
//
//        ImGui::EndPopup();
//    }
//}

const char* GetBodyName(eTurretBodyID id) {
    switch (id) {
    case STANDARTBODY: return "Standard Body";
        // Add new body types here as you create them
    default: return "Unknown Body";
    }
}

const char* GetNeckName(eTurretNeckID id) {
    switch (id) {
    case STANDARTNECK: return "Standard Neck";
    case AIMNECK: return "Auto Aim Neck";
        // Add new neck types here
    default: return "Unknown Neck";
    }
}

const char* GetHeadName(eTurretHeadID id) {
    switch (id) {
    case STANDARTHEAD: return "Standard Head";
        // Add new head types here
    default: return "Unknown Head";
    }
}

void TurretSpawnerWindow(LabAttackFactory* factory, SceneEditor* sceneEditor) {
    
    static eTurretBodyID selectedBodyID = STANDARTBODY;
    static eTurretNeckID selectedNeckID = STANDARTNECK;
    static eTurretHeadID selectedHeadID = STANDARTHEAD;

    ImGui::Begin("Turret Spawner");

    // Position controls
    ImGui::DragFloat3("Spawn Position", glm::value_ptr(sceneEditor->ghostTurret->body->object->mesh->positionXYZ), 0.1f);

    // Body selection - automatically shows all available bodies
    if (ImGui::BeginCombo("Body Type", GetBodyName(selectedBodyID))) {
        for (cTurretBody* body : factory->turretBodies) {
            const bool isSelected = (selectedBodyID == body->ID);
            if (ImGui::Selectable(GetBodyName(body->ID), isSelected)) {
                selectedBodyID = body->ID;
                sTurretCofig* config = sceneEditor->ghostTurret->GetConfig();

                config->bodyID = selectedBodyID;
                sceneEditor->ghostTurret->RebuildTurretGhost(config);
            }
            if (isSelected) {
                ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndCombo();
    }

    // Neck selection - automatically shows all available necks
    if (ImGui::BeginCombo("Neck Type", GetNeckName(selectedNeckID))) {
        for (cTurretNeck* neck : factory->turretNecks) {
            const bool isSelected = (selectedNeckID == neck->ID);
            if (ImGui::Selectable(GetNeckName(neck->ID), isSelected)) {
                selectedNeckID = neck->ID;

                sTurretCofig* config = sceneEditor->ghostTurret->GetConfig();

                config->neckID = selectedNeckID;
                sceneEditor->ghostTurret->RebuildTurretGhost(config);
            }
            if (isSelected) {
                ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndCombo();
    }

    // Head selection - automatically shows all available heads
    if (ImGui::BeginCombo("Head Type", GetHeadName(selectedHeadID))) {
        for (cTurretHead* head : factory->turretHeads) {
            const bool isSelected = (selectedHeadID == head->ID);
            if (ImGui::Selectable(GetHeadName(head->ID), isSelected)) {

                sTurretCofig* config = sceneEditor->ghostTurret->GetConfig();

                config->headID = selectedHeadID;
                sceneEditor->ghostTurret->RebuildTurretGhost(config);
                selectedHeadID = head->ID;
                
            }
            if (isSelected) {
                ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndCombo();
    }

    if (ImGui::Button("Spawn Turret")) {
        sTurretCofig config{
            selectedBodyID,
            selectedNeckID,
            selectedHeadID
        };

        if (factory->SpawnTurret(sceneEditor->ghostTurret->body->object->mesh->positionXYZ, config.bodyID, config.neckID, config.headID)) {
            std::cout << "Spawned turret with config: "
                << GetBodyName(config.bodyID) << ", "
                << GetNeckName(config.neckID) << ", "
                << GetHeadName(config.headID) << std::endl;
        }
    }

    ImGui::End();
}

void RenderDearImGui(SceneEditor* sceneEditor, LabAttackFactory* factory)
{
 

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();



        if (sceneEditor->scene->isFlyCamera)
        {
        // Main menu bar with "Save As" option
        if (ImGui::BeginMainMenuBar()) {
            if (ImGui::BeginMenu("File")) {
                if (ImGui::MenuItem("Save")) {
                    SaveSceneImgui(sceneEditor, sceneEditor->currentFilename);
                }
                if (ImGui::MenuItem("Save As...")) {
                    sceneEditor->showSaveAsPopup = true;
                }
                //if (ImGui::MenuItem("Load Scene...")) {
                //    sceneEditor->showLoadScenePopup = true;
                //}
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }


        CheckIfSceneClosed(sceneEditor);

        // Your GUI components
        SceneHierarchyExample(sceneEditor);
        ObjectPropertiesExample(sceneEditor->selectedObject);
        TurretSpawnerWindow(factory, sceneEditor);

        SaveSceneButton(sceneEditor);
        ShowSaveAsPopup(sceneEditor);
        //ShowLoadScenePopup(sceneEditor);

       

        }
        else
        {

            //PLAY MODE UI
            PlayerUI::RenderHPBar(40, 100);
        }





        ImGui::End();


        ExitButtonWithPopUp(sceneEditor);

        // Render ImGui
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        return;
}

//IMGUI HELL END

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

GLuint LoadShaderProgram(
    const std::string& shaderName,
    const std::string& vertexPath,
    const std::string& fragmentPath,
    std::string& errorOut
) {
    cShaderManager::cShader vertShader;
    vertShader.fileName = vertexPath;

    cShaderManager::cShader fragShader;
    fragShader.fileName = fragmentPath;
    cShaderManager* pShaderManager = new cShaderManager();
    if (!pShaderManager->createProgramFromFile(shaderName, vertShader, fragShader)) {
        errorOut = pShaderManager->getLastError();
        return 0; // Indicate failure
    }

    return pShaderManager->getIDFromFriendlyName(shaderName);
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
        << ::g_pFlyCamera->getCameraData()->position.x << ", "
        << ::g_pFlyCamera->getCameraData()->position.y << ", "
        << ::g_pFlyCamera->getCameraData()->position.z
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


void SpawnEffectInTudeOnPos(Scene* scene, glm::vec3 pos)
{

    Object* slime = scene->GenerateMeshObjectsFromObject("assets/models/Rope.ply",pos, 0.09, glm::vec3(0.f, 0.f, 0.f), true, glm::vec4(0.f, 1.f, 0.f, 1.f), false, scene->sceneObjects);
    SoftBody* slimeBody = new SoftBody();

    slime->isTemporary = true;
    slime->mesh->metal = 0.1f;
    slime->mesh->smoothness = 0.1f;
    slimeBody->acceleration.y = -16;
    slime->mesh->drawBothFaces = true;
    slimeBody->constIterations = 2;
    //slimeBody->sbCollision->collisionMult = 2.f;
    slimeBody->tighness = 2.f;
    //softObject->mesh->NMTexture = "Wall_Simple_Normal.bmp";
    // . . . . . . . . 
    slimeBody->useVolume = true;
    slimeBody->easyControl = true;
    slimeBody->inCylynder = true;
    slimeBody->cykinderRadious = 0.1f;
   scene->AddActionToObj(slimeBody, slime);
}
void SpawnSlimeInTudeOnPos(Scene* scene, glm::vec3 pos)
{

    Object* slime = scene->GenerateMeshObjectsFromObject("assets/models/Sphere_radius_1_xyz_N_uv.ply", pos, 0.2, glm::vec3(0.f, 0.f, 0.f), true, glm::vec4(0.f, 1.f, 0.f, 1.f), false, scene->sceneObjects);
    SoftBody* slimeBody = new SoftBody();

    slime->isTemporary = true;
    slime->mesh->metal = 0.1f;
    slime->mesh->smoothness = 0.1f;
    slimeBody->acceleration.y = -16;
    slime->mesh->drawBothFaces = true;
    slimeBody->constIterations = 2;
    //slimeBody->sbCollision->collisionMult = 2.f;
    slimeBody->tighness = 2.f;
    //softObject->mesh->NMTexture = "Wall_Simple_Normal.bmp";
    // . . . . . . . . 
    slimeBody->useVolume = true;
    slimeBody->easyControl = true;
    slimeBody->inCylynder = true;
    scene->AddActionToObj(slimeBody, slime);
}


Object* screen_quad = nullptr;

void AddActions(Scene* scene, Scene* sceneCam, Scene* securityRoomScene,  GLuint program, LabAttackFactory** factory)
{
    MazeGenerator* mazeGenerator = new MazeGenerator("assets/models/maze.txt", scene, scene->lightManager);
    MazeGenerator* mazeSecurity = new MazeGenerator("assets/models/mazeSecurity.txt", securityRoomScene, securityRoomScene->lightManager);
    LabAttackFactory* LAFactory = new LabAttackFactory();
    *factory = LAFactory;
    LAFactory->scene = scene;
    LAFactory->Start();
    LAFactory->maze = mazeGenerator;
    mazeGenerator->factory = LAFactory;
    screen_quad = sceneCam->GenerateMeshObjectsFromObject("assets/models/screen_quad.ply", glm::vec3(0.f, 0.f, 0.f), 6.5, glm::vec3(0.f), false, glm::vec4(0.f, 1.f, 0.f, 1.f), false, sceneCam->sceneObjects);
    screen_quad->mesh->textures[0] = "main_camera";
    //screen_quad->mesh->chromaticPower = 0.1;
    screen_quad->mesh->blendRatio[0] = 1.f;
    screen_quad->mesh->chromaticPower = 0.0007f;
  //  obj->mesh->textures[1] = "screen_broken.bmp";
  //  obj->mesh->blendRatio[1] = 0.2f;
    MainCamera* mainCamera = new MainCamera(); 
    
    Object* wierd = securityRoomScene->GenerateMeshObjectsFromObject("assets/models/objects/frog.ply", glm::vec3(5.f, 0.f, 0.f), 1, glm::vec3(0.f),true, glm::vec4(0.f, 1.f, 0.f, 1.f), true, securityRoomScene->sceneObjects);
    wierd->mesh->textures[0] = "frog_diffuse.bmp";
    wierd->mesh->blendRatio[0] = 1.0f;
    aRotate* rotateAction = new aRotate();
    securityRoomScene->AddActionToObj(rotateAction, wierd);

    Object* tube = securityRoomScene->GenerateMeshObjectsFromObject("assets/models/objects/frog.ply", glm::vec3(250.f, 13.f, 113.f), 10, glm::vec3(0.f), true, glm::vec4(0.f, 0.5f, 0.f, 1.f), true, securityRoomScene->sceneObjects);
    tube->mesh->textures[0] = "frog_diffuse.bmp";
    tube->mesh->blendRatio[0] = 1.0f;
    aRotate* rotateActionTube = new aRotate();
    securityRoomScene->AddActionToObj(rotateActionTube, wierd);

    Object* securutyCamera = securityRoomScene->GenerateMeshObjectsFromObject("assets/models/Cube_xyz_n_uv.ply", glm::vec3(16.f, 3.0f, 0.f), 3, glm::vec3(0.f, 179.07f, 0.f), false, glm::vec4(0.f, 1.f, 0.f, 1.f), true, securityRoomScene->sceneObjects);
    Object* particles = scene->GenerateMeshObjectsFromObject("assets/models/Cube_xyz_n_uv.ply", glm::vec3(16.f, 3.0f, 0.f), 1, glm::vec3(0.f, 0.f, 0.f), true, glm::vec4(0.f, 1.f, 0.f, 1.f), true, scene->sceneObjects);
    securutyCamera->mesh->textures[0] = "screen_broken.bmp";
    securutyCamera->mesh->blendRatio[0] = 1.0f;
    particles->name = "Particles";
  //  particles->mesh->drawBothFaces=true;
   // particles->mesh->transperency = 0.2;

    aParticleEmitter* particleEmmiter = new aParticleEmitter();
    scene->AddActionToObj(particleEmmiter, particles);

    Object* securutyCamera2 = securityRoomScene->GenerateMeshObjectsFromObject("assets/models/Cube_xyz_n_uv.ply", glm::vec3(41.f, 40.0f, 20.f), 10, glm::vec3(0.f, 0.5f, 0.f), false, glm::vec4(0.f, 1.f, 0.f, 1.f), true, securityRoomScene->sceneObjects);
    securutyCamera2->mesh->textures[0] = "screen_broken.bmp";
    securutyCamera2->mesh->blendRatio[0] = 1.0f;



    Object* puddle = scene->GenerateMeshObjectsFromObject("assets/models/plene_1x1.ply", glm::vec3(50.f,3.2f,30.f),10.f, glm::vec3(0.f, 0.f, 0.f), false, glm::vec4(0.f, 1.f, 0.f, 1.f), true, scene->sceneObjects);
    Object* underpuddle = scene->GenerateMeshObjectsFromObject("assets/models/plene_1x1.ply", glm::vec3(50.f,3.25f,30.f),10.f, glm::vec3(0.f, 0.f, 0.f), true , glm::vec4(0.001f, 0.01f, 0.001f, 1.f), false, scene->sceneObjects);
   
 LAFactory->grass = puddle;
    puddle->name = "GRASS";
    underpuddle->name = "UNDERGRASS";
    underpuddle->isTemporary = true;
    puddle->AddChild(underpuddle);
    puddle->mesh->textures[0] = "screen_broken.bmp";
    puddle->mesh->blendRatio[0] = 1.0f;
    puddle->mesh->shellTexturing = true;
    puddle->isActive = true;
    puddle->mesh->smoothness = 0.99f;
    puddle->mesh->metal = 0.99f;

   // puddle->mesh->NMTexture = "Wall_Simple_Normal.bmp";
    aWavesEffect* waveEffect = new aWavesEffect();
   // waveEffect->offset = glm::vec2(0.5, 0.5);
    scene->AddActionToObj(waveEffect, puddle);
    puddle->isTemporary = true;
    
    Object* player = scene->GenerateMeshObjectsFromObject("", glm::vec3(20.f, 5.f, 7.f), 4, glm::vec3(0.f, 0.f, 0.f), false, glm::vec4(0.f, 1.f, 0.f, 1.f), false, scene->sceneObjects);
    player->name = "PLAYER";
    aPlayerMovement* playerMovement = new aPlayerMovement();
    aPlayerShooting* playerShooting = new aPlayerShooting();
    aTurretPlacer* playerTurPlacer = new aTurretPlacer();
    aToolManager* toolManager = new aToolManager();


    playerMovement->maze = mazeGenerator;

    toolManager->RegisterTool(GLFW_KEY_1, playerTurPlacer);
    toolManager->RegisterTool(GLFW_KEY_2, playerShooting);

    aGrassCollider* playerGrassCollider = new aGrassCollider();
    playerGrassCollider->SetGrass(puddle);
    playerGrassCollider->colliderRadius = 0.4f;

    playerShooting->factory = LAFactory;
    playerTurPlacer->factory = LAFactory;
    player->isTemporary = true;

    player->AddChild(scene->sceneObjects[31]);
    scene->sceneObjects[31]->name = "CHILD";
   
    glm::vec3 turretPos = player->mesh->positionXYZ;
    turretPos.y -= 1;

  //  LAFactory->SpawnTurret(turretPos, STANDARTBODY, STANDARTNECK, STANDARTHEAD);

    scene->AddActionToObj(playerGrassCollider, player);
    scene->AddActionToObj(playerMovement, player);
    scene->AddActionToObj(playerTurPlacer, player);
    scene->AddActionToObj(playerShooting, player);
    scene->AddActionToObj(toolManager, player);


    waveEffect->player = player;
    mazeGenerator->player = player;
    //scene->AddActionToObj(playerShooting, player);

    //aRotationWithMinutes* rotateCam2 = new aRotationWithMinutes();
    //rotateCam2->minRotation;
    //rotateCam2->minRotation;

    //securityRoomScene->AddActionToObj(rotateAction, securutyCamera2);

    Object* wall = securityRoomScene->GenerateMeshObjectsFromObject("assets/models/objects/wall01side.ply", glm::vec3(0.f, -5.f, -7.5f), 4, glm::vec3(0.f, 0.f, 0.f), false, glm::vec4(0.f, 1.f, 0.f, 1.f), false, securityRoomScene->sceneObjects);
    wall->mesh->textures[0] = "Wall_Simple_AlbedoTransparency.bmp";
    wall->mesh->blendRatio[0] = 1.0f;

    //Object* securityLight = securityRoomScene->SetLight(scene->lightManager, 0, glm::vec4(0.f, 10.f, 0.f, 1.f), glm::vec4(1, 1, 1, 1), glm::vec3(1, 1, 1), glm::vec4(0, -1, 0, 0), glm::vec3(1, 0, 0), 1);
    
    scene->AddActionToObj(mainCamera, scene->sceneObjects[0]);
    screen_quad->isTemporary = true;


    securityRoomScene->lightManager->CreateNewLight(glm::vec4(7.f, 0.5f, 0.f, 0.f), glm::vec4(1), glm::vec3(0.0002541, 2.19389e-06, 3.40282e+36), glm::vec4(0), glm::vec3(0), 1);
   

    SoftBody* softBody = new SoftBody();





    Object* softObject = scene->sceneObjects[31];
    //softObject->name = "ENEMY";

    BruteEnemy* bEnem2 = new BruteEnemy();
    bEnem2->maze = mazeGenerator;
    bEnem2->factory = LAFactory;
    //scene->AddActionToObj(bEnem2, softObject);

    softObject->mesh->metal = 0.8f;
    softObject->mesh->smoothness = 0.7f;
    softBody->acceleration.y = -16;
    softObject->mesh->drawBothFaces = true;
    softBody->constIterations = 15;
    softBody->sbCollision->collisionMult = 2.f;
    softBody->tighness = 2.f;
    //softObject->mesh->NMTexture = "Wall_Simple_Normal.bmp";
    // . . . . . . . . .
    softBody->SetMazeToSBCollision(mazeGenerator);
    softBody->useVolume = true;
    softBody->easyControl = true;
    softBody->inCylynder = false;
    scene->AddActionToObj(softBody, softObject);
    mazeGenerator->mainSlime = softBody;

    mazeGenerator->generateMaze();
    mazeSecurity->generateMaze();

    SoftBody* ropeBody = new SoftBody();
   // ropeBody->useVolume = true;
    ropeBody->acceleration.y = -100;
    ropeBody->yToJump = -100.f;
   // ropeBody->acceleration.x = 40;
  //  ropeBody->SetMazeToSBCollision(mazeGenerator);

    Object* ropeObject = scene->sceneObjects[32];
    ropeObject->mesh->drawBothFaces = true;
    ropeObject->mesh->bOverrideObjectColour = true;
    ropeObject->mesh->bDoNotLight = false;
    ropeObject->mesh->objectColourRGBA = glm::vec4(1);
    //error
    ropeBody->isLockOnY = true;
    ropeBody->zLockPos = -0.3f;
    ropeBody->checkGreaterZLock = false;
    ropeBody->AddSoftBodyToCollisions(softBody);
   scene->AddActionToObj(ropeBody, ropeObject);

    ConnSoftToObj* connector = new ConnSoftToObj();
    connector->softbody = ropeBody;

   // scene->AddActionToObj(connector, scene->sceneObjects[33]);

 
    
    // Ensure "Mountain" exists in the VAO Manager before using it
   // myVAOManager->FindDrawInfoByModelName("Mountain", *myModelInfo);

    // Correct constructor usage
   // SoftBody* softBodyAction = new SoftBody(mySoftbody, myVAOManager, "Mountain");

 //   scene->AddActionToObj(softBodyAction, softObject);


    Object* cameraObj2 = scene->sceneObjects[7];
    //Object* cameraObj3 = scene->sceneObjects[8];
    //Object* cameraObj4 = scene->sceneObjects[9];
    ////Object* cameraObj5 = scene->sceneObjects[23];

    CameraToTexture* textureCamera1 = new CameraToTexture();
    CameraToTexture* textureCamera2 = new CameraToTexture();
    CameraToTexture* textureCamera3 = new CameraToTexture();

    textureCamera1->drawistance = 50.f;
    textureCamera2->drawistance = 100.f;
    textureCamera3->drawistance = 500.f;
    textureCamera1->textureName = "securityCamera";
    textureCamera2->textureName = "camera1";
    textureCamera3->textureName = "securityCamera2";


    securityRoomScene->AddActionToObj(textureCamera1, securutyCamera);
    securityRoomScene->AddActionToObj(textureCamera3, securutyCamera2);
    scene->AddActionToObj(textureCamera2, cameraObj2);

    //MIRROR
    aMirrorReflection* mirrorReflection = new aMirrorReflection();
    mirrorReflection->mirrorCenter = puddle->mesh->positionXYZ;
    mirrorReflection->cameraController = textureCamera2;
    mirrorReflection->fovAdjustmentFactor = 20.f;
    mirrorReflection->mirrorObject = puddle;
    mirrorReflection->cameraHeight = 0.f;
   //scene->AddActionToObj(mirrorReflection, cameraObj2);



    ////FRONT SCREEN 1
    ScreenTextureSwitch* screenSwitcher = new ScreenTextureSwitch();

    screenSwitcher->AddTexture("camera1");
    screenSwitcher->AddTexture("securityCamera");
    screenSwitcher->AddTexture("securityCamera2");
    screenSwitcher->AddTextureLayer2("cam_top.bmp");
    screenSwitcher->AddTextureLayer2("cam_top2.bmp");
    screenSwitcher->AddTextureLayer2("cam_top3.bmp");

    scene->AddActionToObj(screenSwitcher, scene->sceneObjects[4]);

    scene->sceneObjects[4]->mesh->chromaticPower = 0.05;



  ////  FRONT SCREEN 2
  //  ScreenTextureSwitch* screenSwitcher2 = new ScreenTextureSwitch();

  //  screenSwitcher2->AddTexture("securityCamera");


  //  scene->AddActionToObj(screenSwitcher2, scene->sceneObjects[4]);

 

   scene->sceneObjects[5]->mesh->textures[0] = "tv_glitch.bmp";
   scene->sceneObjects[5]->mesh->blendRatio[0] = 1.f;
   scene->sceneObjects[5]->mesh->textureSpeed.x = 10;
   scene->sceneObjects[5]->mesh->textures[1] = "screen_broken.bmp";
   scene->sceneObjects[5]->mesh->blendRatio[1] = 0.75f;
   scene->sceneObjects[5]->mesh->bOverrideObjectColour = false;

    scene->sceneObjects[6]->mesh->textures[0] = "main_camera";
    scene->sceneObjects[6]->mesh->blendRatio[0] = 1.f;
    
     scene->sceneObjects[6]->mesh->textures[1] = "screen_broken.bmp";
    scene->sceneObjects[6]->mesh->blendRatio[1] = 0.75f;
    scene->sceneObjects[6]->mesh->bOverrideObjectColour = false; 

  //  puddle->isActive = false;

     puddle->mesh->textures[1] = "camera1";
     puddle->mesh->blendRatio[1] = 1.0f;
     puddle->mesh->bOverrideObjectColour = false;

    // scene->sceneObjects[5]->mesh->textures[0] = "securityCamera";
    //scene->sceneObjects[5]->mesh->blendRatio[0] = 1.f;
    // scene->sceneObjects[6]->mesh->textures[0] = "securityCamera";
    //scene->sceneObjects[6]->mesh->blendRatio[0] = 1.f;

    // LEFT SCREEN
    //ScreenTextureSwitch* screenSwitcher3 = new ScreenTextureSwitch();

    //screenSwitcher3->AddTexture("securityCamera");

    //scene->AddActionToObj(screenSwitcher3, scene->sceneObjects[5]);

    //// RIGHT SCREEN
    //ScreenTextureSwitch* screenSwitcher4 = new ScreenTextureSwitch();

    //screenSwitcher4->AddTexture("securityCamera");


//    scene->AddActionToObj(screenSwitcher4, scene->sceneObjects[6]);


    //Object* playerObject = scene->sceneObjects[31];

    //// Add the player camera action (with an offset for camera positioning)
    //aPlayerCamera* playerCameraAction = new aPlayerCamera(::g_pFlyCamera, glm::vec3(-10.0f, 20.0f, 0.0f));
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

        scene->sceneObjects[0]->mesh->textures[0] = "Pebbles_small.bmp"; 
        scene->sceneObjects[0]->mesh->blendRatio[0] = 2;
        scene->sceneObjects[0]->mesh->bOverrideObjectColour = false;
        //scene->sceneObjects[0]->mesh->transperency = 1;
        //scene->sceneObjects[0]->mesh->textureFillType[0] = 1;

        scene->sceneObjects[1]->mesh->textures[0] = "Computer_3_AlbedoTransparency.bmp";
        scene->sceneObjects[1]->mesh->textures[1] = "rust.bmp";
        scene->sceneObjects[1]->mesh->blendRatio[0] = 1;
        scene->sceneObjects[1]->mesh->blendRatio[1] = 1;
        scene->sceneObjects[1]->mesh->textureFillType[0] = 1;
        scene->sceneObjects[1]->mesh->bOverrideObjectColour = false;

        scene->sceneObjects[2]->mesh->textures[0] = "Computer_3_AlbedoTransparency.bmp";
        scene->sceneObjects[2]->mesh->textures[1] = "metalScratch.bmp";
        scene->sceneObjects[2]->mesh->blendRatio[0] = 1;
        scene->sceneObjects[2]->mesh->blendRatio[1] = 1;
        scene->sceneObjects[2]->mesh->textureFillType[1] = 1;
        scene->sceneObjects[2]->mesh->bOverrideObjectColour = false;

        scene->sceneObjects[3]->mesh->textures[0] = "Computer_3_AlbedoTransparency.bmp";
        scene->sceneObjects[3]->mesh->textures[1] = "rust.bmp";
        scene->sceneObjects[3]->mesh->blendRatio[0] = 1;
        scene->sceneObjects[3]->mesh->blendRatio[1] = 1;
        scene->sceneObjects[3]->mesh->textureFillType[1] = 1;
        scene->sceneObjects[3]->mesh->bOverrideObjectColour = false;

     /*   scene->sceneObjects[4]->mesh->textures[0] = "uv_mapper.bmp";
        scene->sceneObjects[4]->mesh->blendRatio[0] = 1;
        scene->sceneObjects[4]->mesh->textureFillType[1] = 1;
        scene->sceneObjects[4]->mesh->bOverrideObjectColour = false;
        scene->sceneObjects[4]->mesh->stencilTexture = "binaries.bmp";
        scene->sceneObjects[4]->mesh->stencilTextureID = 61;
        scene->sceneObjects[4]->mesh->textureSpeed.x = 0.1f;*/

      /*  scene->sceneObjects[5]->mesh->textures[0] = "uv_mapper.bmp";
        scene->sceneObjects[5]->mesh->blendRatio[0] = 1;
        scene->sceneObjects[5]->mesh->textureFillType[0] = 1;
        scene->sceneObjects[5]->mesh->bOverrideObjectColour = false;

        scene->sceneObjects[6]->mesh->textures[0] = "uv_mapper.bmp";
        scene->sceneObjects[6]->mesh->blendRatio[0] = 1;
        scene->sceneObjects[8]->mesh->textureFillType[1] = 1;
        scene->sceneObjects[6]->mesh->bOverrideObjectColour = false;*/

        //// Front Screen 1
        //scene->sceneObjects[7]->mesh->textures[0] = "gibberish.bmp";
        //scene->sceneObjects[7]->mesh->blendRatio[0] = 9;
        //scene->sceneObjects[7]->mesh->bOverrideObjectColour = false;
        //scene->sceneObjects[7]->mesh->bIsStencilTexture = true;
        //scene->sceneObjects[7]->mesh->stencilTexture = "WorldMap.bmp";
        //scene->sceneObjects[7]->mesh->stencilTextureID = 61;
        //scene->sceneObjects[7]->mesh->textureSpeed.x = 0.1f;

        //// Front Window
        //scene->sceneObjects[8]->mesh->textures[0] = "uv_mapper.bmp";
        //scene->sceneObjects[8]->mesh->textures[0] = "metalScratch.bmp";
        //scene->sceneObjects[8]->mesh->textures[0] = "fingerprint.bmp";
        //scene->sceneObjects[8]->mesh->blendRatio[0] = 3;
        //scene->sceneObjects[8]->mesh->blendRatio[0] = 3;
        //scene->sceneObjects[8]->mesh->blendRatio[0] = 3;
        //scene->sceneObjects[8]->mesh->textureFillType[1] = 1;
        //scene->sceneObjects[8]->mesh->bOverrideObjectColour = false;
        //scene->sceneObjects[8]->mesh->transperency = 0.4;

        //// Front Screen 2
        //scene->sceneObjects[9]->mesh->textures[0] = "gibberish.bmp";
        //scene->sceneObjects[9]->mesh->blendRatio[0] = 9;
        //scene->sceneObjects[9]->mesh->bOverrideObjectColour = false;
        ////scene->sceneObjects[9]->mesh->bIsStencilTexture = true;
        ////scene->sceneObjects[9]->mesh->stencilTexture = "binaries.bmp";
        ////scene->sceneObjects[9]->mesh->stencilTextureID = 61;
        ////scene->sceneObjects[9]->mesh->textureSpeed.x = 0.1f;
 // Left Screen
        scene->sceneObjects[10]->mesh->textures[0] = "Reactor_AlbedoTransparency.bmp";
        scene->sceneObjects[10]->mesh->blendRatio[0] = 9;
        scene->sceneObjects[10]->mesh->bOverrideObjectColour = false;
        //scene->sceneObjects[10]->mesh->bIsStencilTexture = true;
        //scene->sceneObjects[10]->mesh->stencilTexture = "binaries.bmp";
        //scene->sceneObjects[10]->mesh->stencilTextureID = 61;
        //scene->sceneObjects[10]->mesh->textureSpeed.x = 0.1f;

        // Left Window
        scene->sceneObjects[11]->mesh->textures[0] = "Reactor_AlbedoTransparency.bmp";
        scene->sceneObjects[11]->mesh->textures[0] = "fingerprint.bmp";
        scene->sceneObjects[11]->mesh->blendRatio[0] = 3;
        scene->sceneObjects[11]->mesh->blendRatio[1] = 3;
        scene->sceneObjects[11]->mesh->textureFillType[1] = 1;
        scene->sceneObjects[11]->mesh->bOverrideObjectColour = false;


        glm::vec3 tubeFix = glm::vec3(-2.4f, 2.5f, 1.9f);
 
        scene->sceneObjects[12]->mesh->textures[0] = "Frame_Tube_AlbedoTransparency.bmp";
        scene->sceneObjects[12]->mesh->STTexture = "Frame_Tube_MetallicSmoothness.bmp";
        scene->sceneObjects[12]->mesh->AOtexture = "Frame_Tube_AO.bmp";

        scene->sceneObjects[12]->mesh->blendRatio[0] = 9;
        scene->sceneObjects[12]->mesh->bOverrideObjectColour = false;
       // SpawnEffectInTudeOnPos(scene, scene->sceneObjects[12]->mesh->positionXYZ+tubeFix * scene->sceneObjects[12]->mesh->uniformScale);



        // Right Window
        scene->sceneObjects[13]->mesh->textures[0] = "Reactor_AlbedoTransparency.bmp";
        scene->sceneObjects[13]->mesh->textures[0] = "rust.bmp";
        scene->sceneObjects[13]->mesh->blendRatio[0] = 1;
        scene->sceneObjects[13]->mesh->blendRatio[0] = 1;
        scene->sceneObjects[13]->mesh->textureFillType[1] = 1;
        scene->sceneObjects[13]->mesh->bOverrideObjectColour = false;


        // Right Screen
        scene->sceneObjects[14]->mesh->textures[0] = "Frame_Tube_AlbedoTransparency.bmp";
        scene->sceneObjects[14]->mesh->STTexture = "Frame_Tube_MetallicSmoothness.bmp";
        scene->sceneObjects[14]->mesh->AOtexture = "Frame_Tube_AO.bmp";
        scene->sceneObjects[14]->mesh->blendRatio[0] = 9;
        scene->sceneObjects[14]->mesh->bOverrideObjectColour = false;
       // SpawnEffectInTudeOnPos(scene, scene->sceneObjects[14]->mesh->positionXYZ + tubeFix * scene->sceneObjects[14]->mesh->uniformScale);

        //Room

        scene->sceneObjects[15]->mesh->textures[0] = "Frame_Tube_AlbedoTransparency.bmp";
        scene->sceneObjects[15]->mesh->STTexture = "Frame_Tube_MetallicSmoothness.bmp";
        scene->sceneObjects[15]->mesh->AOtexture = "Frame_Tube_AO.bmp";
        scene->sceneObjects[15]->mesh->blendRatio[0] = 1;
        scene->sceneObjects[15]->mesh->bOverrideObjectColour = false;
    //    SpawnSlimeInTudeOnPos(scene, scene->sceneObjects[15]->mesh->positionXYZ + tubeFix * scene->sceneObjects[15]->mesh->uniformScale);


        scene->sceneObjects[16]->mesh->textures[0] = "Glass_Tube_AlbedoTransparency.bmp";
        scene->sceneObjects[16]->mesh->blendRatio[0] = 2;
        scene->sceneObjects[16]->mesh->bOverrideObjectColour = false;
        scene->sceneObjects[16]->mesh->transperency = 0.2;

        scene->sceneObjects[17]->mesh->textures[0] = "Glass_Tube_AlbedoTransparency.bmp";
        scene->sceneObjects[17]->mesh->blendRatio[0] = 2;
        scene->sceneObjects[17]->mesh->bOverrideObjectColour = false;
        scene->sceneObjects[17]->mesh->transperency = 0.2;

        scene->sceneObjects[18]->mesh->textures[0] = "Glass_Tube_AlbedoTransparency.bmp";
        scene->sceneObjects[18]->mesh->blendRatio[0] = 1;
        scene->sceneObjects[18]->mesh->bOverrideObjectColour = false;
        scene->sceneObjects[18]->mesh->transperency = 0.2;

        scene->sceneObjects[19]->mesh->textures[0] = "Frame_Tube_AlbedoTransparency.bmp";
        scene->sceneObjects[19]->mesh->STTexture = "Frame_Tube_MetallicSmoothness.bmp";
        scene->sceneObjects[19]->mesh->AOtexture = "Frame_Tube_AO.bmp";
        scene->sceneObjects[19]->mesh->blendRatio[0] = 2;
        scene->sceneObjects[19]->mesh->bOverrideObjectColour = false;

        //SpawnSlimeInTudeOnPos(scene, scene->sceneObjects[19]->mesh->positionXYZ);

        scene->sceneObjects[20]->mesh->textures[0] = "Frame_Tube_AlbedoTransparency.bmp";
        scene->sceneObjects[20]->mesh->STTexture = "Frame_Tube_MetallicSmoothness.bmp";
        scene->sceneObjects[20]->mesh->AOtexture = "Frame_Tube_AO.bmp";
        scene->sceneObjects[20]->mesh->blendRatio[0] = 2;
        scene->sceneObjects[20]->mesh->bOverrideObjectColour = false;
        SpawnSlimeInTudeOnPos(scene, scene->sceneObjects[20]->mesh->positionXYZ + tubeFix * scene->sceneObjects[20]->mesh->uniformScale);


        scene->sceneObjects[21]->mesh->textures[0] = "Frame_Tube_AlbedoTransparency.bmp";
        scene->sceneObjects[21]->mesh->STTexture = "Frame_Tube_MetallicSmoothness.bmp";
        scene->sceneObjects[21]->mesh->AOtexture = "Frame_Tube_AO.bmp";
        scene->sceneObjects[21]->mesh->blendRatio[0] = 1;
        scene->sceneObjects[21]->mesh->bOverrideObjectColour = true;
        SpawnEffectInTudeOnPos(scene, scene->sceneObjects[21]->mesh->positionXYZ + tubeFix * scene->sceneObjects[21]->mesh->uniformScale);


        scene->sceneObjects[22]->mesh->textures[0] = "Glass_Tube_AlbedoTransparency.bmp";
        scene->sceneObjects[22]->mesh->blendRatio[0] = 2;
        scene->sceneObjects[22]->mesh->bOverrideObjectColour = false;
        scene->sceneObjects[22]->mesh->transperency = 0.2;

        scene->sceneObjects[23]->mesh->textures[0] = "Glass_Tube_AlbedoTransparency.bmp";
        scene->sceneObjects[23]->mesh->blendRatio[0] = 2;
        scene->sceneObjects[23]->mesh->bOverrideObjectColour = false;
        scene->sceneObjects[23]->mesh->transperency = 0.2;

        scene->sceneObjects[24]->mesh->textures[0] = "Glass_Tube_AlbedoTransparency.bmp";
        scene->sceneObjects[24]->mesh->blendRatio[0] = 2;
        scene->sceneObjects[24]->mesh->bOverrideObjectColour = false;
        scene->sceneObjects[24]->mesh->transperency = 0.2;

        scene->sceneObjects[25]->mesh->textures[0] = "Operating_Table_AlbedoTransparency.bmp";
        scene->sceneObjects[25]->mesh->STTexture = "Operating_Table_MetallicSmoothness.bmp";
        scene->sceneObjects[25]->mesh->AOtexture= "Operating_Table_AO.bmp";
        scene->sceneObjects[25]->mesh->NMTexture= "Operating_Table_Normal.bmp";
      //  scene->sceneObjects[25]->mesh->textureSpeed.x = 1.f;

        scene->sceneObjects[25]->mesh->blendRatio[0] = 2;
        scene->sceneObjects[25]->mesh->bOverrideObjectColour = false;

        scene->sceneObjects[26]->mesh->textures[0] = "Operating_Table_AlbedoTransparency.bmp";
        scene->sceneObjects[26]->mesh->STTexture = "Operating_Table_MetallicSmoothness.bmp";
        scene->sceneObjects[26]->mesh->AOtexture = "Operating_Table_AO.bmp";
        scene->sceneObjects[26]->mesh->NMTexture = "Operating_Table_Normal.bmp";
       // scene->sceneObjects[26]->mesh->textureSpeed.x = 1.f;
        scene->sceneObjects[26]->mesh->blendRatio[0] = 2;
        scene->sceneObjects[26]->mesh->bOverrideObjectColour = false;

        scene->sceneObjects[27]->mesh->textures[0] = "Operating_Table_AlbedoTransparency.bmp";
        scene->sceneObjects[27]->mesh->STTexture = "Operating_Table_MetallicSmoothness.bmp";
        scene->sceneObjects[27]->mesh->AOtexture = "Operating_Table_AO.bmp";
        scene->sceneObjects[27]->mesh->NMTexture = "Operating_Table_Normal.bmp";
      //  scene->sceneObjects[27]->mesh->textureSpeed.x = 1.f;
        scene->sceneObjects[27]->mesh->blendRatio[0] = 2;
        scene->sceneObjects[27]->mesh->bOverrideObjectColour = false;

        scene->sceneObjects[28]->mesh->textures[0] = "Lockdown_Light_AlbedoTransparency.bmp";
        scene->sceneObjects[28]->mesh->blendRatio[0] = 2;
        scene->sceneObjects[28]->mesh->bOverrideObjectColour = false;

        scene->sceneObjects[29]->mesh->textures[0] = "Lockdown_Light_AlbedoTransparency.bmp";
        scene->sceneObjects[29]->mesh->blendRatio[0] = 2;
        scene->sceneObjects[29]->mesh->bOverrideObjectColour = false;

        scene->sceneObjects[30]->mesh->textures[0] = "Lockdown_Light_AlbedoTransparency.bmp";
        scene->sceneObjects[30]->mesh->blendRatio[0] = 2;
        scene->sceneObjects[30]->mesh->bOverrideObjectColour = false;
        
        scene->sceneObjects[31]->mesh->textures[0] = "slime.bmp";
        scene->sceneObjects[31]->mesh->blendRatio[0] = 2;
        scene->sceneObjects[31]->mesh->bOverrideObjectColour = false;
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
    std::string errorDepth;
    GLuint depthProgram = LoadShaderProgram("deapth","assets/shaders/depth_vertex.glsl","assets/shaders/depth_fragment.glsl", errorDepth);
    if (!depthProgram) { std::cerr << "Depth Shader Error: " << errorDepth << std::endl; }
    GLuint particleProgram = LoadShaderProgram("particle","assets/shaders/particle_vertex.glsl","assets/shaders/particle_fragment.glsl", errorDepth);  //  if (!particleProgram) { std::cerr << "Particle Shader Error: " << errorDepth << std::endl; }
    if (!particleProgram) { std::cerr << "Particle Shader Error: " << errorDepth << std::endl; }
    //NOTE : GRIDS
    //GridRenderer gridRenderer;
    //if (!gridRenderer.Initialize()) {
    //    std::cerr << "Failed to initialize grid renderer!" << std::endl;
    //    return EXIT_FAILURE;
    //}
   



    SetupDearImGui(window);

 
   PlayerUI::Initialize();

//   PREPARING ENGINE STUFF
//   ----------------------

    scene->vaoManager = new cVAOManager();
    PhysicsManager* physicsMan = new PhysicsManager();
    PrepareFlyCamera();


//   PREPARING SCENE
//   ---------------
 
    Scene* cameraScene = new Scene();
    Scene* secutityRoomScene = new Scene();
    secutityRoomScene->lightManager = new cLightManager();

    scene->Prepare(scene->vaoManager, program, physicsMan, window, g_pFlyCamera);
  //  scene->depthProgram = depthProgram;

    cFBO_RGB_depth* depthFBO = new cFBO_RGB_depth();
    std::string error;
    if (!depthFBO->init(1920, 1080, error, "DepthTexture", scene->textureManager)) {
        std::cerr << "FBO Error: " << error << std::endl;
    }
    scene->depthFBO = depthFBO;
    scene->particleProgram = particleProgram;


    //HACK FOR LIGHTING
    secutityRoomScene->lightManager->loadUniformLocations(scene->programs[0]);
   // cameraScene->Prepare(scene->vaoManager, program, physicsMan, window, g_pFlyCamera);
    cameraScene->textureManager = scene->textureManager;
    cameraScene->programs = scene->programs;
    cameraScene->vaoManager = scene->vaoManager;
    cameraScene->fCamera = g_pFlyCamera;

    secutityRoomScene->textureManager = scene->textureManager;
    secutityRoomScene->programs = scene->programs;
    secutityRoomScene->vaoManager = scene->vaoManager;
    secutityRoomScene->fCamera = g_pFlyCamera;
    secutityRoomScene->physicsManager = scene->physicsManager;
    LabAttackFactory* factory = nullptr;
    AddActions(scene, cameraScene, secutityRoomScene, program, &factory);

    Animator* animator = new Animator();
    animator->scene = scene;
    

    cPhysics* g_pPhysicEngine = new cPhysics();
    g_pPhysicEngine->setVAOManager(scene->vaoManager);


//   PREPARING SOMETHING ELSE (TODO: Try to put it away into functions)
//   ------------------------
    std::cout << "Textures Load Start" << std::endl;

    glUseProgram(program);

    scene->textureManager->SetBasePath("assets/textures");
    scene->textureManager->Create2DTextureFromBMPFile("mountain.bmp");
    scene->textureManager->Create2DTextureFromBMPFile("Computer_3_AlbedoTransparency.bmp");
    scene->textureManager->Create2DTextureFromBMPFile("Wall_Simple_AlbedoTransparency.bmp");
    scene->textureManager->Create2DTextureFromBMPFile("Pebbles_small.bmp");
    scene->textureManager->Create2DTextureFromBMPFile("Floor_Albedo.bmp");
    scene->textureManager->Create2DTextureFromBMPFile("WallAO.bmp");
    scene->textureManager->Create2DTextureFromBMPFile("Wall_Simple_MetallicSmoothness.bmp");
    scene->textureManager->Create2DTextureFromBMPFile("fingerprint.bmp");
    scene->textureManager->Create2DTextureFromBMPFile("ceilling.bmp");
    scene->textureManager->Create2DTextureFromBMPFile("MinoE.bmp");
    scene->textureManager->Create2DTextureFromBMPFile("Water.bmp");
    scene->textureManager->Create2DTextureFromBMPFile("rust.bmp");
    scene->textureManager->Create2DTextureFromBMPFile("metalScratch.bmp");
    scene->textureManager->Create2DTextureFromBMPFile("screen_broken.bmp");
    scene->textureManager->Create2DTextureFromBMPFile("CrossHair.bmp");
    scene->textureManager->Create2DTextureFromBMPFile("Lockdown_Light_AlbedoTransparency.bmp");
    scene->textureManager->Create2DTextureFromBMPFile("uv_mapper.bmp");
    scene->textureManager->Create2DTextureFromBMPFile("frog_diffuse.bmp");
    scene->textureManager->Create2DTextureFromBMPFile("binaries.bmp");
    scene->textureManager->Create2DTextureFromBMPFile("slime.bmp");
    scene->textureManager->Create2DTextureFromBMPFile("castle_element_05_BaseColour.bmp");
    scene->textureManager->Create2DTextureFromBMPFile("Tube_AlbedoTransparency.bmp");
    scene->textureManager->Create2DTextureFromBMPFile("TubeAO.bmp");
    scene->textureManager->Create2DTextureFromBMPFile("Vent_Big_AlbedoTransparency.bmp");
    scene->textureManager->Create2DTextureFromBMPFile("Glass_Tube_AlbedoTransparency.bmp");
    scene->textureManager->Create2DTextureFromBMPFile("Frame_Tube_AlbedoTransparency.bmp");
    scene->textureManager->Create2DTextureFromBMPFile("Frame_Tube_AO.bmp");
    scene->textureManager->Create2DTextureFromBMPFile("Frame_Tube_MetallicSmoothness.bmp");
    scene->textureManager->Create2DTextureFromBMPFile("Reactor_AlbedoTransparency.bmp");
    scene->textureManager->Create2DTextureFromBMPFile("Operating_Table_AlbedoTransparency.bmp");
    scene->textureManager->Create2DTextureFromBMPFile("Operating_Table_AO.bmp");
    scene->textureManager->Create2DTextureFromBMPFile("Operating_Table_MetallicSmoothness.bmp");
    scene->textureManager->Create2DTextureFromBMPFile("Operating_Table_Normal.bmp");
    scene->textureManager->Create2DTextureFromBMPFile("cam_top.bmp");
    scene->textureManager->Create2DTextureFromBMPFile("cam_top2.bmp");
    scene->textureManager->Create2DTextureFromBMPFile("cam_top3.bmp");
    scene->textureManager->Create2DTextureFromBMPFile("tv_glitch.bmp");
    scene->textureManager->Create2DTextureFromBMPFile("Wall_Simple_Normal.bmp");
    scene->textureManager->Create2DTextureFromBMPFile("Floor_Normal.bmp");
    scene->textureManager->Create2DTextureFromBMPFile("Operating_Table_Normal.bmp");
    scene->textureManager->Create2DTextureFromBMPFile("castle_element_05_NormalGL.bmp");
    scene->textureManager->Create2DTextureFromBMPFile("castle_element_05_MetalSmoothness.bmp");
    scene->textureManager->Create2DTextureFromBMPFile("Turret/Turret_Albedo.bmp");
    scene->textureManager->Create2DTextureFromBMPFile("Turret/Turret_Normal.bmp");
    scene->textureManager->Create2DTextureFromBMPFile("Turret/Turret_Occlusion.bmp");
    scene->textureManager->Create2DTextureFromBMPFile("Turret/Turret_ST.bmp");
    scene->textureManager->Create2DTextureFromBMPFile("Bullet/Bullet_colour.bmp");
    scene->textureManager->Create2DTextureFromBMPFile("Bullet/Bullet_AO.bmp");
    scene->textureManager->Create2DTextureFromBMPFile("Bullet/Bullet_met_smothness.bmp");
    scene->textureManager->Create2DTextureFromBMPFile("Bullet/Bullet_normals.bmp");
    scene->textureManager->Create2DTextureFromBMPFile("Turret2/Turret2_albedo.bmp");
    scene->textureManager->Create2DTextureFromBMPFile("Turret2/Turret2_AO.bmp");
    scene->textureManager->Create2DTextureFromBMPFile("Turret2/Turret2_normal.bmp");
    scene->textureManager->Create2DTextureFromBMPFile("Turret2/Turret2_MS.bmp");
    scene->textureManager->Create2DTextureFromBMPFile("Turret_wheel/Wheel_Enem_head_Colour.bmp");
    scene->textureManager->Create2DTextureFromBMPFile("Turret_wheel/Wheel_Enem_body_color.bmp");
    scene->textureManager->Create2DTextureFromBMPFile("Turret_wheel/Wheel_Enem_wheel_color.bmp");
    scene->textureManager->Create2DTextureFromBMPFile("Turret_wheel/Wheel_Enem_body_Normal.bmp");
    scene->textureManager->Create2DTextureFromBMPFile("Turret_wheel/Wheel_Enem_head_Normal.bmp");
    scene->textureManager->Create2DTextureFromBMPFile("Turret_wheel/Wheel_Enem_wheel_Normal.bmp");
    scene->textureManager->Create2DTextureFromBMPFile("Turret_wheel/Wheel_Enem_head_SM.bmp");
    scene->textureManager->Create2DTextureFromBMPFile("Turret_wheel/Wheel_Enem_head_SM.bmp");
    scene->textureManager->Create2DTextureFromBMPFile("Turret_wheel/Wheel_Enem_wheel_SM.bmp");
    scene->textureManager->Create2DTextureFromBMPFile("Grass/Grass_basecolor.bmp");
    scene->textureManager->Create2DTextureFromBMPFile("Grass/Grass_ambientOcclusion.bmp");
    scene->textureManager->Create2DTextureFromBMPFile("Grass/Grass_normal.bmp");
    scene->textureManager->Create2DTextureFromBMPFile("Grass/Grass_smoothness.bmp");
 



    std::cout << "Skybox Texture Load Start" << std::endl;

    // lookings
    //std::string errorString;
    //if (scene->textureManager->CreateCubeTextureFromBMPFiles("Space",
    //    "CubeMaps/SpaceBox_left2_negX.bmp",
    //    "CubeMaps/SpaceBox_right1_posX.bmp",
    //    "CubeMaps/SpaceBox_top3_posY.bmp",
    //    "CubeMaps/SpaceBox_bottom4_negY.bmp",
    //    "CubeMaps/SpaceBox_front5_posZ.bmp",
    //    "CubeMaps/SpaceBox_back6_negZ.bmp", true, errorString))
    //{
    //    std::cout << "Loaded space skybox" << std::endl;
    //}
    //else
    //{
    //    std::cout << "ERROR: Didn't load space skybox because: " << errorString << std::endl;
    //}

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

    sceneEditor->Start("selectBox.txt",fileManager, program, window, scene->vaoManager, scene, factory);





//   GENERATING MAZE
//   ---------------
    
    //for (int i = 0; i < 100; i++)
    //{
    //    
    //    std::cout << "Maze position: " << mazeGenerator->GetMazePoint(i, 0) << std::endl;
    //}

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

    scene->skybox = SkySphere;

   

    glClearColor(0.0f, 0.0f, 0.2f, 1.0f);
  


    scene->Start();
    secutityRoomScene->Start();

 

    //  Turn on the blend operation
    glEnable(GL_BLEND);
    // Do alpha channel transparency
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);

    // HACK:
    unsigned int numberOfNarrowPhaseTrianglesInAABB_BroadPhaseThing = 0;



   Camera* mainCamera =  cameraScene->AddCamera(glm::vec3(16.f, 0.5f, 0.f), glm::vec3(0.f,179.07f,0.f), glm::vec2(1920.f, 1080.f));
  // Camera* securutyCamera=  cameraScene->AddCamera(glm::vec3(16.f, 0.5f, 0.f), glm::vec3(0.f,179.07f,0.f), glm::vec2(1920.f, 1080.f));
  // mainCamera->nightMode = true;



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
        secutityRoomScene->Update();
        

        //scene->sceneObjects[0]->mesh->positionXYZ = scene->fCamera->getEyeLocation();
        //scene->sceneObjects[0]->mesh->rotationEulerXYZ = scene->fCamera->getCameraData()->rotation;


        SkySphere->mesh->positionXYZ = scene->fCamera->getCameraData()->position;
        //SkySphere->mesh->positionXYZ.x -= 5.0f;


        SkySphere->mesh->bIsVisible = true;
        //        pSkySphere->bDoNotLight = true;

        SkySphere->mesh->uniformScale = 25.0f;

        /*scene->lightManager->theLights[1].position.x = scene->sceneObjects[15]->mesh->positionXYZ.x;
        scene->lightManager->theLights[1].position.y = scene->sceneObjects[15]->mesh->positionXYZ.y;
        scene->lightManager->theLights[1].position.z = scene->sceneObjects[15]->mesh->positionXYZ.z;*/


        DrawSkyBox(SkySphere, program, scene->vaoManager, scene->textureManager, scene->fCamera->getCameraData());
//      DRAW LOOP
//      ------------------------------------------       
        //scene->SortObjectsForDrawing();
        //for (Object* object:scene->sceneObjectsSorted)
        //{
  
        //    sMesh* pCurMesh = object->mesh;

        //    DrawMesh(pCurMesh, program, scene->vaoManager, scene->textureManager, scene);

        //}


      // DrawCameraViewToFramebufer(scene->fCamera->getCameraData(), 0,1);
       DrawCameraViewToFramebufer(cameraScene->cameras[0], 0, 0);
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
  
            RenderDearImGui(sceneEditor, factory);
           
          
                handleKeyboardAsync(window, screen_quad, scene);
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





