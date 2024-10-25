#include "sObject.h";
#include <vector>
#include "sObject.h";

//This is a class bc we gonna have functions in it later
class Scene
{
public:

	std::vector<sModelDrawInfo> modelInfos;

	std::vector<sObject*> sceneObjects;

	void Update();

	void Prepare();

	
	


	
};