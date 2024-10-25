#include "sObject.h";
#include <vector>
#include "sObject.h";

#include "cVAOManager/cVAOManager.h"
#include "GLCommon.h"
#include <iostream>
//This is a class bc we gonna have functions in it later
class Scene
{
public:

	std::vector<sModelDrawInfo> modelInfos;

	std::vector<sObject*> sceneObjects;

	void Update();

	void Prepare(cVAOManager* meshManager, GLuint program, std::vector<sMesh*>& meshes);

	


	
};