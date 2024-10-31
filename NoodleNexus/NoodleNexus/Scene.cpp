#include "Scene.hpp"
#include "cVAOManager/cVAOManager.h"
#include "GLCommon.h"
#include <iostream>



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

void Scene::Prepare(cVAOManager* meshManager, GLuint program, std::vector<sMesh*>& meshes, cVAOManager* vaoMan)
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



    for (Object* object : sceneObjects)
    {
        for (Action* action : object->actions)
        {
            if (object->enabled)
                action->Start();
        }
    }

    physicsMan = new PhysicsManager();
    physicsMan->VAOMan = vaoMan;
    //TODO: PHYSICS SHOULD BE ASSIGNED AUTOMATIACLLY NOT IN MAIN

}

void Scene::Update()
{
    //for (Object* obj : sceneObjects)
    //{
    //    obj->Update();
    //}
    CalculateDeltaTime();
    if(!flyCamera)
    MoveCameraToPoint();


    for (Object* object : sceneObjects)
    {
        if (object)
        for (Action* action : object->actions)
        {
            if (object->enabled)
            action->Update();
        }
    }
}

void Scene::AddActionToObj(Action* action, Object* object)
{
    action->object = object;
    actions.push_back(action);
    object->actions.push_back(action);
}

//CREATE SHADED OBJECT WITH THIS COLOR
Object* Scene::CreateObject(glm::vec3 pos, glm::vec3 rotation, float scale, glm::vec4 color, std::string name, std::string modelName)
{
    Object* obj = new Object();
    obj->startTranform->position = pos;
    obj->startTranform->rotation = rotation;
    obj->startTranform->scale.x = scale;

    obj->mesh = new sMesh();
    obj->mesh->positionXYZ = obj->startTranform->position;
    obj->mesh->rotationEulerXYZ = obj->startTranform->rotation;
    obj->mesh->uniformScale = obj->startTranform->scale.x;

    obj->name = name;
    obj->mesh->uniqueFriendlyName = name;
    obj->mesh->modelFileName = modelName;
    

    obj->scene = this;
    obj->isTemporary = true;

    obj->mesh->bIsVisible = true;
    obj->mesh->bOverrideObjectColour = true;
    obj->mesh->objectColourRGBA = color;
    obj->mesh->bDoNotLight = false;

    sceneObjects.push_back(obj);
    
    return obj;

}

//A POSSIBILITY TO CREATE NON SHADED OBJECT
Object* Scene::CreateObject(glm::vec3 pos, glm::vec3 rotation, float scale, glm::vec4 color, std::string name, std::string modelName, bool isShaded)
{
    Object* obj = CreateObject(pos, rotation, scale, color, name, modelName);
    obj->mesh->bDoNotLight = !isShaded;
   
    return obj;

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
    std::vector<Object*>::iterator it = std::find(sceneObjects.begin(), sceneObjects.end(), obj);
    if (it != sceneObjects.end())
    {
        delete* it; // Free the memory if dynamically allocated
        sceneObjects.erase(it); // Remove from vector
    }
    
}

void Scene::CalculateDeltaTime()
{
    // Get the current time using glfwGetTime
    currentTime = glfwGetTime();

    // Calculate deltaTime
    deltaTime = currentTime - lastTime;

    // Update lastTime for the next frame
    lastTime = currentTime;
}
