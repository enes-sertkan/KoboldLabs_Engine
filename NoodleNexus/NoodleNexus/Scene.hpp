#pragma once

#include "sObject.h";
#include <vector>
#include "sObject.h";

#include "cVAOManager/cVAOManager.h"
#include "GLCommon.h"
#include <iostream>
#include "sharedThings.h"
#include "Action.h"
//This is a class bc we gonna have functions in it later
class Scene
{
public:

	std::vector<sModelDrawInfo> modelInfos;

	std::vector<Object*> sceneObjects;

	std::vector<Action*> actions;

	cLightManager* lightManager;

	std::vector<std::string> modelPaths;

	

	void Update();

	void Prepare(cVAOManager* meshManager, GLuint program, std::vector<sMesh*>& meshes);

	
	void AddActionToObj(Action* action, Object* object);

	
};