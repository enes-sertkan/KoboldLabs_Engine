#include "Scene.hpp"
#include "cVAOManager/cVAOManager.h"
#include "GLCommon.h"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <cmath> 




sModelDrawInfo LoadDrawInfo(cVAOManager* meshManager, sModelDrawInfo drawInfo, GLuint program)
{
    meshManager->LoadModelIntoVAO(drawInfo.meshPath,
        drawInfo, program);

    std::cout << drawInfo.meshPath << "-Loaded" << std::endl << drawInfo.numberOfVertices << " vertices loaded" << std::endl;

    return drawInfo;
}


sMesh* CreateMeshObjects(std::vector<sMesh*>& meshes, sMesh* mesh)
{

    meshes.push_back(mesh);

    return mesh;

}



void Scene::Prepare(cVAOManager* meshManager, GLuint program, std::vector<sMesh*>& meshes, PhysicsManager* physMan, GLFWwindow* newWindow, cBasicFlyCamera* newFlyCamera)
{
    for (sModelDrawInfo info : modelInfos)
    {
        LoadDrawInfo(meshManager, info, program);
    }


    for (Object* object : sceneObjects)
    {
        CreateMeshObjects(meshes, object->mesh);
        object->scene = this;
    }

    physicsManager = physMan;
    
    physicsManager->VAOMan = meshManager;

    window = newWindow;

    fCamera = newFlyCamera;
}

void Scene::Update()
{
    //for (Object* obj : sceneObjects)
    //{
    //    obj->Update();
    //}
    if(!flyCamera)
    MoveCameraToPoint();


    for (Object* obj : sceneObjects)
    {
        if (obj)
        {
            for (Action* action : obj->actions)
            {
                action->Update();
            }
        }
    }

}

void Scene::AddActionToObj(Action* action, Object* object)
{
    action->object = object;
    object->actions.push_back(action);
    actions.push_back(action);
}

void Scene::MoveCameraToPoint()
{
   if (cameraPositions.size() == 0) return;
   glm::vec3 newPos = moveTowards(g_pFlyCamera->getEyeLocation(), cameraPositions[currentCameraIndex]->position, 10.f);
   g_pFlyCamera->setEyeLocation(newPos);
}

void Scene::SetCameraToNextPoint()
{
    if (cameraPositions.size() == 0) return;

    if (currentCameraIndex >= cameraPositions.size() - 1) currentCameraIndex = -1;
    currentCameraIndex++;

    g_pFlyCamera->setEyeLocation(cameraPositions[currentCameraIndex]->position);
}

void Scene::SetCameraToFirstPoint()
{
    if (cameraPositions.size() == 0) return;
    g_pFlyCamera->setEyeLocation(cameraPositions[1]->position);
}

void Scene::NextCameraPoint()
{
      if (cameraPositions.size() == 0) return;

    if (currentCameraIndex >= cameraPositions.size() - 1) currentCameraIndex = -1;
    currentCameraIndex++;
}

void Scene::RemoveObject(Object* obj)
{
    auto it = std::find(sceneObjects.begin(), sceneObjects.end(), obj);
    if (it != sceneObjects.end())
    {
        delete* it; // Free the memory if dynamically allocated
        sceneObjects.erase(it); // Remove from vector
    }
    
}


Object * Scene::GenerateMeshObjectsFromObject(
    const std::string& filePath, glm::vec3 posXYZ, glm::vec3 rotXYZ,
    bool bOverrideColor, glm::vec4 objectColor, bool bDoLightingExist,
    std::vector<Object*>& sceneObjects // Pass sceneObjects as a parameter
) {
    Object* object = new Object;
    sMesh* Meshes = new sMesh();
    object->mesh = Meshes;

    // Initialize mesh properties
    object->mesh->modelFileName = filePath;
    object->mesh->positionXYZ = posXYZ;
    object->mesh->rotationEulerXYZ = rotXYZ;
    object->mesh->bOverrideObjectColour = bOverrideColor;
    object->mesh->objectColourRGBA = objectColor;
    object->isTemporary;
    object->isCollisionStatic;
    object->name;

    // Set lighting based on the parameter
    object->mesh->bDoNotLight = !bDoLightingExist;
    
    // Add the object to the scene
    sceneObjects.push_back(object);

    return object;
}