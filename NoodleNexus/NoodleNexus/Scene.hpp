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
#include "cBasicTextureManager.h"



//This is a class bc we gonna have functions in it later
class Scene
{
	
private:
	std::chrono::steady_clock::time_point previousTime;
	std::chrono::steady_clock::time_point currentTime;
	sMesh* pSphere = NULL;

public:


	void SortObjectsForDrawing();
	bool CompareObjectsBasedOnDistanecToCamera(Object* a, Object* b);

	cBasicTextureManager* textureManager;

	void DrawMesh(sMesh* pCurMesh, GLuint program);
	std::vector<GLuint> programs;
	void DrawSphere(glm::vec3 position, glm::vec4 RGBA, float scale, GLuint program);
	void DrawRay(glm::vec3 pos, glm::vec3 posEnd, GLuint program, glm::vec4 color, bool isWire, float scale);

	float deltaTime=0.00001f;//make it small, but at least something

	void UpdateDeltaTime();

	bool isFlyCamera = true;
	std::vector<sModelDrawInfo> modelInfos;

	std::vector<Object*> sceneObjects;
	std::vector<Object*> sceneObjectsSorted;

	std::vector<Action*> actions;

	cVAOManager* vaoManager;

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

	void Start();
	void Update();

	void Prepare(cVAOManager* meshManager, GLuint program, PhysicsManager* physMan, GLFWwindow* newWindow, cBasicFlyCamera* newFlyCamera);

	
	void AddActionToObj(Action* action, Object* object);

	Object* GenerateMeshObjectsFromObject(
		const std::string& filePath, glm::vec3 posXYZ, float scale, glm::vec3 rotXYZ,
		bool bOverrideColor, glm::vec4 objectColor, bool bDoLightingExist,
		std::vector<Object*>& sceneObjects
	);

	Object* SetLight(cLightManager* lightManager, int index,
		const glm::vec4& position,
		const glm::vec4& diffuse,
		const glm::vec3& attenuation,
		const glm::vec4& direction,
		const glm::vec3& param1,
		float param2x);

	void ErrorCallback(int error, const char* description);

	
};