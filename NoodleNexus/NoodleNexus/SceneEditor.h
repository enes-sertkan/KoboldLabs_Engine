#pragma once
#include "Scene.hpp"
#include "Transform.h"
#include "GLCommon.h"
#include "sObject.h"

#include "SceneEditor.h"
#include "KLFileManager.hpp"
#include "cVAOManager/cVAOManager.h";

//#include "linmath.h"
#include <glm/glm.hpp>
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4

class SceneEditor
{
public:
	void Start(std::string selectBoxPath, KLFileManager klFileManger, GLuint program, GLFWwindow* currentWindow, cVAOManager* managerVAO, Scene* currentScene);


	Scene* scene = nullptr; //Current scene

	Object* selectedObject = nullptr;

	int objIndex=0;

	double currentFrameTime, lastFrameTime;

	Object* selectBox;

	cLightManager::sLight* PickFirstLight();


		cLightManager::sLight* PickNextLight();


	bool isEditing;

	GLFWwindow* window = nullptr;


	std::string editMode;


	void Update();

	cLightManager::sLight* selectedLight;
	int lightIndex=0;

	cLightManager lightManager;
	
	void HandleInputAsync(GLFWwindow* window);

	void SaveScene(std::string name);


	Object* PickFirstObject();
	Object* PickNextObject();

	void ChangeMode(std::string mode);

private:
	void UpdateSelectBox();


	
};

