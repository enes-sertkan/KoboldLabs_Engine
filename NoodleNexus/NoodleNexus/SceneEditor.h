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
#include "cTurret.h"
#include "TurretConfig.h"

class SceneEditor
{
public:
	Turret* ghostTurret;

	void Start(std::string selectBoxPath, KLFileManager* klFileManger, GLuint program, GLFWwindow* currentWindow, cVAOManager* managerVAO, Scene* currentScene, LabAttackFactory* factory);

	KLFileManager* fileManger = nullptr;
	Scene* scene = nullptr; //Current scene

	std::string currentFilename = "SaveScene.txt";
	bool showSaveAsPopup = false;
	bool showLoadScenePopup = false;
	char newFilenameBuffer[256] = "SaveScene.txt";
	char loadFilenameBuffer[256] = "NewScene.txt";

	Object* selectedObject = nullptr;

	int objIndex=0;
	bool selectWireMode = false;
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


	
	void HandleInputAsync(GLFWwindow* window);

	void SaveScene(std::string name);


	Object* PickFirstObject();
	Object* PickNextObject();

	void ChangeMode(std::string mode);

private:
	void UpdateSelectBox();


	
};

