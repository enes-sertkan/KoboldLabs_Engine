#pragma once

#include "sObject.h";
#include <vector>
#include "sObject.h";

#include "cVAOManager/cVAOManager.h"
#include "GLCommon.h"
#include <iostream>
#include "sharedThings.h"
#include "Action.h"
#include "Transform.h"
#include "cBasicFlyCamera/cBasicFlyCamera.h"
//This is a class bc we gonna have functions in it later
class Scene
{
public:
	bool flyCamera = true;
	std::vector<sModelDrawInfo> modelInfos;

	std::vector<Object*> sceneObjects;

	std::vector<Action*> actions;

	cLightManager* lightManager;

	std::vector<std::string> modelPaths;
	std::vector<Transform*> cameraPositions;
	void MoveCameraToPoint();
	void SetCameraToNextPoint();
	void SetCameraToFirstPoint();
	void NextCameraPoint();

	void RemoveObject(Object* object);
	
	int currentCameraIndex=0;


	void Update();

	void Prepare(cVAOManager* meshManager, GLuint program, std::vector<sMesh*>& meshes);

	
	void AddActionToObj(Action* action, Object* object);
	Object* CreateObject(glm::vec3 pos, glm::vec3 rotation, float scale, glm::vec4 color, std::string name, std::string modelName);

	
};