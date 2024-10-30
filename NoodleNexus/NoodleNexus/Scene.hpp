#pragma once

#include "sObject.h";
#include <vector>
#include "sObject.h";

#include "cVAOManager/cVAOManager.h"
#include "GLCommon.h"
#include <iostream>
#include <GLFW/glfw3.h>
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

	double currentTime, lastTime, deltaTime;

	void CalculateDeltaTime()
	{
		// Get the current time using glfwGetTime
		currentTime = glfwGetTime();

		// Calculate deltaTime
		deltaTime = currentTime - lastTime;

		// Update lastTime for the next frame
		lastTime = currentTime;
	}

	Object* CreateObject(glm::vec3 position, glm::vec3 rotation, float scale, glm::vec4 color, const std::string& name, const std::string& modelPath) {
		Object* newObject = new Object();
		newObject->name = name;
		newObject->mesh = new sMesh();
		newObject->mesh->modelFileName = modelPath;
		newObject->mesh->positionXYZ = position;
		newObject->mesh->rotationEulerXYZ = rotation;
		newObject->mesh->uniformScale = scale;
		newObject->mesh->objectColourRGBA = color;
		newObject->mesh->bOverrideObjectColour = true;

		// Add the new object to the scene
		sceneObjects.push_back(newObject);
		return newObject; // Return the created object
	}

	
};