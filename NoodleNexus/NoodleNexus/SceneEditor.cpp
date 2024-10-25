#include "SceneEditor.h"
#include "KLFileManager.hpp"
#include "cVAOManager/cVAOManager.h"


void SceneEditor::Start(std::string selectBoxPath, KLFileManager klFileManger, GLuint program, GLFWwindow* currentWindow, cVAOManager* managerVAO, Scene* currentScene)
{
    scene = currentScene;
	sModelDrawInfo modelInfo = klFileManger.ReadModelFile(selectBoxPath);
   
    Object* object = new Object();
    object->mesh = new sMesh();

    managerVAO->LoadModelIntoVAO(modelInfo.meshPath, modelInfo, program);

	//Load Model
    object->name = "SelectBox"; //Object Name     //Not ssure if this will work
    object->mesh->uniqueFriendlyName = "SelectBox"; //TODO: THis is so bad. Object should have on;y one name. Mesh Should not probably.
    object->mesh->bDoNotLight = true;
    object->mesh->bOverrideObjectColour = true;
    object->mesh->objectColourRGBA = glm::vec4(0, 1.f, 0, 1.f);
    object->mesh->uniformScale = 1.f;
    object->mesh->modelFileName = modelInfo.meshPath;

    selectBox = object;
    scene->sceneObjects.push_back(selectBox);

    window = currentWindow;

 

}

void SceneEditor::Update()
{
    if (selectedObject != nullptr)
      UpdateSelectBox();
    
    HandleInputAsync(window);
}

void SceneEditor::UpdateSelectBox()
{
    selectBox->mesh->positionXYZ = selectedObject->startTranform->position;
    selectBox->mesh->uniformScale = selectedObject->startTranform->scale.x;
    selectBox->mesh->rotationEulerXYZ = selectedObject->startTranform->rotation;

}


Object* SceneEditor::PickFirstObject()
{
    if (scene->sceneObjects.size() < 1) return nullptr;

    selectedObject = scene->sceneObjects[0];


}

Object* SceneEditor::PickNextObject()
{

    if (objIndex >= scene->sceneObjects.size()-1) objIndex = -1;


    objIndex++;
    selectedObject = scene->sceneObjects[objIndex];


	return nullptr;
}

