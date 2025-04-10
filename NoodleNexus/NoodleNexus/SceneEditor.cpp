#include "SceneEditor.h"
#include "KLFileManager.hpp"
#include "cVAOManager/cVAOManager.h"
#include "LabAttackFactory.h"

void SceneEditor::Start(std::string selectBoxPath, KLFileManager* klFileManger, GLuint program, GLFWwindow* currentWindow, cVAOManager* managerVAO, Scene* currentScene, LabAttackFactory* factory)
{
    scene = currentScene;
	sModelDrawInfo modelInfo = klFileManger->ReadModelFile(selectBoxPath);

    Object* object = new Object();
    object->mesh = new sMesh();
    object->isTemporary = true;  // Mark select box as temporary


    managerVAO->LoadModelIntoVAO(modelInfo.meshPath, modelInfo, program);

	//Load Model
    object->name = "SelectBox"; //Object Name     //Not ssure if this will work
    object->mesh->uniqueFriendlyName = "SelectBox"; //TODO: THis is so bad. Object should have on;y one name. Mesh Should not probably.
    object->mesh->bDoNotLight = true;
    object->mesh->bOverrideObjectColour = true;
    object->mesh->objectColourRGBA = glm::vec4(0, 1.f, 0, 1.f);
    object->mesh->uniformScale = 5.f;
    object->mesh->modelFileName = modelInfo.meshPath;
    object->mesh->bIsWireframe = true;
    object->isTemporary = true;

    selectBox = object;
    scene->sceneObjects.push_back(selectBox);
    lastFrameTime = glfwGetTime();
    window = currentWindow;

    editMode = "Objects";

    PickFirstObject();
    PickFirstLight();

    
    ghostTurret = factory->SpawnTurretGhost(glm::vec3(0), STANDARTBODY, STANDARTNECK, STANDARTHEAD);
    fileManger = klFileManger;

    scene->sceneEditor = this;
 

}

void SceneEditor::ChangeMode(std::string mode)
{
    editMode = mode;

    if (mode == "Lights")
        PickFirstLight();
}

void SceneEditor::Update()
{
    if (selectedObject != nullptr)
    {
   

        // Update selected object's mesh properties
        if (selectedObject->startTranform != nullptr)
        {
            UpdateSelectBox();
            selectedObject->mesh->positionXYZ = selectedObject->startTranform->position;
            selectedObject->mesh->rotationEulerXYZ = selectedObject->startTranform->rotation;
            selectedObject->mesh->uniformScale = selectedObject->startTranform->scale.x;
        }
    }
    HandleInputAsync(window);



}

void SceneEditor::UpdateSelectBox()
{
    if (editMode == "Objects")
    {
        selectBox->mesh->positionXYZ = selectedObject->GetWorldPosition();
        selectBox->mesh->rotationEulerXYZ = selectedObject->GetWorldRotation();
        selectBox->mesh->uniformScale = selectedObject->GetWorldScale();

    
    }
    if (editMode == "Lights")
    {
        selectBox->mesh->positionXYZ = selectedLight->position;
        selectBox->mesh->rotationEulerXYZ = glm::vec3(0, 0, 0);
    }



}


cLightManager::sLight* SceneEditor::PickFirstLight()
{
    
    lightIndex = 0;
    selectedLight = &scene->lightManager->theLights[0];
    return selectedLight;
}

cLightManager::sLight* SceneEditor::PickNextLight()
{
    if (lightIndex >= 19) lightIndex = -1; //THIS MIGHT NEED CHANGING OR MIGHT EXPLODE
    
    lightIndex++;
        selectedLight = &scene->lightManager->theLights[lightIndex];  

        return selectedLight;
}



//Object* SceneEditor::PickFirstObject()
//{
//    if (scene->sceneObjects.size() < 1) return nullptr;
//
//    selectedObject = scene->sceneObjects[0];
//
//
//}

//Object* SceneEditor::PickNextObject()
//{
//
//    if (objIndex >= scene->sceneObjects.size()-1) objIndex = -1;
//
//
//    objIndex++;
//    if (selectBox == scene->sceneObjects[objIndex])
//    {
//        return PickNextObject();
//    }
//    selectedObject = scene->sceneObjects[objIndex];
//
//
//	return nullptr;
//}

Object* SceneEditor::PickFirstObject()
{
    if (scene->sceneObjects.empty())
    {
        selectedObject = nullptr; // Clear the pointer if no objects
        return nullptr;
    }
    if (selectedObject)
        if (selectWireMode)  selectedObject->mesh->bIsWireframe = false;

    selectedObject = scene->sceneObjects[0];
    if (selectWireMode)  selectedObject->mesh->bIsWireframe = true;
    return selectedObject;
}

Object* SceneEditor::PickNextObject()
{
    if (scene->sceneObjects.empty())
        return nullptr;
    if(selectWireMode) selectedObject->mesh->bIsWireframe = false;
  

    objIndex = (objIndex + 1) % scene->sceneObjects.size(); // Wrap around
    selectedObject = scene->sceneObjects[objIndex];
    if (selectWireMode) selectedObject->mesh->bIsWireframe = true;


    // Ensure we don't select the selectBox again
    if (selectedObject == selectBox)
    {
        return PickNextObject();
    }

    return selectedObject;
}