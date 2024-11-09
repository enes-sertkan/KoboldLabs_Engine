#pragma once

#include "sObject.h";
#include <vector>
#include "cVAOManager/cVAOManager.h"
#include "GLCommon.h"
#include <iostream>
#include "sharedThings.h"
#include "Action.h"
#include "Transform.h"
#include "cBasicFlyCamera/cBasicFlyCamera.h"
#include "PhysicsManager.h"
#include <chrono>


//This is a class bc we gonna have functions in it later
class Scene
{
	
private:
	std::chrono::steady_clock::time_point previousTime;
	std::chrono::steady_clock::time_point currentTime;


public:

	float deltaTime=0.00001f;//make it small, but at least something

	void UpdateDeltaTime();

	bool isFlyCamera = true;
	std::vector<sModelDrawInfo> modelInfos;

	std::vector<Object*> sceneObjects;

	std::vector<Action*> actions;

	cLightManager* lightManager;
	PhysicsManager* physicsManager;

	std::vector<std::string> modelPaths;
	std::vector<Transform*> cameraPositions;
	GLFWwindow* window = nullptr;
	cBasicFlyCamera* fCamera = nullptr;

	void MoveCameraToPoint();
	void SetCameraToNextPoint();
	void SetCameraToFirstPoint();
	void NextCameraPoint();

	void RemoveObject(Object* object);
	
	int currentCameraIndex=0;


	void Update();

	void Prepare(cVAOManager* meshManager, GLuint program, std::vector<sMesh*>& meshes, PhysicsManager* physMan, GLFWwindow* newWindow, cBasicFlyCamera* newFlyCamera);

	
	void AddActionToObj(Action* action, Object* object);

	Object* GenerateMeshObjectsFromObject(
		const std::string& filePath, glm::vec3 posXYZ, float scale, glm::vec3 rotXYZ,
		bool bOverrideColor, glm::vec4 objectColor, bool bDoLightingExist,
		std::vector<Object*>& sceneObjects
	);

	
};