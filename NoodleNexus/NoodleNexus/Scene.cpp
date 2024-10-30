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

void Scene::Prepare(cVAOManager* meshManager, GLuint program, std::vector<sMesh*>& meshes)
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

    

}

void Scene::Update()
{
    //for (Object* obj : sceneObjects)
    //{
    //    obj->Update();
    //}
    MoveCameraToPoint();

    for (Action* action : actions)
    {
        action->Update();
    }

}

void Scene::AddActionToObj(Action* action, Object* object)
{
    action->object = object;
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
