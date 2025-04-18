#pragma once

#include "sObject.h";
#include "cVAOManager/cVAOManager.h"
#include "GLCommon.h"
#include "sharedThings.h"
#include "Action.h"
#include "Transform.h"
#include "cBasicFlyCamera/cBasicFlyCamera.h"
#include "PhysicsManager.h"
#include "cBasicTextureManager.h"
#include "LuaScript.h"
#include "cVAOManager/cVAOManager.h"
#include "GLCommon.h"

#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <cmath> 
#include <algorithm>
#include <vector>
#include <Lua5.4.7/lua.hpp>
#include <chrono>
#include <vector>
#include <iostream>
#include "CameraPoint.h"
#include "Camera.hpp"
#include "cFBO_RGB_depth.hpp"
//#include "Animator.h"

class SceneEditor;

//This is a class bc we gonna have functions in it later
class Scene
{
	
private:
	std::chrono::steady_clock::time_point previousTime;
	std::chrono::steady_clock::time_point currentTime;
	sMesh* pSphere = NULL;
	lua_State* L;

	std::vector<Object*> GetObjectsByTransparency(bool transparent) const {
		std::vector<Object*> result;
		for (auto obj : sceneObjectsSorted) {
			if (obj->mesh->transperency < 1.0f == transparent) {
				result.push_back(obj);
			}
		}
		return result;
	}
public:
	//Animator* animator = nullptr;
	SceneEditor* sceneEditor = nullptr;
	Object* selectedObject = nullptr;

	float cameraMoveSpeed=200;
	float cameraRotateSpeed=50;

	Object* skybox = nullptr;

	void SortObjectsForDrawing();
	bool CompareObjectsBasedOnDistanecToCamera(Object* a, Object* b);

	cBasicTextureManager* textureManager;

	void DrawMesh(sMesh* pCurMesh, GLuint program);
	std::vector<GLuint> programs;
	GLuint depthProgram;
	GLuint particleProgram;
	void DrawSphere(glm::vec3 position, glm::vec4 RGBA, float scale, GLuint program);
	void DrawRay(glm::vec3 pos, glm::vec3 posEnd, GLuint program, glm::vec4 color, bool isWire, float scale);

	float deltaTime=0.00001f;//make it small, but at least something

	void UpdateDeltaTime();

	bool isFlyCamera = true;
	std::vector<sModelDrawInfo> modelInfos;

	std::vector<Object*> sceneObjects;
	std::vector<Object*> sceneObjectsSorted;
	std::vector<Camera*> cameras;

	Camera* AddCamera(glm::vec3 startPosition, glm::vec3 startRotation, glm::vec2 res);


	std::vector<Action*> actions;

	cVAOManager* vaoManager;

	cLightManager* lightManager;
	PhysicsManager* physicsManager;
	cFBO_RGB_depth* depthFBO;
	std::vector<std::string> modelPaths;
	std::vector<CameraPoint> cameraPoints;
	CameraPoint currentCameraPoint;
	GLFWwindow* window = nullptr;
	cBasicFlyCamera* fCamera = nullptr;

	void MoveCameraToPoint();
	void SetCameraToNextPoint();
	void SetCameraToFirstPoint();
	void SetCameraToPlayer();
	void NextCameraPoint();
	void SetCurrentCameraToID();
	void SetCurrentCamera(CameraPoint camPoint);

	void RemoveObject(Object* object);
	
	int currentCameraIndex=0;

	void Start();
	void Update();

	void Prepare(cVAOManager* meshManager, GLuint program, PhysicsManager* physMan, GLFWwindow* newWindow, cBasicFlyCamera* newFlyCamera);

	
	void AddActionToObj(Action* action, Object* object);


	std::vector<Object*> GetOpaqueObjects() const {
		return GetObjectsByTransparency(false);
	}

	std::vector<Object*> GetTransparentObjects() const {
		return GetObjectsByTransparency(true);
	}

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